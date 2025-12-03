#ifndef __ERDP_IF_SPI_H__
#define __ERDP_IF_SPI_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
#include "erdp_interface.h"
#include "erdp_if_gpio.h"

    typedef enum
    {
        ERDP_SPI0 = 0,
        ERDP_SPI1,
        ERDP_SPI2,
        ERDP_SPI3,
        ERDP_SPI_NUM, // Maximum number of SPI
    } ERDP_Spi_t;

    typedef enum
    {
        ERDP_SPI_MODE_MASTER = 0, /* SPI master mode */
        ERDP_SPI_MODE_SLAVE,      /* SPI slave mode */
    } ERDP_SpiMode_t;

    typedef enum
    {
        ERDP_SPI_CLKMODE_0 = 0, /* CPOL=0, CPHA=0: Clock idle low, data sampled on rising edge */
        ERDP_SPI_CLKMODE_1,     /* CPOL=0, CPHA=1: Clock idle low, data sampled on falling edge */
        ERDP_SPI_CLKMODE_2,     /* CPOL=1, CPHA=0: Clock idle high, data sampled on falling edge */
        ERDP_SPI_CLKMODE_3      /* CPOL=1, CPHA=1: Clock idle high, data sampled on rising edge */
    } ERDP_SpiClkMode_t;

    typedef enum
    {
        ERDP_SPI_ENDIAN_MSB = 0, /* Data transmitted MSB (Most Significant Bit) first */
        ERDP_SPI_ENDIAN_LSB      /* Data transmitted LSB (Least Significant Bit) first */
    } ERDP_SpiEndian_t;

    typedef enum
    {
        ERDP_SPI_DATASIZE_8BIT = 0, /* 8-bit data transfer */
        ERDP_SPI_DATASIZE_16BIT     /* 16-bit data transfer */
    } ERDP_SpiDataSize_t;

    typedef struct
    {
        ERDP_SpiClkMode_t clk_mode;
        ERDP_SpiEndian_t endian;
        uint32_t prescale;
        uint8_t priority;
    } ERDP_SpiCfg_t;

    typedef struct
    {
        ERDP_Spi_t spi;

        ERDP_GpioPort_t sck_port; // GPIO port for SCK pin
        ERDP_GpioPin_t sck_pin;   // GPIO pin for SCK pin
        uint32_t sck_af;          // GPIO function number for SCK pin

        ERDP_GpioPort_t mosi_port; // GPIO port for MOSI pin
        ERDP_GpioPin_t mosi_pin;   // GPIO pin for MOSI pin
        uint32_t mosi_af;          // GPIO function number for MOSI pin

        ERDP_GpioPort_t miso_port; // GPIO port for MISO pin
        ERDP_GpioPin_t miso_pin;   // GPIO pin for MISO pin
        uint32_t miso_af;          // GPIO function number for MISO pin

        ERDP_GpioPort_t cs_port; // GPIO port for CS pin
        ERDP_GpioPin_t cs_pin;   // GPIO pin for CS pin
        uint32_t cs_af;          // GPIO function number for CS pin

    } ERDP_SpiInfo_t;

    /**
     * @brief Initialize SPI GPIO pins
     * @param[in] spi_info Pointer to SPI GPIO configuration structure
     * @param[in] mode SPI operation mode (master/slave)
     */
    void erdp_if_spi_gpio_init(ERDP_SpiInfo_t *spi_info, ERDP_SpiMode_t mode);

    /**
     * @brief Initialize SPI peripheral
     * @param[in] spi SPI instance identifier
     * @param[in] mode SPI operation mode (master/slave)
     * @param[in] spi_cfg Pointer to SPI configuration structure
     * @param[in] data_size Data size (8/16 bit)
     */
    void erdp_if_spi_init(ERDP_Spi_t spi, ERDP_SpiMode_t mode, ERDP_SpiCfg_t *spi_cfg, ERDP_SpiDataSize_t data_size);

    /**
     * @brief Deinitialize SPI peripheral
     * @param[in] spi SPI instance identifier
     */
    void erdp_if_spi_deinit(ERDP_Spi_t spi);

    /**
     * @brief Enable SPI peripheral
     * @param[in] spi SPI instance identifier
     * @param[in] enable true to enable SPI, false to disable SPI
     */
    void erdp_if_spi_enable(ERDP_Spi_t spi, bool enable);

    /**
     * @brief Send data through SPI
     * @param[in] spi SPI instance identifier
     * @param[in] data Data to be transmitted (8/16 bit depending on configuration)
     */
    void erdp_if_spi_send(ERDP_Spi_t spi, uint16_t data);

    /**
     * @brief Receive data from SPI
     * @param[in] spi SPI instance identifier
     * @return Received data (8/16 bit depending on configuration)
     */
    uint16_t erdp_if_spi_recv(ERDP_Spi_t spi);

    /**
     * @brief Check if SPI transfer is complete
     * @param[in] spi SPI instance identifier
     * @return true if transfer is complete, false otherwise
     */
    bool erdp_if_spi_transfer_complete(ERDP_Spi_t spi);

    /**
     * @brief Check if SPI transmit buffer is empty
     * @param[in] spi SPI instance identifier
     * @return true if transmit buffer is empty, false otherwise
     */
    bool erdp_if_spi_transmit_buffer_empty(ERDP_Spi_t spi);

    /**
     * @brief Check if SPI receive buffer is not empty
     * @param[in] spi SPI instance identifier
     * @return true if receive buffer contains data, false otherwise
     */
    bool erdp_if_spi_receive_buffer_not_empty(ERDP_Spi_t spi);

#ifdef __cplusplus
}
#endif

#endif //__ERDP_IF_SPI_H__
