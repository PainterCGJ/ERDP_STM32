/* erdp include */
#include "erdp_if_exti.h"

/* platform include */
#include "stm32f4xx.h"

extern void erdp_exti_irq_handler(ERDP_GpioPin_t pin);

const static uint32_t exti_line[ERDP_GPIO_PIN_MAX] = {
    // (uint32_t)EXTI_0,
    // (uint32_t)EXTI_1,
    // (uint32_t)EXTI_2,
    // (uint32_t)EXTI_3,
    // (uint32_t)EXTI_4,
    // (uint32_t)EXTI_5,
    // (uint32_t)EXTI_6,
    // (uint32_t)EXTI_7,
    // (uint32_t)EXTI_8,
    // (uint32_t)EXTI_9,
    // (uint32_t)EXTI_10,
    // (uint32_t)EXTI_11,
    // (uint32_t)EXTI_12,
    // (uint32_t)EXTI_13,
    // (uint32_t)EXTI_14,
    // (uint32_t)EXTI_15,
};

const static uint8_t exti_source_port[ERDP_GPIO_MAX] = {
    // (uint8_t)EXTI_SOURCE_GPIOA, // port A
    // (uint8_t)EXTI_SOURCE_GPIOB, // port B
    // (uint8_t)EXTI_SOURCE_GPIOC, // port C
    // (uint8_t)EXTI_SOURCE_GPIOD, // port D
    // (uint8_t)EXTI_SOURCE_GPIOE, // port E
    // (uint8_t)EXTI_SOURCE_GPIOF, // port F
    // (uint8_t)EXTI_SOURCE_GPIOG, // port G
    // (uint8_t)EXTI_SOURCE_GPIOH, // port H
    // (uint8_t)EXTI_SOURCE_GPIOI, // port I
};

const static uint8_t exti_irq_id[ERDP_GPIO_PIN_MAX] = {
    // (uint8_t)EXTI0_IRQn,     // port 0
    // (uint8_t)EXTI1_IRQn,     // port 1
    // (uint8_t)EXTI2_IRQn,     // port 2
    // (uint8_t)EXTI3_IRQn,     // port 3
    // (uint8_t)EXTI4_IRQn,     // port 4
    // (uint8_t)EXTI5_9_IRQn,   // port 5
    // (uint8_t)EXTI5_9_IRQn,   // port 6
    // (uint8_t)EXTI5_9_IRQn,   // port 7
    // (uint8_t)EXTI5_9_IRQn,   // port 8
    // (uint8_t)EXTI5_9_IRQn,   // port 9
    // (uint8_t)EXTI10_15_IRQn, // port 10
    // (uint8_t)EXTI10_15_IRQn, // port 11
    // (uint8_t)EXTI10_15_IRQn, // port 12
    // (uint8_t)EXTI10_15_IRQn, // port 13
    // (uint8_t)EXTI10_15_IRQn, // port 14
    // (uint8_t)EXTI10_15_IRQn, // port 15

};

const static uint8_t exti_source_pin[ERDP_GPIO_PIN_MAX] = {
    // (uint8_t)EXTI_SOURCE_PIN0,  // pin 0
    // (uint8_t)EXTI_SOURCE_PIN1,  // pin 1
    // (uint8_t)EXTI_SOURCE_PIN2,  // pin 2
    // (uint8_t)EXTI_SOURCE_PIN3,  // pin 3
    // (uint8_t)EXTI_SOURCE_PIN4,  // pin 4
    // (uint8_t)EXTI_SOURCE_PIN5,  // pin 5
    // (uint8_t)EXTI_SOURCE_PIN6,  // pin 6
    // (uint8_t)EXTI_SOURCE_PIN7,  // pin 7
    // (uint8_t)EXTI_SOURCE_PIN8,  // pin 8
    // (uint8_t)EXTI_SOURCE_PIN9,  // pin 9
    // (uint8_t)EXTI_SOURCE_PIN10, // pin 10
    // (uint8_t)EXTI_SOURCE_PIN11, // pin 11
    // (uint8_t)EXTI_SOURCE_PIN12, // pin 12
    // (uint8_t)EXTI_SOURCE_PIN13, // pin 13
    // (uint8_t)EXTI_SOURCE_PIN14, // pin 14
    // (uint8_t)EXTI_SOURCE_PIN15, // pin 15
};

void erdp_if_exti_init(ERDP_GpioPort_t port, ERDP_GpioPin_t pin, ERDP_ExtiEdage_t edge, uint8_t priority)
{
    // uint32_t pull_up_down;
    // exti_trig_type_enum exti_trig;
    // uint32_t gpio_periph = erdp_if_gpio_get_port(port);
    // uint32_t gpio_pin = erdp_if_gpio_get_pin(pin);
    // switch (edge)
    // {
    // case ERDP_EXTI_RAISING_EDGE:
    // {
    //     pull_up_down = GPIO_PUPD_NONE;
    //     exti_trig = EXTI_TRIG_RISING;
    //     break;
    // }
    // case ERDP_EXTI_FALLING_EDGE:
    // {
    //     pull_up_down = GPIO_PUPD_PULLUP;
    //     exti_trig = EXTI_TRIG_FALLING;
    //     break;
    // }
    // case ERDP_EXTI_BOTH_EDGE:
    // {
    //     pull_up_down = GPIO_PUPD_PULLDOWN;
    //     exti_trig = EXTI_TRIG_BOTH;
    //     break;
    // }
    // default:
    // {
    //     pull_up_down = GPIO_PUPD_NONE;
    // }
    // }
    // rcu_periph_clock_enable(RCU_SYSCFG);
    // rcu_periph_clock_enable(erdp_if_gpio_get_PCLK(port));
    // gpio_mode_set(gpio_periph, GPIO_MODE_INPUT, pull_up_down, gpio_pin);
    // nvic_irq_enable(exti_irq_id[pin], priority, 0);
    // syscfg_exti_line_config(exti_source_port[port], exti_source_pin[pin]);
    // exti_init(exti_line[pin], EXTI_INTERRUPT, exti_trig);
    // exti_interrupt_flag_clear(exti_line[pin]);
}

void EXTI0_IRQHandler(void){
    // if(exti_interrupt_flag_get(EXTI_0) != RESET){
    //     erdp_exti_irq_handler(ERDP_GPIO_PIN_0);
    //     exti_interrupt_flag_clear(EXTI_0);
    // }
}
void EXTI1_IRQHandler(void){
    // if(exti_interrupt_flag_get(EXTI_1)!= RESET){
    //     erdp_exti_irq_handler(ERDP_GPIO_PIN_1);
    //     exti_interrupt_flag_clear(EXTI_1);
    // }
}
void EXTI2_IRQHandler(void){
    // if(exti_interrupt_flag_get(EXTI_2)!= RESET){
    //     erdp_exti_irq_handler(ERDP_GPIO_PIN_2);
    //     exti_interrupt_flag_clear(EXTI_2);
    // }
}
void EXTI3_IRQHandler(void){
    // if(exti_interrupt_flag_get(EXTI_3)!= RESET){
    //     erdp_exti_irq_handler(ERDP_GPIO_PIN_3);
    //     exti_interrupt_flag_clear(EXTI_3);
    // }
}
void EXTI4_IRQHandler(void){
    // if(exti_interrupt_flag_get(EXTI_4)!= RESET){
    //     erdp_exti_irq_handler(ERDP_GPIO_PIN_4);
    //     exti_interrupt_flag_clear(EXTI_4);
    // }
}
void EXTI5_9_IRQHandler(void){
    // if(exti_interrupt_flag_get(EXTI_5)!= RESET){
    //     erdp_exti_irq_handler(ERDP_GPIO_PIN_5);
    //     exti_interrupt_flag_clear(EXTI_5);
    // }
    // if(exti_interrupt_flag_get(EXTI_6)!= RESET){
    //     erdp_exti_irq_handler(ERDP_GPIO_PIN_6);
    //     exti_interrupt_flag_clear(EXTI_6);
    // }
    // if(exti_interrupt_flag_get(EXTI_7)!= RESET){
    //     erdp_exti_irq_handler(ERDP_GPIO_PIN_7);
    //     exti_interrupt_flag_clear(EXTI_7);
    // }
    // if(exti_interrupt_flag_get(EXTI_8)!= RESET){
    //     erdp_exti_irq_handler(ERDP_GPIO_PIN_8);
    //     exti_interrupt_flag_clear(EXTI_8);
    // }
    // if(exti_interrupt_flag_get(EXTI_9)!= RESET){
    //     erdp_exti_irq_handler(ERDP_GPIO_PIN_9);
    //     exti_interrupt_flag_clear(EXTI_9);
    // }
}
void EXTI10_15_IRQHandler(void){
    // if(exti_interrupt_flag_get(EXTI_10)!= RESET){
    //     erdp_exti_irq_handler(ERDP_GPIO_PIN_10);
    //     exti_interrupt_flag_clear(EXTI_10);
    // }
    // if(exti_interrupt_flag_get(EXTI_11)!= RESET){
    //     erdp_exti_irq_handler(ERDP_GPIO_PIN_11);
    //     exti_interrupt_flag_clear(EXTI_11);
    // }
    // if(exti_interrupt_flag_get(EXTI_12)!= RESET){
    //     erdp_exti_irq_handler(ERDP_GPIO_PIN_12);
    //     exti_interrupt_flag_clear(EXTI_12);
    // }
    // if(exti_interrupt_flag_get(EXTI_13)!= RESET){
    //     erdp_exti_irq_handler(ERDP_GPIO_PIN_13);
    //     exti_interrupt_flag_clear(EXTI_13);
    // }
    // if(exti_interrupt_flag_get(EXTI_14)!= RESET){
    //     erdp_exti_irq_handler(ERDP_GPIO_PIN_14);
    //     exti_interrupt_flag_clear(EXTI_14);
    // }
    // if(exti_interrupt_flag_get(EXTI_15)!= RESET){
    //     erdp_exti_irq_handler(ERDP_GPIO_PIN_15);
    //     exti_interrupt_flag_clear(EXTI_15);
    // }
}