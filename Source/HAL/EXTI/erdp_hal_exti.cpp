#include "erdp_hal_exti.hpp"

namespace erdp
{
    Exti *Exti::__exti_instance[ERDP_GPIO_PIN_MAX] = {nullptr};

    // Friend function implementation
    void erdp_exti_irq_handler_internal(ERDP_GpioPin_t pin)
    {
        Exti::__exti_instance[pin]->__irq_handler();
    }

} // namespace erdp

// C interface
extern "C" void erdp_exti_irq_handler(ERDP_GpioPin_t pin)
{
    erdp::erdp_exti_irq_handler_internal(pin);
}