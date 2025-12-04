#ifndef __ERDP_HAL_UART_GPP__
#define __ERDP_HAL_UART_GPP__

#include "erdp_hal.hpp"
#include "erdp_if_uart.h"
#include "erdp_if_gpio.h"

#include <type_traits>
#include <vector>
namespace erdp
{
    extern "C"
    {
        void erdp_uart_irq_handler(ERDP_Uart_t uart);
    }

    typedef struct
    {
        ERDP_Uart_t uart;     // UART number
        uint32_t baudrate;    // Baudrate for the UART
        ERDP_UartMode_t mode; // Mode for the UART (TX/RX, TX only, RX only)

        ERDP_GpioPort_t tx_port; // GPIO port for TX pin
        ERDP_GpioPin_t tx_pin;   // GPIO pin for TX pin
        uint32_t tx_af;          // Alternate function for TX pin

        ERDP_GpioPort_t rx_port; // GPIO port for RX pin
        ERDP_GpioPin_t rx_pin;   // GPIO pin for RX pin
        uint32_t rx_af;          // Alternate function for RX pin

        uint8_t priority; // Priority for the UART receive interrupt

    } UartConfig_t;

    class UartDev
    {
        friend void erdp_uart_irq_handler(ERDP_Uart_t uart);
#ifdef ERDP_ENABLE_RTOS
#define GET_SYS_TICK() Thread::get_system_1ms_ticks()
        using Buffer = Queue<uint8_t>;
#else
        using Buffer = RingBuffer<uint8_t>;
#define GET_SYS_TICK() erdp_if_rtos_get_system_1ms_ticks()
#endif

    public:
        UartDev() {}
        UartDev(const UartConfig_t &config, size_t recv_buffer_size)
        {
            __init(config, recv_buffer_size);
        }

        void init(const UartConfig_t &config, size_t recv_buffer_size)
        {
            __init(config, recv_buffer_size);
        }

        void send(const uint8_t *data, uint32_t len) const
        {
            erdp_if_uart_send_bytes(__uart, data, len);
        }

        void send(const std::vector<uint8_t> &data) const{
            erdp_if_uart_send_bytes(__uart, data.data(), data.size());
        }

        bool recv(std::vector<uint8_t> &buffer, uint32_t timeout = 5)
        {
            bool ret = false;
            uint8_t data;
            buffer.clear();
            uint32_t start_time = GET_SYS_TICK();

            while (GET_SYS_TICK() - start_time < timeout)
            {
                if (__recv_buffer.pop(data))
                {
                    buffer.push_back(data);
                    ret = true;
                    start_time = GET_SYS_TICK(); // Reset the timer on successful receive
                }
            }

            return ret;
        }
        bool recv(uint8_t &data)
        {
            return __recv_buffer.pop(data);
        }

        void set_usr_irq_handler(std::function<void()> usr_irq_handler)
        {
            __usr_irq_handler = usr_irq_handler;
        }

        void set_as_debug_com()
        {
            __debug_com = this;
        }

        static const UartDev *const &get_debug_com()
        {
            return __debug_com;
        }

    private:
        ERDP_Uart_t __uart = ERDP_UART0;           // Default to UART0
        static UartDev *__instance[ERDP_UART_NUM]; // Array to hold instances for each UART
        static UartDev *__debug_com;
        uint8_t __data;
        Buffer __recv_buffer;
        std::function<void()> __usr_irq_handler = nullptr;

        void __init(const UartConfig_t &config, size_t recv_buffer_size)
        {
            if (!__recv_buffer.init(recv_buffer_size))
            {
                erdp_assert(false);
                return;
            }
            ERDP_UartGpioCfg_t gpio_cfg = {
                .tx_port = config.tx_port,
                .tx_pin = config.tx_pin,
                .tx_af = config.tx_af,
                .rx_port = config.rx_port,
                .rx_pin = config.rx_pin,
                .rx_af = config.rx_af};
            __uart = config.uart;

            __instance[__uart] = this; // Store the instance for the IRQ handler

            erdp_if_uart_init(config.uart, config.baudrate, config.mode, config.priority);
            erdp_if_uart_gpio_init(&gpio_cfg);
        }

        void __irq_handler()
        {
            erdp_if_uart_read_byte(__uart, &__data);
            if(__recv_buffer.push(__data))
            {
                
            }
			
            if (__usr_irq_handler != nullptr)
            {
                __usr_irq_handler(); // Call the user-defined function
            }
        }
    };
} // namespace erdp

#endif // ERDP_HAL_UART_GPP
