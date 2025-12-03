/* erdp include */
#include "erdp_if_spi.h"
#include "erdp_if_gpio.h"

/* platform include */
#include "stm32f4xx.h"

extern void erdp_spi_irq_handler(ERDP_Spi_t spi);

const static uint32_t spi_instance[ERDP_SPI_NUM] = {
    // (uint32_t)SPI0,
    // (uint32_t)SPI1,
    // (uint32_t)SPI2,
    // (uint32_t)SPI3,
};

const static uint32_t spi_pclk[ERDP_SPI_NUM] = {
    // (uint32_t)SPI0,
    // (uint32_t)SPI1,
    // (uint32_t)SPI2,
    // (uint32_t)SPI3,
};

const static uint8_t spi_irq_id[ERDP_SPI_NUM] = {
    // (uint8_t)SPI0_IRQn,
    // (uint8_t)SPI1_IRQn,
    // (uint8_t)SPI2_IRQn,
    // (uint8_t)SPI3_IRQn,
};



uint32_t erdp_if_spi_get_PCLK(ERDP_Spi_t spi)
{
    return spi_pclk[spi];
}
void erdp_if_spi_gpio_init(ERDP_SpiInfo_t *spi_info, ERDP_SpiMode_t mode)
{

    // uint32_t gpio_periph;
    // uint32_t gpio_pin;

    // rcu_periph_clock_enable(erdp_if_gpio_get_PCLK(spi_info->miso_port));
    // rcu_periph_clock_enable(erdp_if_gpio_get_PCLK(spi_info->mosi_port));
    // rcu_periph_clock_enable(erdp_if_gpio_get_PCLK(spi_info->sck_port));
    // rcu_periph_clock_enable(erdp_if_gpio_get_PCLK(spi_info->cs_port));

    // gpio_periph = erdp_if_gpio_get_port(spi_info->mosi_port);
    // gpio_pin = erdp_if_gpio_get_pin(spi_info->mosi_pin);
    // gpio_mode_set(gpio_periph, GPIO_MODE_AF, GPIO_PUPD_NONE, gpio_pin);
    // gpio_output_options_set(gpio_periph, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, gpio_pin);
    // gpio_af_set(gpio_periph, spi_info->mosi_af, gpio_pin);

    // gpio_periph = erdp_if_gpio_get_port(spi_info->miso_port);
    // gpio_pin = erdp_if_gpio_get_pin(spi_info->miso_pin);
    // gpio_mode_set(gpio_periph, GPIO_MODE_AF, GPIO_PUPD_NONE, gpio_pin);
    // gpio_output_options_set(gpio_periph, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, gpio_pin);
    // gpio_af_set(gpio_periph, spi_info->miso_af, gpio_pin);

    // gpio_periph = erdp_if_gpio_get_port(spi_info->sck_port);
    // gpio_pin = erdp_if_gpio_get_pin(spi_info->sck_pin);
    // gpio_mode_set(gpio_periph, GPIO_MODE_AF, GPIO_PUPD_NONE, gpio_pin);
    // gpio_output_options_set(gpio_periph, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, gpio_pin);
    // gpio_af_set(gpio_periph, spi_info->sck_af, gpio_pin);

    // gpio_periph = erdp_if_gpio_get_port(spi_info->cs_port);
    // gpio_pin = erdp_if_gpio_get_pin(spi_info->cs_pin);
    // if (mode == ERDP_SPI_MODE_MASTER)
    // {
    //     gpio_mode_set(gpio_periph, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, gpio_pin);
    //     gpio_output_options_set(gpio_periph, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, gpio_pin);
    // }
    // else if (mode == ERDP_SPI_MODE_SLAVE)
    // {
    //     gpio_mode_set(gpio_periph, GPIO_MODE_AF, GPIO_PUPD_NONE, gpio_pin);
    //     gpio_af_set(gpio_periph, spi_info->cs_af, gpio_pin);
    // }
}

void erdp_if_spi_init(ERDP_Spi_t spi, ERDP_SpiMode_t mode, ERDP_SpiCfg_t *spi_cfg, ERDP_SpiDataSize_t data_size)
{
    // rcu_periph_clock_enable(erdp_if_spi_get_PCLK(spi));
    // spi_parameter_struct spi_init_struct;
    // spi_init_struct.trans_mode = SPI_TRANSMODE_FULLDUPLEX;

    // if (mode == ERDP_SPI_MODE_MASTER)
    // {
    //     spi_init_struct.nss = SPI_NSS_SOFT;
    //     spi_init_struct.device_mode = SPI_MASTER;
    // }
    // else if (mode == ERDP_SPI_MODE_SLAVE)
    // {
    //     spi_init_struct.nss = SPI_NSS_HARD;
    //     spi_init_struct.device_mode = SPI_SLAVE;
    //     nvic_irq_enable(spi_irq_id[spi], spi_cfg->priority, 0);
    //     spi_i2s_interrupt_enable(spi_instance[spi], SPI_I2S_INT_RBNE);
    // }

    // if (data_size == ERDP_SPI_DATASIZE_8BIT)
    // {
    //     spi_init_struct.frame_size = SPI_FRAMESIZE_8BIT;
    // }
    // else if (data_size == ERDP_SPI_DATASIZE_16BIT)
    // {
    //     spi_init_struct.frame_size = SPI_FRAMESIZE_16BIT;
    // }

    // if (spi_cfg->clk_mode == ERDP_SPI_CLKMODE_0)
    // {
    //     spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    // }
    // else if (spi_cfg->clk_mode == ERDP_SPI_CLKMODE_1)
    // {
    //     spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_2EDGE;
    // }
    // else if (spi_cfg->clk_mode == ERDP_SPI_CLKMODE_2)
    // {
    //     spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_1EDGE;
    // }
    // else if (spi_cfg->clk_mode == ERDP_SPI_CLKMODE_3)
    // {
    //     spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
    // }

    // if (spi_cfg->endian == ERDP_SPI_ENDIAN_MSB)
    // {
    //     spi_init_struct.endian = SPI_ENDIAN_MSB;
    // }
    // else if (spi_cfg->endian == ERDP_SPI_ENDIAN_LSB)
    // {
    //     spi_init_struct.endian = SPI_ENDIAN_LSB;
    // }

    // spi_init_struct.prescale = spi_cfg->prescale;
    // spi_init(spi_instance[spi], &spi_init_struct);
    // spi_enable(spi_instance[spi]);
}

void erdp_if_spi_deinit(ERDP_Spi_t spi)
{
    // spi_disable(spi_instance[spi]);
    // spi_i2s_interrupt_disable(spi_instance[spi], SPI_I2S_INT_RBNE);
    // nvic_irq_disable(spi_irq_id[spi]);
    // rcu_periph_clock_disable(erdp_if_spi_get_PCLK(spi));
}

void erdp_if_spi_enable(ERDP_Spi_t spi, bool enable)
{
    // if (enable)
    // {
    //     spi_enable(spi_instance[spi]);
    // }
    // else
    // {
    //     spi_disable(spi_instance[spi]);
    // }
}
void erdp_if_spi_send(ERDP_Spi_t spi, uint16_t data)
{
    // spi_i2s_data_transmit(spi_instance[spi], data);
}

uint16_t erdp_if_spi_recv(ERDP_Spi_t spi)
{
    // return spi_i2s_data_receive(spi_instance[spi]);
	return 0;
}

bool erdp_if_spi_transfer_complete(ERDP_Spi_t spi)
{
    // return !spi_i2s_flag_get(spi_instance[spi], SPI_STAT_TRANS);
	return 0;
}

bool erdp_if_spi_transmit_buffer_empty(ERDP_Spi_t spi)
{
    // return spi_i2s_flag_get(spi_instance[spi], SPI_STAT_TBE);
	return 0;
}

bool erdp_if_spi_receive_buffer_not_empty(ERDP_Spi_t spi)
{
    // return spi_i2s_flag_get(spi_instance[spi], SPI_STAT_RBNE);
	return 0;
}

void SPI0_IRQHandler(void)
{
    erdp_spi_irq_handler(ERDP_SPI0);
}

void SPI1_IRQHandler(void)
{
    erdp_spi_irq_handler(ERDP_SPI1);
}

void SPI2_IRQHandler(void)
{
    erdp_spi_irq_handler(ERDP_SPI2);
}

void SPI3_IRQHandler(void)
{
    erdp_spi_irq_handler(ERDP_SPI3);
}
