#ifndef __ERDP_IF_I2C_H__
#define __ERDP_IF_I2C_H__
#ifdef __cplusplus
extern "C" {
#endif
#include "erdp_if_gpio.h"
#include "erdp_interface.h"

    typedef enum {
        ERDP_I2C1 = 0,
        ERDP_I2C2,
        ERDP_I2C3,

        ERDP_I2C_NUM,    // Maximum number of I2C
    } ERDP_I2C_t;

    typedef enum {
        ERDP_I2C_MODE_I2C,
        ERDP_I2C_MODE_SMBUS_DEVICE,
        ERDP_I2C_MODE_SMBUS_HOST,
    } ERDP_I2cMode_t;

    typedef enum {
        ERDP_I2C_ACK_ENABLE,
        ERDP_I2C_ACK_DISABLE,
    } ERDP_I2cAck_t;

    typedef enum {
        ERDP_I2C_ADDRESS_7BIT,
        ERDP_I2C_ADDRESS_10BIT,
    } ERDP_I2cAddressLen_t;

    typedef enum {
        ERDP_I2C_DIRECTION_TRANSMITTER,
        ERDP_I2C_DIRECTION_RECEIVER,
    } ERDP_I2cDirection_t;

    typedef struct {
        ERDP_I2C_t i2c;
        ERDP_GpioPort_t scl_port;    // GPIO port for SCL pin
        ERDP_GpioPin_t scl_pin;      // GPIO pin for SCL pin
        uint32_t scl_af;             // GPIO function number for SCL pin

        ERDP_GpioPort_t sda_port;    // GPIO port for SDA pin
        ERDP_GpioPin_t sda_pin;      // GPIO pin for SDA pin
        uint32_t sda_af;             // GPIO function number for SDA pin
    } ERDP_I2cInfo_t;

    typedef struct {
        uint32_t frequency;
        uint16_t own_address;
        ERDP_I2cAddressLen_t acknowledged_address;
        ERDP_I2cAck_t ack;
    } ERDP_I2cCfg_t;

    /**
     * @brief Initialize I2C GPIO pins
     * @param[in] i2c_info Pointer to I2C GPIO configuration structure
     */
    void erdp_if_i2c_gpio_init(const ERDP_I2cInfo_t *i2c_info);
    /**
     * @brief Initialize I2C peripheral
     * @param[in] i2c I2C instance identifier
     * @param[in] mode I2C mode
     * @param[in] i2c_cfg Pointer to I2C configuration structure
     */
    void erdp_if_i2c_init(ERDP_I2C_t i2c, ERDP_I2cMode_t mode, const ERDP_I2cCfg_t *i2c_cfg);

    /**
     * @brief Deinitialize I2C peripheral
     * @param[in] i2c I2C instance identifier
     */
    void erdp_if_i2c_deinit(ERDP_I2C_t i2c);

    /**
     * @brief Enable I2C peripheral
     * @param[in] i2c I2C instance identifier
     * @param[in] enable true to enable I2C, false to disable I2C
     */
    void erdp_if_i2c_enable(ERDP_I2C_t i2c, bool enable);

    /**
     * @brief Send data via I2C
     * @param[in] i2c I2C instance identifier
     * @param[in] data Data to send
     */
    void erdp_if_i2c_send(ERDP_I2C_t i2c, uint8_t data);

    /**
     * @brief Start I2C master mode
     * @param[in] i2c I2C instance identifier
     */
    void erdp_if_i2c_master_start(ERDP_I2C_t i2c);

    /**
     * @brief Check if I2C master mode start is done
     */
    bool erdp_if_i2c_master_start_done(ERDP_I2C_t i2c);

    /**
     * @brief Send 7 bits address via I2C
     * @param[in] i2c I2C instance identifier
     * @param[in] address Address to send
     * @param[in] direction Direction to send
     */
    void erdp_if_i2c_master_send_7bits_address(ERDP_I2C_t i2c, uint8_t address, ERDP_I2cDirection_t direction);

    /**
     * @brief Check if I2C master mode send 7 bits address is done
     * @param[in] i2c I2C instance identifier
     */
    bool erdp_if_master_send_address_done(ERDP_I2C_t i2c);

    /**
     * @brief Receive ack via I2C
     * @param[in] i2c I2C instance identifier
     */
    bool erdp_if_i2c_master_recv_ack(ERDP_I2C_t i2c);

    /**
     * @brief Check if I2C transfer is complete
     * @param[in] i2c I2C instance identifier
     */
    bool erdp_if_i2c_master_transfer_complete(ERDP_I2C_t i2c);

    /**
     * @brief Stop I2C
     * @param[in] i2c I2C instance identifier
     */
    void erdp_if_i2c_stop(ERDP_I2C_t i2c);

#ifdef __cplusplus
}
#endif

#endif    // __ERDP_IF_I2C_H__