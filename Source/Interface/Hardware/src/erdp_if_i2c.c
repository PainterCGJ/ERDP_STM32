/* erdp include */
#include "erdp_if_i2c.h"

/* platform include */
#include "stm32f4xx.h"

const static uint32_t i2c_pclk[ERDP_I2C_NUM] = {
    RCC_APB1Periph_I2C1,
    RCC_APB1Periph_I2C2,
    RCC_APB1Periph_I2C3,
};

const static uint32_t i2c_irq[ERDP_I2C_NUM] = {
    I2C1_EV_IRQn,
    I2C2_EV_IRQn,
    I2C3_EV_IRQn,
};

const static uint32_t i2c_instance[ERDP_I2C_NUM] = {
    (uint32_t)I2C1,
    (uint32_t)I2C2,
    (uint32_t)I2C3,
};

uint32_t erdp_if_i2c_get_periph(ERDP_I2C_t i2c) { return i2c_instance[i2c]; }

uint32_t erdp_if_i2c_get_PCLK(ERDP_I2C_t i2c) { return i2c_pclk[i2c]; }

uint32_t erdp_if_i2c_get_irq(ERDP_I2C_t i2c) { return i2c_irq[i2c]; }

void erdp_if_i2c_gpio_init(const ERDP_I2cInfo_t *i2c_info) {
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_TypeDef *scl_periph = (GPIO_TypeDef *)erdp_if_gpio_get_port(i2c_info->scl_port);
    GPIO_TypeDef *sda_periph = (GPIO_TypeDef *)erdp_if_gpio_get_port(i2c_info->sda_port);

    RCC_AHB1PeriphClockCmd(erdp_if_gpio_get_PCLK(i2c_info->scl_port), ENABLE);
    RCC_AHB1PeriphClockCmd(erdp_if_gpio_get_PCLK(i2c_info->sda_port), ENABLE);
    GPIO_PinAFConfig(scl_periph, i2c_info->scl_pin, i2c_info->scl_af);
    GPIO_PinAFConfig(sda_periph, i2c_info->sda_pin, i2c_info->sda_af);

    GPIO_InitStructure.GPIO_Pin = i2c_info->scl_pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(scl_periph, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = i2c_info->sda_pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(sda_periph, &GPIO_InitStructure);
}

void erdp_if_i2c_init(ERDP_I2C_t i2c, ERDP_I2cMode_t mode, const ERDP_I2cCfg_t *i2c_cfg) {
    I2C_InitTypeDef I2C_InitStructure;
    I2C_TypeDef *i2c_periph = (I2C_TypeDef *)erdp_if_i2c_get_periph(i2c);

    // Set I2C mode
    switch (mode) {
        case ERDP_I2C_MODE_I2C:
            I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
            break;
        case ERDP_I2C_MODE_SMBUS_DEVICE:
            I2C_InitStructure.I2C_Mode = I2C_Mode_SMBusDevice;
            break;
        case ERDP_I2C_MODE_SMBUS_HOST:
            I2C_InitStructure.I2C_Mode = I2C_Mode_SMBusHost;
            break;
    }

    // Set I2C duty cycle
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_16_9;

    // Set I2C acknowledged address
    switch (i2c_cfg->acknowledged_address) {
        case ERDP_I2C_ADDRESS_7BIT:
            I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
            break;
        case ERDP_I2C_ADDRESS_10BIT:
            I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_10bit;
            break;
    }

    // Set I2C clock speed
    I2C_InitStructure.I2C_ClockSpeed = i2c_cfg->frequency;

    // Set I2C acknowledge
    switch (i2c_cfg->ack) {
        case ERDP_I2C_ACK_ENABLE:
            I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
            break;
        case ERDP_I2C_ACK_DISABLE:
            I2C_InitStructure.I2C_Ack = I2C_Ack_Disable;
            break;
    }

    // Set I2C own address
    I2C_InitStructure.I2C_OwnAddress1 = i2c_cfg->own_address;

    // Initialize I2C
    I2C_Init(i2c_periph, &I2C_InitStructure);
}

void erdp_if_i2c_deinit(ERDP_I2C_t i2c) { I2C_Cmd((I2C_TypeDef *)erdp_if_i2c_get_periph(i2c), DISABLE); }

void erdp_if_i2c_enable(ERDP_I2C_t i2c, bool enable) {
    if (enable) {
        I2C_Cmd((I2C_TypeDef *)erdp_if_i2c_get_periph(i2c), ENABLE);
    } else {
        I2C_Cmd((I2C_TypeDef *)erdp_if_i2c_get_periph(i2c), DISABLE);
    }
}

void erdp_if_i2c_send(ERDP_I2C_t i2c, uint8_t data) { I2C_SendData((I2C_TypeDef *)erdp_if_i2c_get_periph(i2c), data); }

void erdp_if_i2c_master_start(ERDP_I2C_t i2c) { I2C_GenerateSTART((I2C_TypeDef *)erdp_if_i2c_get_periph(i2c), ENABLE); }

bool erdp_if_i2c_master_start_done(ERDP_I2C_t i2c) {
    if (I2C_CheckEvent((I2C_TypeDef *)erdp_if_i2c_get_periph(i2c), I2C_EVENT_MASTER_MODE_SELECT) == SET) {
        return true;
    }

    return false;
}

void erdp_if_i2c_master_send_7bits_address(ERDP_I2C_t i2c, uint8_t address, ERDP_I2cDirection_t direction) {
    if (direction == ERDP_I2C_DIRECTION_TRANSMITTER) {
        I2C_Send7bitAddress((I2C_TypeDef *)erdp_if_i2c_get_periph(i2c), address, I2C_Direction_Transmitter);
    } else {
        I2C_Send7bitAddress((I2C_TypeDef *)erdp_if_i2c_get_periph(i2c), address, I2C_Direction_Receiver);
    }
}

bool erdp_if_master_send_address_done(ERDP_I2C_t i2c) {
    return I2C_GetFlagStatus((I2C_TypeDef *)erdp_if_i2c_get_periph(i2c), I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == SET;
}

bool erdp_if_i2c_master_recv_ack(ERDP_I2C_t i2c) {
    // 检查ADDR标志位：在主机模式下，该标志表示地址已发送且从机已应答（ACK）
    // I2C_FLAG_ADDR: Address sent flag (Master mode) - 地址已发送标志（主机模式）
    if (I2C_GetFlagStatus((I2C_TypeDef *)erdp_if_i2c_get_periph(i2c), I2C_FLAG_ADDR) == SET) {
        return true;
    }
    return false;
}

void erdp_if_i2c_stop(ERDP_I2C_t i2c) { I2C_GenerateSTOP((I2C_TypeDef *)erdp_if_i2c_get_periph(i2c), ENABLE); }

bool erdp_if_i2c_master_transfer_complete(ERDP_I2C_t i2c) {
    return I2C_GetFlagStatus((I2C_TypeDef *)erdp_if_i2c_get_periph(i2c), I2C_EVENT_MASTER_BYTE_TRANSMITTED) == SET;
}