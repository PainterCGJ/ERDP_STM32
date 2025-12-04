#include "erdp_hal_exti.hpp"
namespace erdp
{
    Exti *Exti::__exti_instance[ERDP_GPIO_PIN_MAX] = {nullptr};

} // namespace erdp

extern "C" void erdp_exti_irq_handler(ERDP_GpioPin_t pin)
{
    erdp::Exti::__exti_instance[pin]->__irq_handler();
}