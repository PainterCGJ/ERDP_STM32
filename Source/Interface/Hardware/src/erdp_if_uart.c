#include "erdp_if_uart.h"
#include "stm32f4xx_usart.h"

extern void erdp_uart_irq_handler(ERDP_Uart_t uart);

const static uint32_t uart_instance[ERDP_UART_NUM]={
    // (uint32_t)USART0,
    // (uint32_t)USART1,
    // (uint32_t)USART2,
    // (uint32_t)UART3,
    // (uint32_t)UART4,
    // (uint32_t)USART5,
    // (uint32_t)UART6,
    // (uint32_t)UART7
};

const static uint32_t uart_pclk[ERDP_UART_NUM]={
    // (uint32_t)USART0,
    // (uint32_t)USART1,
    // (uint32_t)USART2,
    // (uint32_t)UART3,
    // (uint32_t)UART4,
    // (uint32_t)USART5,
    // (uint32_t)UART6,
    // (uint32_t)UART7
};

const static uint32_t uart_irq[ERDP_UART_NUM]={
    // (uint32_t)USART0,
    // (uint32_t)USART1,
    // (uint32_t)USART2,
    // (uint32_t)UART3,
    // (uint32_t)UART4,
    // (uint32_t)USART5,
    // (uint32_t)UART6,
    // (uint32_t)UART7
};

uint32_t erdp_if_uart_get_base(ERDP_Uart_t uart)
{
    return uart_instance[uart];
}

uint32_t erdp_if_uart_get_PCLK(ERDP_Uart_t uart)
{
    return uart_pclk[uart];
}

uint32_t erdp_if_uart_get_irq(ERDP_Uart_t uart)
{
    return uart_irq[uart];
}

void erdp_if_uart_gpio_init(ERDP_UartGpioCfg_t *gpio_cfg)
{
    // uint32_t tx_base = erdp_if_gpio_get_port(gpio_cfg->tx_port);
    // uint32_t tx_pin = erdp_if_gpio_get_pin(gpio_cfg->tx_pin);
    // uint32_t tx_af = gpio_cfg->tx_af;
    // uint32_t rx_base = erdp_if_gpio_get_port(gpio_cfg->rx_port);
    // uint32_t rx_pin = erdp_if_gpio_get_pin(gpio_cfg->rx_pin);
    // uint32_t rx_af = gpio_cfg->rx_af;

    // rcu_periph_clock_enable(erdp_if_gpio_get_PCLK(gpio_cfg->tx_port));          // Enable the GPIO port clock for TX
    // rcu_periph_clock_enable(erdp_if_gpio_get_PCLK(gpio_cfg->rx_port));          // Enable the GPIO port clock for RX
    // gpio_af_set(tx_base, tx_af, tx_pin);                                        // Set the alternate function for TX pin
    // gpio_af_set(rx_base, rx_af, rx_pin);                                        // Set the alternate function for RX pin
    // gpio_mode_set(tx_base, GPIO_MODE_AF, GPIO_PUPD_PULLUP, tx_pin);             // Set the mode for TX pin to alternate function
    // gpio_mode_set(rx_base, GPIO_MODE_AF, GPIO_PUPD_PULLUP, rx_pin);             // Set the mode for RX pin to alternate function
    // gpio_output_options_set(tx_base, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, tx_pin); // Set the output options for TX pin
    // gpio_output_options_set(rx_base, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, rx_pin); // Set the output options for RX pin
}

void erdp_if_uart_init(ERDP_Uart_t uart, uint32_t baudrate, ERDP_UartMode_t mode, uint8_t priority)
{

    // // 1. initialize the UART peripheral
    // uint32_t uart_base = uart_instance[uart];
    // uint32_t pclk = erdp_if_uart_get_PCLK(uart);
    // usart_deinit(uart_base);
    // usart_flag_clear(uart_base, USART_FLAG_RBNE | USART_FLAG_TBE | USART_FLAG_TC | USART_FLAG_IDLE | USART_FLAG_ORERR | USART_FLAG_PERR | USART_FLAG_FERR);
    // rcu_periph_clock_enable(pclk);

    // usart_baudrate_set(uart_base, baudrate);

    // switch (mode)
    // {
    // case ERDP_UART_TX_RX:
    //     usart_transmit_config(uart_base, USART_TRANSMIT_ENABLE);
    //     usart_receive_config(uart_base, USART_RECEIVE_ENABLE);
    //     break;
    // case ERDP_UART_TX_ONLY:
    //     usart_transmit_config(uart_base, USART_TRANSMIT_ENABLE);
    //     usart_receive_config(uart_base, USART_RECEIVE_DISABLE);
    //     break;
    // case ERDP_UART_RX_ONLY:
    //     usart_transmit_config(uart_base, USART_TRANSMIT_DISABLE);
    //     usart_receive_config(uart_base, USART_RECEIVE_ENABLE);
    //     break;
    // default:
    //     break;
    // }

    // // 2. configure the nvic for the UART
    // nvic_irq_enable(erdp_if_uart_get_irq(uart), priority, 0); // Enable the NVIC for the UART interrupt
    // usart_interrupt_enable(uart_base, USART_INT_RBNE);        // Enable the IDLE interrupt

    // // 3. enable the UART peripheral
    // usart_enable(uart_base); // Enable the UART
}

ERDP_Status_t erdp_if_uart_int_flag_get(ERDP_Uart_t uart, ERDP_UartIrqFlag_t flag)
{
    // uint32_t uart_base = uart_instance[uart];
    // FlagStatus gd_flag_status;
    // usart_interrupt_flag_enum gd_usart_flag;
    // switch (flag)
    // {
    // case ERDP_UART_INT_FLAG_TBE:
    // {
    //     gd_usart_flag = USART_INT_FLAG_TBE;
    //     break;
    // }
    // case ERDP_UART_INT_FLAG_TC:
    // {
    //     gd_usart_flag = USART_INT_FLAG_TC;
    //     break;
    // }
    // case ERDP_UART_INT_FLAG_RBNE:
    // {
    //     gd_usart_flag = USART_INT_FLAG_RBNE;
    //     break;
    // }
    // case ERDP_UART_INT_FLAG_IDLE:
    // {
    //     gd_usart_flag = USART_INT_FLAG_IDLE;
    //     break;
    // }
    // default:
    //     return ERDP_RESET;
    // }
    // gd_flag_status = usart_interrupt_flag_get(uart_base, gd_usart_flag);
    // if (gd_flag_status == SET)
    // {
    //     return ERDP_SET;
    // }
    return ERDP_RESET;
}

void erdp_if_uart_send_bytes(ERDP_Uart_t uart, const uint8_t *data, uint32_t len)
{
    // uint32_t uart_base = uart_instance[uart];
    // for (uint32_t i = 0; i < len; i++)
    // {
    //     usart_data_transmit(uart_base, data[i]);
    //     while (usart_flag_get(uart_base, USART_FLAG_TBE) == RESET)
    //         ; // Wait for transmission complete
    // }
}

void erdp_if_uart_read_byte(ERDP_Uart_t uart, uint8_t *data)
{
    // while (usart_flag_get(uart_instance[uart], USART_FLAG_RBNE) == RESET)
    //     ; // Wait for data to be received
    // *data = usart_data_receive(uart_instance[uart]); // Read the received data
}

void USART0_IRQHandler(void)
{
    erdp_uart_irq_handler(ERDP_UART0);
}
void USART1_IRQHandler(void)
{
    erdp_uart_irq_handler(ERDP_UART1);
}
void USART2_IRQHandler(void)
{
    erdp_uart_irq_handler(ERDP_UART2);
}
void UART3_IRQHandler(void)
{
    erdp_uart_irq_handler(ERDP_UART3);
}
void UART4_IRQHandler(void)
{
    erdp_uart_irq_handler(ERDP_UART4);
}
void USART5_IRQHandler(void)
{
    erdp_uart_irq_handler(ERDP_UART5);
}
void UART6_IRQHandler(void)
{
    erdp_uart_irq_handler(ERDP_UART6);
}
void UART7_IRQHandler(void)
{
    erdp_uart_irq_handler(ERDP_UART7);
}
