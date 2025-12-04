#include "erdp_if_uart.h"
#include "stm32f4xx_usart.h"

extern void erdp_uart_irq_handler(ERDP_Uart_t uart);

const static uint32_t uart_instance[ERDP_UART_NUM] = {
    0, (uint32_t)USART1, (uint32_t)USART2, (uint32_t)USART3, (uint32_t)UART4, (uint32_t)UART5, (uint32_t)USART6,
};

const static uint32_t uart_pclk[ERDP_UART_NUM] = {
    0,
    RCC_APB2Periph_USART1,
    RCC_APB1Periph_USART2,
    RCC_APB1Periph_USART3,
    RCC_APB1Periph_UART4,
    RCC_APB1Periph_UART5,
    RCC_APB2Periph_USART6,
};

typedef void (*rcc_clock_cmd_func_t)(uint32_t RCC_AHBxPeriph, FunctionalState NewState);
const static rcc_clock_cmd_func_t rcc_clock_cmd_func[ERDP_UART_NUM] = {
    NULL,
    RCC_APB2PeriphClockCmd,
    RCC_APB1PeriphClockCmd,
    RCC_APB1PeriphClockCmd,
    RCC_APB1PeriphClockCmd,
    RCC_APB1PeriphClockCmd,
    RCC_APB2PeriphClockCmd,
};
const static uint32_t uart_irq[ERDP_UART_NUM] = {
    0, USART1_IRQn, USART2_IRQn, USART3_IRQn, UART4_IRQn, UART5_IRQn, USART6_IRQn,

};

uint32_t erdp_if_uart_get_base(ERDP_Uart_t uart) { return uart_instance[uart]; }

uint32_t erdp_if_uart_get_PCLK(ERDP_Uart_t uart) { return uart_pclk[uart]; }

uint32_t erdp_if_uart_get_irq(ERDP_Uart_t uart) { return uart_irq[uart]; }

void erdp_if_uart_gpio_init(ERDP_UartGpioCfg_t *gpio_cfg) {
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_TypeDef *tx_port = (GPIO_TypeDef *)erdp_if_gpio_get_port(gpio_cfg->tx_port);
    uint32_t tx_pin = erdp_if_gpio_get_pin(gpio_cfg->tx_pin);
    GPIO_TypeDef *rx_port = (GPIO_TypeDef *)erdp_if_gpio_get_port(gpio_cfg->rx_port);
    uint32_t rx_pin = erdp_if_gpio_get_pin(gpio_cfg->rx_pin);

    RCC_AHB1PeriphClockCmd(erdp_if_gpio_get_PCLK(gpio_cfg->tx_port), ENABLE);
    RCC_AHB1PeriphClockCmd(erdp_if_gpio_get_PCLK(gpio_cfg->rx_port), ENABLE);

    GPIO_PinAFConfig(tx_port, gpio_cfg->tx_pin, gpio_cfg->tx_af);
    GPIO_PinAFConfig(rx_port, gpio_cfg->rx_pin, gpio_cfg->rx_af);

    GPIO_InitStructure.GPIO_Pin = tx_pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(tx_port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = rx_pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(rx_port, &GPIO_InitStructure);
}

void erdp_if_uart_init(ERDP_Uart_t uart, uint32_t baudrate, ERDP_UartMode_t mode, uint8_t priority) {
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    uint32_t pclk = erdp_if_uart_get_PCLK(uart);
    // erdp_assert(uart > ERDP_UART0 && uart < ERDP_UART_NUM);

    rcc_clock_cmd_func[uart](pclk, ENABLE);
    switch (mode) {
        case ERDP_UART_TX_RX:
            USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    // 收发模式
            break;
        case ERDP_UART_TX_ONLY:
            USART_InitStructure.USART_Mode = USART_Mode_Tx;    // 收发模式
            break;
        case ERDP_UART_RX_ONLY:
            USART_InitStructure.USART_Mode = USART_Mode_Rx;    // 收发模式
            break;
        default:
            break;
    }

    USART_InitStructure.USART_BaudRate = baudrate;                                     // 波特率设置
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                        // 字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                             // 一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                                // 无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;    // 无硬件数据流控制
    USART_Init((USART_TypeDef *)erdp_if_uart_get_base(uart), &USART_InitStructure);    // 初始化串口


    NVIC_InitStructure.NVIC_IRQChannel = erdp_if_uart_get_irq(uart);
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = priority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    USART_Cmd((USART_TypeDef *)erdp_if_uart_get_base(uart), ENABLE);    // 使能串口
    USART_ITConfig((USART_TypeDef *)erdp_if_uart_get_base(uart), USART_IT_RXNE, ENABLE);    // 开启相关中断

}

void erdp_if_uart_send_bytes(ERDP_Uart_t uart, const uint8_t *data, uint32_t len) {
    for (uint32_t i = 0; i < len; i++) {
        while (USART_GetFlagStatus((USART_TypeDef *)uart_instance[uart], USART_FLAG_TXE) == RESET) {
            ;    // Wait for transmission complete
        }
        USART_SendData((USART_TypeDef *)uart_instance[uart], data[i]);    // 发送数据
    }
}

void erdp_if_uart_read_byte(ERDP_Uart_t uart, uint8_t *data) {
    while (USART_GetFlagStatus((USART_TypeDef *)uart_instance[uart], USART_FLAG_RXNE) == RESET) {
        ;    // Wait for data to be received
    }
    *data = USART_ReceiveData((USART_TypeDef *)uart_instance[uart]);    // Read the received data
}

void USART1_IRQHandler(void) { erdp_uart_irq_handler(ERDP_UART1); }
void USART2_IRQHandler(void) { erdp_uart_irq_handler(ERDP_UART2); }
void USART3_IRQHandler(void) { erdp_uart_irq_handler(ERDP_UART3); }
void UART4_IRQHandler(void) { erdp_uart_irq_handler(ERDP_UART4); }
void UART5_IRQHandler(void) { erdp_uart_irq_handler(ERDP_UART5); }
void USART6_IRQHandler(void) { erdp_uart_irq_handler(ERDP_UART6); }
