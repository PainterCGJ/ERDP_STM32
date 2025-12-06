#ifndef __ERDP_OSAL_QUEUE_HPP__
#define __ERDP_OSAL_QUEUE_HPP__
#include "erdp_assert.h"
#include "erdp_if_rtos.h"
#include "erdp_osal_fifo.hpp"

namespace erdp
{
#ifdef ERDP_ENABLE_RTOS
    template <typename _Type>
    class Queue : public FifoBase<_Type>
    {
    public:
        Queue() {}
        Queue(uint32_t queue_length)
        {
            init(queue_length);
        }
        ~Queue() { erdp_if_rtos_queue_delet(__handler); }

        bool init(size_t queue_length)
        {
            __handler = erdp_if_rtos_queue_create(queue_length, sizeof(_Type));
            if (__handler == nullptr)
            {
                return false;
            }
            __queue_length = queue_length;
            __queue_size = 0;
            return true;
        }
        bool push(const _Type &elm_to_push, uint32_t ticks_to_wait)
        {
            erdp_assert(__handler != nullptr);
            if (erdp_if_rtos_queue_send(__handler, (uint8_t *)(&elm_to_push), ticks_to_wait))
            {
                __queue_size++;
                return true;
            }
            return false;
        }

        bool push(const _Type &elm_to_push)
        {
            erdp_assert(__handler != nullptr);
            if (erdp_if_rtos_queue_send(__handler, (uint8_t *)(&elm_to_push), 0))
            {
                __queue_size++;
                return true;
            }
            return false;
        }

        bool pop(_Type &elm_recv, uint32_t ticks_to_wait)
        {
            erdp_assert(__handler != nullptr);
            if (erdp_if_rtos_queue_recv(__handler, (uint8_t *)(&elm_recv), ticks_to_wait))
            {
                __queue_size--;
                return true;
            }
            return false;
        }

        bool pop(_Type &elm_recv)
        {
            erdp_assert(__handler != nullptr);
            if (erdp_if_rtos_queue_recv(__handler, (uint8_t *)(&elm_recv), 0))
            {
                __queue_size--;
                return true;
            }
            return false;
        }

        bool empty() const noexcept
        {
            erdp_assert(__handler != nullptr);
            if (__queue_size == 0)
            {
                return true;
            }
            return false;
        }

        bool full() const noexcept
        {
            erdp_assert(__handler != nullptr);
            if (__queue_size == __queue_length)
            {
                return true;
            }
            return false;
        }

        uint32_t size() const noexcept
        {
            erdp_assert(__handler != nullptr);
            return __queue_size;
        }

    private:
        OS_Queue __handler;
        uint32_t __queue_length;
        uint32_t __queue_size;
    };
#endif // ERDP_ENABLE_RTOS
} // namespace erdp

#endif

