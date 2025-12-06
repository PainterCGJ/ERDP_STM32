#ifndef __ERDP_OSAL_FIFO_HPP__
#define __ERDP_OSAL_FIFO_HPP__
#include <cstddef>
#include <cstdint>
namespace erdp
{
    template <typename _Type>
    class FifoBase
    {
    public:
        FifoBase() {}
        ~FifoBase() {}

        virtual bool init(size_t size) = 0;
        virtual bool push(const _Type &elm) = 0;
        virtual bool pop(_Type &elm) = 0;
        virtual uint32_t size() const noexcept = 0;
        virtual bool empty() const noexcept = 0;
        virtual bool full() const noexcept = 0;
    };
} // namespace erdp

#endif

