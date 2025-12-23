#ifndef __BOARD_HPP__
#define __BOARD_HPP__

#include "erdp_hal_gpio.hpp"
#include "erdp_hal_i2c.hpp"
#include "erdp_hal_spi.hpp"
#include "erdp_hal_uart.hpp"
#include "erdp_hal_exti.hpp"

extern const erdp::I2cConfig_t oled_i2c_cfg;
extern const erdp::I2cInfo_t oled_i2c_info;

#endif