#ifndef __ERDP_IF_GPIO_H__
#define __ERDP_IF_GPIO_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>
#include "erdp_interface.h"

    typedef enum
    {
        ERDP_GPIOA,
        ERDP_GPIOB,
        ERDP_GPIOC,
        ERDP_GPIOD,
        ERDP_GPIOE,
        ERDP_GPIOF,
        ERDP_GPIOG,
        ERDP_GPIOH,
        ERDP_GPIOI,
        
        ERDP_GPIO_MAX,
    } ERDP_GpioPort_t;

    typedef enum
    {
        ERDP_GPIO_PIN_0,
        ERDP_GPIO_PIN_1,
        ERDP_GPIO_PIN_2,
        ERDP_GPIO_PIN_3,
        ERDP_GPIO_PIN_4,
        ERDP_GPIO_PIN_5,
        ERDP_GPIO_PIN_6,
        ERDP_GPIO_PIN_7,
        ERDP_GPIO_PIN_8,
        ERDP_GPIO_PIN_9,
        ERDP_GPIO_PIN_10,
        ERDP_GPIO_PIN_11,
        ERDP_GPIO_PIN_12,
        ERDP_GPIO_PIN_13,
        ERDP_GPIO_PIN_14,
        ERDP_GPIO_PIN_15,

        ERDP_GPIO_PIN_MAX,
    } ERDP_GpioPin_t;

    typedef enum
    {
        ERDP_GPIO_PIN_MODE_INPUT,
        ERDP_GPIO_PIN_MODE_OUTPUT,
        ERDP_GPIO_PIN_MODE_ALTERNATE,
        ERDP_GPIO_PIN_MODE_ANALOG,
    } ERDP_GpioPinMode_t;

    typedef enum
    {
        ERDP_GPIO_PIN_PULL_NONE,
        ERDP_GPIO_PIN_PULLUP,
        ERDP_GPIO_PIN_PULLDOWN,
    } ERDP_GpioPinPull_t;

    typedef enum
    {
        ERDP_GPIO_SPEED_LOW,
        ERDP_GPIO_SPEED_MEDIUM,
        ERDP_GPIO_SPEED_HIGH,
        ERDP_GPIO_SPEED_MAX
    } ERDP_GpioSpeed_t;

    /** 
    * @brief  Get the SOC GPIO port base address for a given ERDP port.
    * @param[in]  port: The ERDP GPIO port to get the SOC GPIO base address for.
    * @return  The base address of the SOC GPIO port.
    */
    uint32_t erdp_if_gpio_get_port(ERDP_GpioPort_t port);
    
    /**
     * @brief  Get the SOC GPIO pin number for a given ERDP pin.
     * @param[in]  pin: The ERDP GPIO pin to get the SOC GPIO pin number for.
     * @return  The SOC GPIO pin number.
     */
    uint32_t erdp_if_gpio_get_pin(ERDP_GpioPin_t pin);

    /**
     * @brief  Get the SOC GPIO peripheral clock for a given ERDP port.
     * @param[in]  port: The ERDP GPIO port to get the SOC GPIO peripheral clock for.
     * @return  The SOC GPIO peripheral clock.
     * */
    uint32_t erdp_if_gpio_get_PCLK(ERDP_GpioPort_t port);

    /**
     * @brief  Initialize a GPIO pin with the given mode, pull, and speed.
     * @param[in]  port: The ERDP GPIO port to initialize.
     * @param[in]  pin: The ERDP GPIO pin to initialize.
     * @param[in]  mode: The mode to set the GPIO pin to.
     * @param[in]  pull: The pull-up/down configuration to set the GPIO pin to.
     * @param[in]  speed: The speed to set the GPIO pin to.
     * @return  None.
     */
    void erdp_if_gpio_init(ERDP_GpioPort_t port, ERDP_GpioPin_t pin, ERDP_GpioPinMode_t mode,
                           ERDP_GpioPinPull_t pull, ERDP_GpioSpeed_t speed);

    void erdp_if_gpio_af_cfg(ERDP_GpioPort_t port, ERDP_GpioPin_t pin, uint32_t af);

    /**
     * @brief  Write a value to a GPIO pin.
     * @param[in]  port: The ERDP GPIO port to write to.
     * @param[in]  pin: The ERDP GPIO pin to write to.
     * @param[in]  value: The value to write to the GPIO pin.
     * @return  None.
     */
    void erdp_if_gpio_write(ERDP_GpioPort_t port, ERDP_GpioPin_t pin, ERDP_Status_t value);

    /**
     * @brief  Read the value of a GPIO pin.
     * @param[in]  port: The ERDP GPIO port to read from.
     * @param[in]  pin: The ERDP GPIO pin to read from.
     * @return  The value of the GPIO pin.
     */
    ERDP_Status_t erdp_if_gpio_read(ERDP_GpioPort_t port, ERDP_GpioPin_t pin);

#ifdef __cplusplus
}
#endif

#endif
