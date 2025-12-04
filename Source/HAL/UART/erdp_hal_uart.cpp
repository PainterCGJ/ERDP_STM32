#include "erdp_hal_uart.hpp"
namespace erdp
{
    UartDev *UartDev::__instance[ERDP_UART_NUM];
    UartDev *UartDev::__debug_com = nullptr;

    extern "C"
    {
        void erdp_uart_irq_handler(ERDP_Uart_t uart)
        {
            UartDev::__instance[uart]->__irq_handler();
        }

        // C interface function for syscalls.c to output characters to UART
        void erdp_uart_putchar(char character)
        {
            if (UartDev::get_debug_com() != nullptr)
            {
                UartDev::get_debug_com()->send((const uint8_t *)&character, 1);
            }
        }
    }
}