/* erdp include */
#include "erdp_if_exti.h"

/* platform include */
#include "stm32f4xx.h"
#include "stm32f4xx_exti.h"

extern void erdp_exti_irq_handler(ERDP_GpioPin_t pin);

const static uint32_t exti_line[ERDP_GPIO_PIN_MAX] = {
    EXTI_Line0, EXTI_Line1, EXTI_Line2,  EXTI_Line3,  EXTI_Line4,  EXTI_Line5,  EXTI_Line6,  EXTI_Line7,
    EXTI_Line8, EXTI_Line9, EXTI_Line10, EXTI_Line11, EXTI_Line12, EXTI_Line13, EXTI_Line14, EXTI_Line15,
};

const static uint8_t exti_source_port[ERDP_GPIO_MAX] = {
    EXTI_PortSourceGPIOA,    // port A
    EXTI_PortSourceGPIOB,    // port B
    EXTI_PortSourceGPIOC,    // port C
    EXTI_PortSourceGPIOD,    // port D
    EXTI_PortSourceGPIOE,    // port E
    EXTI_PortSourceGPIOF,    // port F
    EXTI_PortSourceGPIOG,    // port G
    EXTI_PortSourceGPIOH,    // port H
    EXTI_PortSourceGPIOI,    // port I
};

const static uint8_t exti_irq_id[ERDP_GPIO_PIN_MAX] = {
    EXTI0_IRQn,     EXTI1_IRQn,     EXTI2_IRQn,     EXTI3_IRQn,     EXTI4_IRQn,     EXTI9_5_IRQn,
    EXTI9_5_IRQn,   EXTI9_5_IRQn,   EXTI9_5_IRQn,   EXTI9_5_IRQn,   EXTI15_10_IRQn, EXTI15_10_IRQn,
    EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn,
};

const static uint8_t exti_source_pin[ERDP_GPIO_PIN_MAX] = {
    EXTI_PinSource0,  EXTI_PinSource1,  EXTI_PinSource2,  EXTI_PinSource3,  EXTI_PinSource4,  EXTI_PinSource5,
    EXTI_PinSource6,  EXTI_PinSource7,  EXTI_PinSource8,  EXTI_PinSource9,  EXTI_PinSource10, EXTI_PinSource11,
    EXTI_PinSource12, EXTI_PinSource13, EXTI_PinSource14, EXTI_PinSource15,
};

void erdp_if_exti_init(ERDP_GpioPort_t port, ERDP_GpioPin_t pin, ERDP_ExtiEdage_t edge, uint8_t priority) {
    GPIOPuPd_TypeDef pull_up_down;
    EXTITrigger_TypeDef exti_trig;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    GPIO_TypeDef *gpio_periph = (GPIO_TypeDef *)erdp_if_gpio_get_port(port);
    uint16_t gpio_pin = erdp_if_gpio_get_pin(pin);
    switch (edge) {
        case ERDP_EXTI_RAISING_EDGE: {
            pull_up_down = GPIO_PuPd_DOWN;
            exti_trig = EXTI_Trigger_Rising;
            break;
        }
        case ERDP_EXTI_FALLING_EDGE: {
            pull_up_down = GPIO_PuPd_UP;
            exti_trig = EXTI_Trigger_Falling;
            break;
        }
        case ERDP_EXTI_BOTH_EDGE: {
            pull_up_down = GPIO_PuPd_NOPULL;
            exti_trig = EXTI_Trigger_Rising_Falling;
            break;
        }
        default: {
            pull_up_down = GPIO_PuPd_NOPULL;
        }
    }

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_EXTILineConfig(exti_source_port[port], exti_source_pin[pin]);

    GPIO_InitStructure.GPIO_Pin = gpio_pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = pull_up_down;
    GPIO_Init(gpio_periph, &GPIO_InitStructure);

    EXTI_InitStructure.EXTI_Line = exti_line[pin];
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = exti_trig;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = exti_irq_id[pin];
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = priority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    EXTI_ClearITPendingBit(exti_line[pin]);
}

void EXTI0_IRQHandler(void) {
    if(EXTI_GetITStatus(EXTI_Line0) == SET){
        erdp_exti_irq_handler(ERDP_GPIO_PIN_0);
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}
void EXTI1_IRQHandler(void) {
    if(EXTI_GetITStatus(EXTI_Line1) == SET){
        erdp_exti_irq_handler(ERDP_GPIO_PIN_1);
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}
void EXTI2_IRQHandler(void) {
    if(EXTI_GetITStatus(EXTI_Line2) == SET){
        erdp_exti_irq_handler(ERDP_GPIO_PIN_2);
        EXTI_ClearITPendingBit(EXTI_Line2);
    }
}
void EXTI3_IRQHandler(void) {
    if(EXTI_GetITStatus(EXTI_Line3) == SET){
        erdp_exti_irq_handler(ERDP_GPIO_PIN_3);
        EXTI_ClearITPendingBit(EXTI_Line3);
    }
}
void EXTI4_IRQHandler(void) {
    if(EXTI_GetITStatus(EXTI_Line4) == SET){
        erdp_exti_irq_handler(ERDP_GPIO_PIN_4);
        EXTI_ClearITPendingBit(EXTI_Line4);
    }
}
void EXTI5_9_IRQHandler(void) {
    if(EXTI_GetITStatus(EXTI_Line5) == SET){
        erdp_exti_irq_handler(ERDP_GPIO_PIN_5);
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
    if(EXTI_GetITStatus(EXTI_Line6) == SET){
        erdp_exti_irq_handler(ERDP_GPIO_PIN_6);
        EXTI_ClearITPendingBit(EXTI_Line6);
    }
    if(EXTI_GetITStatus(EXTI_Line7) == SET){
        erdp_exti_irq_handler(ERDP_GPIO_PIN_7);
        EXTI_ClearITPendingBit(EXTI_Line7);
    }
    if(EXTI_GetITStatus(EXTI_Line8) == SET){
        erdp_exti_irq_handler(ERDP_GPIO_PIN_8);
        EXTI_ClearITPendingBit(EXTI_Line8);
    }
    if(EXTI_GetITStatus(EXTI_Line9) == SET){
        erdp_exti_irq_handler(ERDP_GPIO_PIN_9);
        EXTI_ClearITPendingBit(EXTI_Line9);
    }
}
void EXTI10_15_IRQHandler(void) {
    if(EXTI_GetITStatus(EXTI_Line10) == SET){
        erdp_exti_irq_handler(ERDP_GPIO_PIN_10);
        EXTI_ClearITPendingBit(EXTI_Line10);
    }
    if(EXTI_GetITStatus(EXTI_Line11) == SET){
        erdp_exti_irq_handler(ERDP_GPIO_PIN_11);
        EXTI_ClearITPendingBit(EXTI_Line11);
    }
    if(EXTI_GetITStatus(EXTI_Line12) == SET){
        erdp_exti_irq_handler(ERDP_GPIO_PIN_12);
        EXTI_ClearITPendingBit(EXTI_Line12);
    }
    if(EXTI_GetITStatus(EXTI_Line13) == SET){
        erdp_exti_irq_handler(ERDP_GPIO_PIN_13);
        EXTI_ClearITPendingBit(EXTI_Line13);
    }
    if(EXTI_GetITStatus(EXTI_Line14) == SET){
        erdp_exti_irq_handler(ERDP_GPIO_PIN_14);
        EXTI_ClearITPendingBit(EXTI_Line14);
    }
    if(EXTI_GetITStatus(EXTI_Line15) == SET){
        erdp_exti_irq_handler(ERDP_GPIO_PIN_15);
        EXTI_ClearITPendingBit(EXTI_Line15);
    }
}