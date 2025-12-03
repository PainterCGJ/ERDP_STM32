/*
//D:\Users\painter\AppData\Local\Keil_v5\ARM\ARMCLANG\bin\fromelf.exe --bin --output ./Build/erdp.bin ./Build/erdp.axf
*/
#include "erdp_hal_uart.hpp"
#include "erdp_hal_gpio.hpp"
#include "erdp_hal_spi.hpp"
#include "erdp_osal.hpp"
#include "erdp_assert.h"
#include "erdp_hal_exti.hpp"
// #include "printf.h"
#include <vector>
#include <functional>
#include "log_adapter.hpp"
#include "log_adapter.hpp"

using namespace erdp;
using namespace std;
#define SYS_LED_PORT ERDP_GPIOC
#define SYS_LED_PIN ERDP_GPIO_PIN_0


class LED : private GpioDev {
   public:
    LED(ERDP_GpioPort_t port, ERDP_GpioPin_t pin,
        ERDP_Status_t on_level = ERDP_SET)
        : GpioDev(port, pin, ERDP_GPIO_PIN_MODE_OUTPUT, ERDP_GPIO_PIN_PULL_NONE,
                  ERDP_GPIO_SPEED_LOW),
          __on_level(on_level) {}

    void on() {
        write((ERDP_Status_t)__on_level);
        __status = (ERDP_Status_t)__on_level;
    }
    void off() {
        write((ERDP_Status_t)!__on_level);
        __status = (ERDP_Status_t)!__on_level;
    }

    void toggle() {
        __status = (ERDP_Status_t)!__status;
        write(__status);
    }

   private:
    ERDP_Status_t __on_level;
    ERDP_Status_t __status;
};

void Thread::main_thread(void *parm)
{

    LED sys_led(SYS_LED_PORT, SYS_LED_PIN, ERDP_RESET);
    // lambda 表达式可以隐式转换为 std::function<void()>
    // 注意：静态检查器可能无法正确推断 lambda 类型，但实际编译器可以正常编译
    // Thread 构造函数接受 std::function<void()> 类型（定义在 erdp_osal.hpp 第118行）
	erdp::Thread LED_thread(
        [&sys_led]() {
            while (1) {
                sys_led.toggle();    // Toggle the system LED to indicate the
                                     // system is
                Thread::delay_ms(1000);
            }
        },
        "LED", 6, 128);
    LED_thread.join();

    while (1)
    {
        Thread::delay_ms(10);
    }
}
