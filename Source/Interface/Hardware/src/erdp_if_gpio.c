
/* erdp include */
#include "erdp_if_gpio.h"

/* platform include */
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

const static uint32_t port_instance[ERDP_GPIO_MAX] = {
    (uint32_t)GPIOA, (uint32_t)GPIOB, (uint32_t)GPIOC, (uint32_t)GPIOD, (uint32_t)GPIOE,
    (uint32_t)GPIOF, (uint32_t)GPIOG, (uint32_t)GPIOH, (uint32_t)GPIOI,
};

const static uint32_t pin_instance[ERDP_GPIO_PIN_MAX] = {
    (uint32_t)GPIO_Pin_0,  (uint32_t)GPIO_Pin_1,  (uint32_t)GPIO_Pin_2,  (uint32_t)GPIO_Pin_3,
    (uint32_t)GPIO_Pin_4,  (uint32_t)GPIO_Pin_5,  (uint32_t)GPIO_Pin_6,  (uint32_t)GPIO_Pin_7,
    (uint32_t)GPIO_Pin_8,  (uint32_t)GPIO_Pin_9,  (uint32_t)GPIO_Pin_10, (uint32_t)GPIO_Pin_11,
    (uint32_t)GPIO_Pin_12, (uint32_t)GPIO_Pin_13, (uint32_t)GPIO_Pin_14, (uint32_t)GPIO_Pin_15,
};

const static uint32_t gpio_pclk[ERDP_GPIO_MAX] = {
    RCC_AHB1Periph_GPIOA,    // GPIOA,
    RCC_AHB1Periph_GPIOB,    // GPIOB,
    RCC_AHB1Periph_GPIOC,    // GPIOC,
    RCC_AHB1Periph_GPIOD,    // GPIOD,
    RCC_AHB1Periph_GPIOE,    // GPIOE,
    RCC_AHB1Periph_GPIOF,    // GPIOF,
    RCC_AHB1Periph_GPIOG,    // GPIOG,
    RCC_AHB1Periph_GPIOH,    // GPIOH,
    RCC_AHB1Periph_GPIOI,    // GPIOI,
};

uint32_t erdp_if_gpio_get_port(ERDP_GpioPort_t port) { return port_instance[port]; }

uint32_t erdp_if_gpio_get_pin(ERDP_GpioPin_t pin) { return pin_instance[pin]; }

uint32_t erdp_if_gpio_get_PCLK(ERDP_GpioPort_t port) { return gpio_pclk[port]; }

void erdp_if_gpio_init(ERDP_GpioPort_t port, ERDP_GpioPin_t pin, ERDP_GpioPinMode_t mode, ERDP_GpioPinPull_t pull,
                       ERDP_GpioSpeed_t speed) {
    // Initialize the GPIO pin
    uint32_t gpio_periph = erdp_if_gpio_get_port(port);
    uint32_t gpio_pin = erdp_if_gpio_get_pin(pin);
    GPIOMode_TypeDef gpio_mode = GPIO_Mode_IN;
    GPIOPuPd_TypeDef gpio_pull = GPIO_PuPd_NOPULL;
    GPIOSpeed_TypeDef gpio_speed = GPIO_Low_Speed;

    switch (mode) {
        case ERDP_GPIO_PIN_MODE_INPUT:
            gpio_mode = GPIO_Mode_IN;
            break;
        case ERDP_GPIO_PIN_MODE_OUTPUT:
            gpio_mode = GPIO_Mode_OUT;
            break;
        case ERDP_GPIO_PIN_MODE_ALTERNATE:
            gpio_mode = GPIO_Mode_AF;
            break;
        case ERDP_GPIO_PIN_MODE_ANALOG:
            gpio_mode = GPIO_Mode_AN;
            break;
        default:
            break;
    }

    switch (pull) {
        case ERDP_GPIO_PIN_PULL_NONE:
            gpio_pull = GPIO_PuPd_NOPULL;
            break;
        case ERDP_GPIO_PIN_PULLUP:
            gpio_pull = GPIO_PuPd_UP;
            break;
        case ERDP_GPIO_PIN_PULLDOWN:
            gpio_pull = GPIO_PuPd_DOWN;
            break;
        default:
            gpio_pull = GPIO_PuPd_NOPULL;
            break;
    }

    switch (speed) {
        case ERDP_GPIO_SPEED_LOW:
            gpio_speed = GPIO_Low_Speed;
            break;
        case ERDP_GPIO_SPEED_MEDIUM:
            gpio_speed = GPIO_Medium_Speed;
            break;
        case ERDP_GPIO_SPEED_HIGH:
            gpio_speed = GPIO_High_Speed;
            break;
        case ERDP_GPIO_SPEED_MAX:
            gpio_speed = GPIO_High_Speed;
            break;
    }

    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(gpio_pclk[port], ENABLE);
    GPIO_InitStructure.GPIO_Pin = gpio_pin;
    GPIO_InitStructure.GPIO_Mode = gpio_mode;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = gpio_speed;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init((GPIO_TypeDef*)gpio_periph, &GPIO_InitStructure);
}

void erdp_if_gpio_af_cfg(ERDP_GpioPort_t port, ERDP_GpioPin_t pin, uint32_t af) {
    // Set the alternate function for the GPIO pin
    GPIO_PinAFConfig((GPIO_TypeDef*)port_instance[port], pin_instance[pin], (uint8_t)af);
}

void erdp_if_gpio_write(ERDP_GpioPort_t port, ERDP_GpioPin_t pin, ERDP_Status_t value) {
    if (value == ERDP_SET) {
        GPIO_SetBits((GPIO_TypeDef*)port_instance[port], pin_instance[pin]);    // Set the GPIO pin
    } else {
        GPIO_ResetBits((GPIO_TypeDef*)port_instance[port], pin_instance[pin]);    // Reset the GPIO pin
    }
}

ERDP_Status_t erdp_if_gpio_read(ERDP_GpioPort_t port, ERDP_GpioPin_t pin) {
    if (GPIO_ReadInputDataBit((GPIO_TypeDef*)port_instance[port], pin_instance[pin]) == SET) {
        return ERDP_SET;    // Pin is set
    }
    return ERDP_RESET;
}
