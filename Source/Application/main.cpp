/*
//D:\Users\painter\AppData\Local\Keil_v5\ARM\ARMCLANG\bin\fromelf.exe --bin --output ./Build/erdp.bin ./Build/erdp.axf
*/
#include <stdio.h>

#include <vector>

#include "board.hpp"
#include "erdp_assert.h"
#include "erdp_hal_exti.hpp"
#include "erdp_hal_gpio.hpp"
#include "erdp_hal_spi.hpp"
#include "erdp_hal_uart.hpp"
#include "erdp_osal.hpp"
#include "log_adapter.hpp"
#include "oled_iic.h"

using namespace erdp;
using namespace std;
#define SYS_LED_PORT ERDP_GPIOC
#define SYS_LED_PIN  ERDP_GPIO_PIN_0

class LED : private GpioDev {
   public:
    LED(ERDP_GpioPort_t port, ERDP_GpioPin_t pin, ERDP_Status_t on_level = ERDP_SET)
        : GpioDev(port, pin, ERDP_GPIO_PIN_MODE_OUTPUT, ERDP_GPIO_PIN_PULL_NONE, ERDP_GPIO_SPEED_LOW),
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

void Thread::main_thread(void *parm) {
    OLED_Init();
    OLED_Clear();
    // OLED_InvertBlock(0, 0, 128, 64);
    // OLED_ShowString(11,0,(uint8_t*)"Select a mode",16,1);
    OLED_Fill(0, 0, 127, 63, 0);
    OLED_Refresh_Gram();

    UartConfig_t uart_config = {
        .uart = ERDP_UART1,
        .baudrate = 115200,
        .mode = ERDP_UART_TX_RX,
        .tx_port = ERDP_GPIOA,
        .tx_pin = ERDP_GPIO_PIN_9,
        .tx_af = GPIO_AF_USART1,
        .rx_port = ERDP_GPIOA,
        .rx_pin = ERDP_GPIO_PIN_10,
        .rx_af = GPIO_AF_USART1,
        .priority = 6,
    };
    UartDev uart(uart_config, 100);
    uart.set_as_debug_com();
    Logger logger;
    logger.start();
    //    //uart.send((uint8_t*)"Hello World\n",12);
    Logger::i("Main", "Hello World");

    LED sys_led(SYS_LED_PORT, SYS_LED_PIN, ERDP_RESET);
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

    // Exti exti(ERDP_GPIOC, ERDP_GPIO_PIN_1, ERDP_EXTI_FALLING_EDGE, 8);
    // exti.set_usr_irq_hendler([&]() { Logger::i("Main", "EXTI IRQ"); });

    erdp::I2cDev oled_i2c(ERDP_I2C_MODE_I2C, oled_i2c_info, oled_i2c_cfg);
    oled_i2c.send(0x78, (uint8_t*)"Hello World", 12);

    std::vector<uint8_t> buffer;
    while (1) {
        if (uart.recv(buffer)) {
            uart.send(buffer);
            buffer.clear();
        }
        // OLED_ShowString(11,0,(uint8_t*)"Select a mode",16,1);
        OLED_Refresh_Gram();
        Thread::delay_ms(10);
    }
}
