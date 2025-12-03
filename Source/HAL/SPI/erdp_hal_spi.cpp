#include "erdp_hal_spi.hpp"
namespace erdp
{
    SpiBase *SpiBase::__spi_instance[ERDP_SPI_NUM];
    extern "C"
    {
        void erdp_spi_irq_handler(ERDP_Spi_t spi)
        {
            if (SpiBase::__spi_instance[spi] != nullptr)
            {
                SpiBase::__spi_instance[spi]->__irq_handler();
            }
        }
    }
}
