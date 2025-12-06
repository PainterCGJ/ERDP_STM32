#ifndef __ERDP_OSAL_RINGBUFFER_HPP__
#define __ERDP_OSAL_RINGBUFFER_HPP__
#include "erdp_assert.h"
#include "erdp_osal_fifo.hpp"
#include <cstddef>
#include <cstdint>

namespace erdp
{
    template <typename T>
    class RingBuffer : public FifoBase<T>
    {
    public:
        RingBuffer(const RingBuffer &) = delete;
        RingBuffer &operator=(const RingBuffer &) = delete;

        RingBuffer() {}

        RingBuffer(uint8_t *mempool, size_t mempool_size) noexcept
        {
            init(mempool, mempool_size);
        }

        RingBuffer(size_t size) noexcept
        {
            init(size);
        }

        bool init(uint8_t *mempool, size_t mempool_size)
        {
            erdp_assert(mempool != nullptr);
            erdp_assert(mempool_size % sizeof(T) == 0);
            __buffer = reinterpret_cast<T *>(mempool);
            __size = mempool_size / sizeof(T);
            __head = 0;
            __tail = 0;
            return true;
        }

        bool init(uint32_t size) noexcept
        {
            __buffer = new T[size];
            if (__buffer == nullptr)
            {
                return false;
            }
            __size = size;
            __head = 0;
            __tail = 0;
            return true;
        }

        bool full() const noexcept { return (__tail + 1) % __size == __head; }

        bool empty() const noexcept { return __head == __tail; }

        bool push(const T &item) noexcept
        {
            if (full())
            {
                return false;
            }
            __buffer[__tail] = item;
            __tail = (__tail + 1) % __size;
            return true;
        }

        bool pop(T &item) noexcept
        {
            if (empty())
            {
                return false;
            }
            item = __buffer[__head];
            __head = (__head + 1) % __size;
            return true;
        }

        uint32_t size() const noexcept
        {
            return (__tail - __head + __size) % __size;
        }

    private:
        T *__buffer;
        uint32_t __size;
        volatile uint32_t __head;
        volatile uint32_t __tail;

        T &operator[](uint32_t index)
        {
            erdp_assert(index < size());
            return *reinterpret_cast<T *>(__buffer + ((__head + index) % __size));
        }

        const T &operator[](uint32_t index) const
        {
            erdp_assert(index < size());
            return *reinterpret_cast<const T *>(__buffer + ((__head + index) % __size));
        }
    };
} // namespace erdp

#endif

