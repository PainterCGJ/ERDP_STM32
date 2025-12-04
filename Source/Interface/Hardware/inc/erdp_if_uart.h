#ifndef __ERDP_IF_UART_H__
#define __ERDP_IF_UART_H__
#ifdef __cplusplus
 extern "C" {
 #endif // __cplusplus
#include "erdp_interface.h"
#include "erdp_if_gpio.h"

typedef enum
{
    ERDP_UART0 = 0,
    ERDP_UART1,
    ERDP_UART2,
    ERDP_UART3,
    ERDP_UART4,
    ERDP_UART5,
    ERDP_UART6,
    ERDP_UART_NUM, // Maximum number of UARTs
} ERDP_Uart_t;

typedef enum{
    ERDP_UART_TX_RX = 0,  // Transmit and Receive mode
    ERDP_UART_TX_ONLY,    // Transmit only mode
    ERDP_UART_RX_ONLY,    // Receive only mode
}ERDP_UartMode_t;

typedef enum{
    ERDP_UART_INT_FLAG_TBE = 0, // Transmit buffer empty flag
    ERDP_UART_INT_FLAG_TC,      // Transmit complete flag
    ERDP_UART_INT_FLAG_RBNE,    // Receive buffer not empty flag
    ERDP_UART_INT_FLAG_IDLE,    // Idle line detected flag
}ERDP_UartIrqFlag_t;

typedef struct{
    ERDP_GpioPort_t tx_port; // GPIO port for TX pin
    ERDP_GpioPin_t tx_pin;   // GPIO pin for TX pin
    uint32_t tx_af;          // GPIO alternate function for TX pin
    ERDP_GpioPort_t rx_port; // GPIO port for RX pin
    ERDP_GpioPin_t rx_pin;   // GPIO pin for RX pin
    uint32_t rx_af;          // GPIO alternate function for RX pin
}ERDP_UartGpioCfg_t;

/**
 * @brief  Get the SOC UART base address for a given ERDP UART.
 * @param[in]  uart: The ERDP UART to get the SOC UART base address for.
 * @return  The base address of the SOC UART.
 */
uint32_t erdp_if_uart_get_base(ERDP_Uart_t uart);

/**
 * @brief  Get the SOC UART PCLK for a given ERDP UART.
 * @param[in]  uart: The ERDP UART to get the SOC UART PCLK for.
 * @return  The PCLK of the SOC UART.
 */
uint32_t erdp_if_uart_get_PCLK(ERDP_Uart_t uart);

/**
 * @brief Initialize UART GPIO pins configuration
 * @param[in] gpio_cfg: Pointer to UART GPIO configuration structure
 */
void erdp_if_uart_gpio_init(ERDP_UartGpioCfg_t* gpio_cfg);

/**
 * @brief  Initialize the UART interface.
 * @param[in]  uart: The UART to initialize.
 * @param[in]  baudrate: The baudrate to use for the UART.
 * @param[in]  mode: The mode to use for the UART.
 * @param[in]  priority: The priority to use for the UART receive interrupt.
 * @return  None.
 */
void erdp_if_uart_init(ERDP_Uart_t uart, uint32_t baudrate, ERDP_UartMode_t mode,uint8_t priority);

/**
 * @brief Send multiple bytes via specified UART
 * @param[in] uart: UART port number to use
 * @param[in] data: Pointer to data buffer to send
 * @param[in] len: Length of data to send (in bytes)
 * @return None
 */
void erdp_if_uart_send_bytes(ERDP_Uart_t uart, const uint8_t* data, uint32_t len);

/**
 * @brief Read a single byte from specified UART
 * @param[in] uart: UART port number to read from
 * @param[out] data: Pointer to buffer for storing the received data
 * @return None
 */
void erdp_if_uart_read_byte(ERDP_Uart_t uart, uint8_t* data);

#ifdef __cplusplus
 }
 #endif // __cplusplus

#endif // ERDP_IF_UART_H
