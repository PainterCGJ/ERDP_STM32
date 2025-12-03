#ifndef __ERDP_HAL_GPIO_HPP__
#define __ERDP_HAL_GPIO_HPP__

#include "erdp_if_gpio.h"
namespace erdp
{
    class GpioDev
    {
    public:
        GpioDev() {}
        GpioDev(ERDP_GpioPort_t port, ERDP_GpioPin_t pin, ERDP_GpioPinMode_t mode,
                ERDP_GpioPinPull_t pull = ERDP_GPIO_PIN_PULL_NONE, ERDP_GpioSpeed_t speed = ERDP_GPIO_SPEED_HIGH)
            : __port(port), __pin(pin)
        {
            erdp_if_gpio_init(port, pin, mode, pull, speed);
        }
        ~GpioDev() {}

        void init(ERDP_GpioPort_t port, ERDP_GpioPin_t pin, ERDP_GpioPinMode_t mode,
                  ERDP_GpioPinPull_t pull = ERDP_GPIO_PIN_PULL_NONE, ERDP_GpioSpeed_t speed = ERDP_GPIO_SPEED_HIGH)
        {
            erdp_if_gpio_init(port, pin, mode, pull, speed);
            __port = port;
            __pin = pin;
        }

        void af_cfg(uint32_t af)
        {
            erdp_if_gpio_af_cfg(__port, __pin, af);
        }
        void write(ERDP_Status_t value)
        {
            erdp_if_gpio_write(__port, __pin, value);
        }
        ERDP_Status_t read()
        {
            return erdp_if_gpio_read(__port, __pin);
        }

    private:
        ERDP_GpioPort_t __port = ERDP_GPIOA;    // Default to GPIOA
        ERDP_GpioPin_t __pin = ERDP_GPIO_PIN_0; // Default to pin 0
    };
    
} // namespace erdp

#endif
