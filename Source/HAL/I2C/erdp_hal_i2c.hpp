#ifndef __ERDP_HAL_I2C_HPP__
#define __ERDP_HAL_I2C_HPP__

#include "erdp_hal.hpp"
#include "erdp_if_i2c.h"

namespace erdp {

    using I2cConfig_t = ERDP_I2cCfg_t;
    using I2cInfo_t = ERDP_I2cInfo_t;

    class I2cDev {
       public:
#ifdef ERDP_ENABLE_RTOS
#define GET_SYS_TICK() Thread::get_system_1ms_ticks()
        using Buffer = Queue<uint8_t>;
#else
        using Buffer = RingBuffer<uint8_t>;
#define GET_SYS_TICK() erdp_if_rtos_get_system_1ms_ticks()
#endif
        I2cDev() = default;
        I2cDev(ERDP_I2cMode_t mode, const I2cInfo_t &i2c_info, const I2cConfig_t &i2c_cfg) { init(mode, i2c_info, i2c_cfg); }
        void init(ERDP_I2cMode_t mode, const I2cInfo_t &i2c_info, const I2cConfig_t &i2c_cfg) { 
            erdp_if_i2c_init(i2c_info.i2c, mode, &i2c_cfg);
            __i2c = i2c_info.i2c;
        }
        bool send(uint16_t address, uint8_t *data, uint32_t data_len, uint32_t timeout = 5,
                  ERDP_I2cAck_t is_ack = ERDP_I2C_ACK_ENABLE) {
            uint32_t start_time = GET_SYS_TICK();
            bool result = false;
            erdp_if_i2c_master_start(__i2c);
            if (!__wait_flag(erdp_if_i2c_master_start_done, timeout)) {
                erdp_if_i2c_stop(__i2c);
                return false;
            }
            erdp_if_i2c_master_send_7bits_address(__i2c, address, ERDP_I2C_DIRECTION_TRANSMITTER);
            if (is_ack == ERDP_I2C_ACK_ENABLE) {
                if (__wait_flag(erdp_if_master_send_address_done, timeout)) {
                    result = __send_with_ack(data, data_len, timeout);
                }
            } else {
                __NOP();
                __send_without_ack(data, data_len, timeout);
                result = true;
            }
            erdp_if_i2c_stop(__i2c);
            return result;
        }

        bool write_register_with_8bit_address(uint16_t slave_address, uint8_t register_address, uint8_t *data,
                                              uint32_t data_len, uint32_t timeout = 5,
                                              ERDP_I2cAck_t is_ack = ERDP_I2C_ACK_ENABLE) {
            uint32_t start_time = GET_SYS_TICK();
            bool result = false;

            erdp_if_i2c_master_start(__i2c);
            if (!__wait_flag(erdp_if_i2c_master_start_done, timeout)) {
                erdp_if_i2c_stop(__i2c);
                return false;
            }

            erdp_if_i2c_master_send_7bits_address(__i2c, slave_address, ERDP_I2C_DIRECTION_TRANSMITTER);
            if (is_ack == ERDP_I2C_ACK_ENABLE) {
                if (!__wait_flag(erdp_if_master_send_address_done, timeout)){
                    erdp_if_i2c_stop(__i2c);
                    return false;
                }
                if( __send_with_ack(&register_address, 1, timeout)){
                    result = __send_with_ack(data, data_len, timeout);
                }
                else{
                    erdp_if_i2c_stop(__i2c);
                    return false;
                }
            } else {
                __NOP();
                __send_without_ack(&register_address, 1, timeout);
                __NOP();
                __send_without_ack(data, data_len, timeout);
                result = true;
            }
            return result;
        }

       private:
        ERDP_I2C_t __i2c = ERDP_I2C1;
        bool __wait_flag(bool (*func)(ERDP_I2C_t i2c), uint32_t timeout) {
            uint32_t start_time = GET_SYS_TICK();
            while (GET_SYS_TICK() - start_time < timeout) {
                if (func(__i2c)) {
                    return true;
                }
            }
            return false;
        }

        bool __send_with_ack(uint8_t *data, uint32_t data_len, uint32_t timeout = 5) {
            for (uint32_t i = 0; i < data_len; i++) {
                erdp_if_i2c_send(__i2c, data[i]);
                if (!__wait_flag(erdp_if_i2c_master_transfer_complete, timeout)) {
                    return false;
                }
            }
            return true;
        }

        void __send_without_ack(uint8_t *data, uint32_t data_len, uint32_t timeout = 5) {
            for (uint32_t i = 0; i < data_len; i++) {
                erdp_if_i2c_send(__i2c, data[i]);
                __NOP();
            }
        }
    };
}    // namespace erdp
#endif