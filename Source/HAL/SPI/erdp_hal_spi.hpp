#ifndef __ERDP_HAL_SPI_HPP__
#define __ERDP_HAL_SPI_HPP__
#include "erdp_if_spi.h"
#include "erdp_hal.hpp"

#include <vector>

namespace erdp
{
    extern "C"
    {
        void erdp_spi_irq_handler(ERDP_Spi_t spi);
    }
    using SpiConfig_t = ERDP_SpiCfg_t;
    using SpiInfo_t = ERDP_SpiInfo_t;

    class SpiBase
    {
    public:
        SpiBase() {}
        const SpiInfo_t &get_spi_info() const { return __spi_info; }

    protected:
        SpiInfo_t __spi_info;
        SpiConfig_t __spi_cfg;
        static SpiBase *__spi_instance[ERDP_SPI_NUM];

    private:
        friend void erdp_spi_irq_handler(ERDP_Spi_t spi);
        virtual void __irq_handler() {}
    };

    template <ERDP_SpiDataSize_t DATA_SIZE>
    class SpiDevBase : public SpiBase
    {
    public:
        using DataType = typename std::conditional<DATA_SIZE == ERDP_SPI_DATASIZE_8BIT,
                                                   uint8_t,
                                                   uint16_t>::type;
#ifdef ERDP_ENABLE_RTOS
        using Buffer = Queue<DataType>;
#else
        using Buffer = RingBuffer<DataType>;
#endif

        Buffer &rx_buffer = __rx_buffer;

        SpiDevBase() {}
        SpiDevBase(const SpiInfo_t &spi_info, ERDP_SpiMode_t mode, const SpiConfig_t &spi_cfg, uint32_t rx_buffer_size)
        {
            __init(spi_info, mode, spi_cfg, rx_buffer_size);
        }

        void enable()
        {
            erdp_if_spi_enable(__spi_info.spi, true);
        }

        void disable()
        {
            erdp_if_spi_enable(__spi_info.spi, false);
        }

        virtual bool send(DataType *data, uint32_t len) = 0;
        virtual bool recv(uint32_t &&len) = 0;

    protected:
        void __init(const SpiInfo_t &spi_info, ERDP_SpiMode_t mode, const SpiConfig_t &spi_cfg, uint32_t rx_buffer_size)
        {
            __spi_info = spi_info;
            __spi_cfg = spi_cfg;
            __rx_buffer.init(rx_buffer_size);
            __spi_instance[__spi_info.spi] = this;
            erdp_if_spi_init(__spi_info.spi, mode, &__spi_cfg, DATA_SIZE);
            erdp_if_spi_gpio_init(&__spi_info, mode);
        }
        Buffer __rx_buffer;

    private:
    };

    template <ERDP_SpiDataSize_t DATA_SIZE = ERDP_SPI_DATASIZE_8BIT>
    class SpiMasterBase : public SpiDevBase<DATA_SIZE>
    {
    public:
        SpiMasterBase() = default; // Add default constructor
        SpiMasterBase(const SpiInfo_t &spi_info, const SpiConfig_t &spi_cfg, uint32_t rx_buffer_size)
            : SpiDevBase<DATA_SIZE>(spi_info, ERDP_SPI_MODE_MASTER, spi_cfg, rx_buffer_size)
        {
        }

        void cs_high()
        {
            erdp_if_gpio_write(SpiBase::__spi_info.cs_port, SpiBase::__spi_info.cs_pin, ERDP_SET);
        }
        void cs_low()
        {
            erdp_if_gpio_write(SpiBase::__spi_info.cs_port, SpiBase::__spi_info.cs_pin, ERDP_RESET);
        }

        bool send(typename SpiDevBase<DATA_SIZE>::DataType *data, uint32_t len) override
        {
            uint32_t tx_count = 0;
            while (tx_count < len)
            {
                while (!erdp_if_spi_transmit_buffer_empty(SpiBase::__spi_info.spi))
                    ;
                erdp_if_spi_send(SpiBase::__spi_info.spi, data[tx_count++]);
            }
            while (!erdp_if_spi_transfer_complete(SpiBase::__spi_info.spi))
                ;
            erdp_if_spi_recv(SpiBase::__spi_info.spi);

            return true;
        }

        bool recv(uint32_t &&len) override
        {
            uint32_t rx_count = 0;
            typename SpiDevBase<DATA_SIZE>::DataType data = 0;
            erdp_if_spi_recv(SpiBase::__spi_info.spi);
            while (rx_count < len)
            {
                while (!erdp_if_spi_transmit_buffer_empty(SpiBase::__spi_info.spi))
                    ;
                erdp_if_spi_send(SpiBase::__spi_info.spi, 0x00);
                while (!erdp_if_spi_receive_buffer_not_empty(SpiBase::__spi_info.spi))
                    ;
                data = static_cast<typename SpiDevBase<DATA_SIZE>::DataType>(
                    erdp_if_spi_recv(SpiBase::__spi_info.spi));
                SpiDevBase<DATA_SIZE>::__rx_buffer.push(data);
                rx_count++;
            }
            while (!erdp_if_spi_transfer_complete(SpiBase::__spi_info.spi))
                ;
            return true;
        }

        bool recv(typename SpiDevBase<DATA_SIZE>::DataType *buffer, uint32_t len)
        {
            uint32_t rx_count = 0;
            typename SpiDevBase<DATA_SIZE>::DataType data = 0;
            while (rx_count < len)
            {
                while (!erdp_if_spi_transmit_buffer_empty(SpiBase::__spi_info.spi))
                    ;
                erdp_if_spi_send(SpiBase::__spi_info.spi, 0x00);
                while (!erdp_if_spi_receive_buffer_not_empty(SpiBase::__spi_info.spi))
                    ;
                data = static_cast<typename SpiDevBase<DATA_SIZE>::DataType>(erdp_if_spi_recv(SpiBase::__spi_info.spi));
                buffer[rx_count++] = data;
            }
            while (!erdp_if_spi_transfer_complete(SpiBase::__spi_info.spi))
                ;
            return true;
        }

        bool send_recv(typename SpiDevBase<DATA_SIZE>::DataType *tx_data, uint32_t tx_len, uint32_t rx_len)
        {
            uint32_t tx_size = tx_len > rx_len ? tx_len : rx_len;
            uint32_t tx_count = 0;
            uint32_t rx_count = 0;
            uint8_t txallowed = 1;
            typename SpiDevBase<DATA_SIZE>::DataType data = 0;
            while (tx_count < tx_size || rx_count < rx_len)
            {
                if (txallowed)
                {
                    txallowed = 0;
                    while (!erdp_if_spi_transmit_buffer_empty(SpiBase::__spi_info.spi))
                        ;
                    if (tx_count < tx_len)
                    {
                        erdp_if_spi_send(SpiBase::__spi_info.spi, tx_data[tx_count]);
                    }
                    else
                    {
                        erdp_if_spi_send(SpiBase::__spi_info.spi, 0x00);
                    }
                    tx_count++;
                }
                if (rx_count < rx_len)
                {
                    while (!erdp_if_spi_receive_buffer_not_empty(SpiBase::__spi_info.spi))
                        ;
                    data = static_cast<typename SpiDevBase<DATA_SIZE>::DataType>(erdp_if_spi_recv(SpiBase::__spi_info.spi));
                    SpiDevBase<DATA_SIZE>::__rx_buffer.push(data);
                    rx_count++;
                    txallowed = 1;
                }
                else
                {
                    txallowed = 1;
                }
            }
            while (!erdp_if_spi_transfer_complete(SpiBase::__spi_info.spi))
                ;
            erdp_if_spi_recv(SpiBase::__spi_info.spi);
            return true;
        }

    protected:
        void __init(const SpiInfo_t &spi_info, const SpiConfig_t &spi_cfg, uint32_t rx_buffer_size)
        {
            SpiDevBase<DATA_SIZE>::__init(spi_info, ERDP_SPI_MODE_MASTER, spi_cfg, rx_buffer_size);
        }
    };

    template <ERDP_SpiDataSize_t DATA_SIZE = ERDP_SPI_DATASIZE_8BIT>
    class SpiSlaveBase : public SpiDevBase<DATA_SIZE>
    {
    public:
        SpiSlaveBase() = default; // Add default constructor
        SpiSlaveBase(const SpiInfo_t &spi_info, const SpiConfig_t &spi_cfg, uint32_t rx_buffer_size, uint32_t tx_buffer_size)
            : SpiDevBase<DATA_SIZE>(spi_info, ERDP_SPI_MODE_SLAVE, spi_cfg, rx_buffer_size)
        {
            __tx_buffer.init(tx_buffer_size);
        }

        bool send(typename SpiDevBase<DATA_SIZE>::DataType *data, uint32_t len) override
        {
            __load_tx_buffer(data, len);
            return true;
        }

        bool recv(uint32_t &&len) override
        {
            return true;
        }

        bool is_send_complete()
        {
            return __tx_buffer.empty();
        }

        void set_usr_rx_irq_handler(std::function<void(typename SpiDevBase<DATA_SIZE>::DataType)> handler)
        {
            __usr_rx_irq_handler = handler;
        }

        void clear_usr_rx_irq_handler()
        {
            __usr_rx_irq_handler = nullptr;
        }

    protected:
        void __init(const SpiInfo_t &spi_info, const SpiConfig_t &spi_cfg, uint32_t rx_buffer_size, uint32_t tx_buffer_size)
        {
            SpiDevBase<DATA_SIZE>::__init(spi_info, ERDP_SPI_MODE_SLAVE, spi_cfg, rx_buffer_size);
            __tx_buffer.init(tx_buffer_size);
        }

    private:
        uint32_t __tx_count = 0;
        typename SpiDevBase<DATA_SIZE>::DataType __data;
        typename SpiDevBase<DATA_SIZE>::Buffer __tx_buffer;
        std::function<void(typename SpiDevBase<DATA_SIZE>::DataType)> __usr_rx_irq_handler = nullptr;
        bool __load_tx_buffer(typename SpiDevBase<DATA_SIZE>::DataType *data, uint32_t len)
        {
            __tx_count = 0;
            while (__tx_count < len)
            {
                if (__tx_count == 0 && erdp_if_spi_transmit_buffer_empty(SpiBase::__spi_info.spi))
                {
                    erdp_if_spi_send(SpiBase::__spi_info.spi, data[__tx_count++]);
                }
                else
                {
                    if (!__tx_buffer.push(data[__tx_count++]))
                    {
                        return false;
                    }
                }
            }
            return true;
        }
        void __irq_handler() override
        {
            if (erdp_if_spi_transmit_buffer_empty(SpiBase::__spi_info.spi))
            {
                if (__tx_buffer.pop(__data))
                {
                    erdp_if_spi_send(SpiBase::__spi_info.spi, __data);
                }
            }
            if (erdp_if_spi_receive_buffer_not_empty(SpiBase::__spi_info.spi))
            {
                __data = static_cast<typename SpiDevBase<DATA_SIZE>::DataType>(erdp_if_spi_recv(SpiBase::__spi_info.spi));
                SpiDevBase<DATA_SIZE>::__rx_buffer.push(__data);
                if (__usr_rx_irq_handler)
                {
                    __usr_rx_irq_handler(__data);
                }
            }
        }
    };
    template <ERDP_SpiMode_t MODE,
              ERDP_SpiDataSize_t DATA_SIZE = ERDP_SPI_DATASIZE_8BIT>
    class SpiDev : public std::conditional_t<MODE == ERDP_SPI_MODE_MASTER,
                                             SpiMasterBase<DATA_SIZE>,
                                             SpiSlaveBase<DATA_SIZE>>
    {
    public:
        // Default constructor
        SpiDev() = default;

        // Master mode constructor
        template <ERDP_SpiMode_t M = MODE, typename = std::enable_if_t<M == ERDP_SPI_MODE_MASTER>>
        SpiDev(const SpiInfo_t &spi_info, const SpiConfig_t &spi_cfg, uint32_t rx_buffer_size)
            : SpiMasterBase<DATA_SIZE>(spi_info, spi_cfg, rx_buffer_size) {}

        // Slave mode constructor
        template <ERDP_SpiMode_t M = MODE, typename = std::enable_if_t<M == ERDP_SPI_MODE_SLAVE>>
        SpiDev(const SpiInfo_t &spi_info, const SpiConfig_t &spi_cfg, uint32_t rx_buffer_size, uint32_t tx_buffer_size)
            : SpiSlaveBase<DATA_SIZE>(spi_info, spi_cfg, rx_buffer_size, tx_buffer_size) {}

        // Master mode init
        template <ERDP_SpiMode_t M = MODE, typename = std::enable_if_t<M == ERDP_SPI_MODE_MASTER>>
        void init(const SpiInfo_t &spi_info, const SpiConfig_t &spi_cfg, uint32_t rx_buffer_size)
        {
            SpiMasterBase<DATA_SIZE>::__init(spi_info, spi_cfg, rx_buffer_size);
        }

        // Slave mode init
        template <ERDP_SpiMode_t M = MODE, typename = std::enable_if_t<M == ERDP_SPI_MODE_SLAVE>>
        void init(const SpiInfo_t &spi_info, const SpiConfig_t &spi_cfg, uint32_t rx_buffer_size, uint32_t tx_buffer_size)
        {
            SpiSlaveBase<DATA_SIZE>::__init(spi_info, spi_cfg, rx_buffer_size, tx_buffer_size);
        }
    };
} // namespace erdp
#endif
