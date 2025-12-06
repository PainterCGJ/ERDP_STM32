#ifndef __ERDP_OSAL_SEMAPHORE_HPP__
#define __ERDP_OSAL_SEMAPHORE_HPP__
#include "erdp_if_rtos.h"
#include <type_traits>

namespace erdp
{
#ifdef ERDP_ENABLE_RTOS
    template <Semaphore_tag T>
    class Semaphore
    {
    public:
        template <Semaphore_tag U = T, typename = std::enable_if_t<U != COUNT_TAG>>
        Semaphore()
        {
            if constexpr (U == BINARY_TAG)
            {
                __handler = erdp_if_rtos_semaphore_creat(BINARY_TAG);
            }
            else if constexpr (U == MUTEX_TAG)
            {
                __handler = erdp_if_rtos_semaphore_creat(MUTEX_TAG);
            }
            else if constexpr (U == RECURISIVE_TAG)
            {
                __handler = erdp_if_rtos_semaphore_creat(RECURISIVE_TAG);
            }
        }

        template <Semaphore_tag U = T, typename = std::enable_if_t<U == COUNT_TAG>>
        Semaphore(uint32_t max_count, uint32_t initial_count)
        {
            __handler = erdp_if_rtos_counting_semaphore_creat(max_count, initial_count);
        }

        // 获取信号量
        bool take(uint32_t ticks_to_wait = portMAX_DELAY)
        {
            if constexpr (T == RECURISIVE_TAG)
            {
                return erdp_if_rtos_recursive_semaphore_take(__handler, ticks_to_wait);
            }
            else
            {
                return erdp_if_rtos_semaphore_take(__handler, ticks_to_wait);
            }
        }

        // 释放信号量
        bool give()
        {
            if constexpr (T == RECURISIVE_TAG)
            {
                return erdp_if_rtos_recursive_semaphore_give(__handler);
            }
            else
            {
                return erdp_if_rtos_semaphore_give(__handler);
            }
        }

        ~Semaphore()
        {
            if (__handler != nullptr)
            {
                erdp_if_rtos_semaphore_delet(__handler);
            }
        }

        // 删除拷贝构造和赋值
        Semaphore(const Semaphore &) = delete;
        Semaphore &operator=(const Semaphore &) = delete;

    private:
        OS_Semaphore __handler = nullptr;
    };

    class Mutex : private Semaphore<MUTEX_TAG>
    {
    public:
        Mutex() : Semaphore<MUTEX_TAG>() {}

        bool lock(uint32_t ticks_to_wait = portMAX_DELAY)
        {
            return take(ticks_to_wait);
        }
        bool try_lock()
        {
            return take(0);
        }
        bool unlock()
        {
            return give();
        }
    };
#endif // ERDP_ENABLE_RTOS
} // namespace erdp

#endif

