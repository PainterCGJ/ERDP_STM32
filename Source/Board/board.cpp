#include "board.hpp"

const erdp::I2cConfig_t oled_i2c_cfg = {
    .frequency = 100000,
    .own_address = 0x00,
    .acknowledged_address = ERDP_I2C_ADDRESS_7BIT,
    .ack = ERDP_I2C_ACK_ENABLE,
};

const erdp::I2cInfo_t oled_i2c_info = {
    .i2c = ERDP_I2C2,

    .scl_port = ERDP_GPIOB,
    .scl_pin = ERDP_GPIO_PIN_10,
    .scl_af = GPIO_AF_I2C2,

    .sda_port = ERDP_GPIOB,
    .sda_pin = ERDP_GPIO_PIN_11,
    .sda_af = GPIO_AF_I2C2,
};