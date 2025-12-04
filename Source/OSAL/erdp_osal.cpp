#include "erdp_osal.hpp"
#include <new>
#include <cstring>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "erdp_if_rtos.h"

extern "C" {

// 必须使用 volatile，且严格返回 0/1
int __cxa_guard_acquire(volatile int* guard_object) {
    return !*(volatile char*)guard_object;
}

void __cxa_guard_release(volatile int* guard_object) {
    *(volatile char*)guard_object = 1;
}

void __cxa_guard_abort(volatile int*) {
    // 可选实现，一般忽略
}

/**
 * @brief 覆盖标准库的 malloc，使用 FreeRTOS 堆管理
 * @param size 要分配的内存大小（字节）
 * @return 指向分配内存的指针，失败返回 NULL
 */
void *malloc(size_t size)
{
    return erdp_if_rtos_malloc(size);
}

/**
 * @brief 覆盖标准库的 free，使用 FreeRTOS 堆管理
 * @param ptr 要释放的内存指针
 */
void free(void *ptr)
{
    erdp_if_rtos_free(ptr);
}

/**
 * @brief 覆盖标准库的 calloc，分配并清零内存
 * @param num 元素数量
 * @param size 每个元素的大小（字节）
 * @return 指向分配内存的指针，失败返回 NULL
 */
void *calloc(size_t num, size_t size)
{
    // 检查溢出：如果 num * size 会溢出，返回 NULL
    if (num != 0 && size > (SIZE_MAX / num))
    {
        return NULL;
    }
    
    size_t total_size = num * size;
    void *ptr = erdp_if_rtos_malloc(total_size);
    if (ptr != NULL)
    {
        memset(ptr, 0, total_size);
    }
    return ptr;
}

/**
 * @brief 覆盖标准库的 realloc，重新分配内存
 * @param ptr 原内存指针（可以为 NULL）
 * @param size 新的内存大小（字节）
 * @return 指向重新分配内存的指针，失败返回 NULL
 * @warning 由于 FreeRTOS heap_4 不提供获取已分配内存块大小的接口，
 *          此实现无法自动复制原有数据。调用者需要自己处理数据迁移。
 *          建议：如果需要保留数据，调用者应记录原大小并手动复制数据。
 */
void *realloc(void *ptr, size_t size)
{
    // 如果 ptr 为 NULL，等同于 malloc
    if (ptr == NULL)
    {
        return erdp_if_rtos_malloc(size);
    }

    // 如果 size 为 0，等同于 free
    if (size == 0)
    {
        erdp_if_rtos_free(ptr);
        return NULL;
    }

    // 分配新内存
    void *new_ptr = erdp_if_rtos_malloc(size);
    if (new_ptr == NULL)
    {
        return NULL;
    }

    // 注意：FreeRTOS heap_4 的内存块头部信息不对外暴露，
    // 无法获取原内存块大小，因此无法自动复制数据。
    // 调用者需要自己处理数据迁移（如果需要保留数据）。
    
    erdp_if_rtos_free(ptr);
    return new_ptr;
}

}



#ifdef ERDP_ENABLE_RTOS
namespace erdp
{
    OS_TaskHandle Thread::__main_task = nullptr;
    void erdp_task_run(void *parm)
    {
        Thread *thead = static_cast<Thread *>(parm);
        thead->thread_code();
        thead->kill();
    }

    void create_main_task()
    {
        Thread::__main_task = erdp_if_rtos_task_create(Thread::main_thread, "main", ERDP_CONFIG_MAIN_THREAD_STACK_SIZE, nullptr, 20);
    }
} // namespace erdp
int main(void)
{
    erdp_if_rtos_system_config();
    erdp::create_main_task();
    erdp::Thread::start_scheduler();
    while (1)
        ;
    return 0;
}

void *operator new(size_t size)
{
    return erdp_if_rtos_malloc(size);
}

void *operator new[](size_t size)
{
    return erdp_if_rtos_malloc(size);
}

void operator delete(void *pointer) noexcept
{
    erdp_if_rtos_free(pointer);
}

void operator delete[](void *pointer) noexcept
{
    erdp_if_rtos_free(pointer);
}

#else  // ERDP_ENABLE_RTOS
namespace erdp
{

    Heap4 *default_heap = nullptr;
    static uint8_t memory_pool[ERDP_CONFIG_HEAP_SIZE];
    Heap4 __heap;

} // namespace erdp

// C++操作符重载实现(无异常)
void *operator new(size_t size)
{
    if (!erdp::default_heap)
    {
        erdp::__heap.init(erdp::memory_pool, sizeof(erdp::memory_pool));
        erdp::default_heap = &erdp::__heap;
    }
    return erdp::default_heap->allocate(size);
}

void *operator new[](size_t size)
{
    return operator new(size);
}

void operator delete(void *ptr) noexcept
{
    if (ptr && erdp::default_heap)
    {
        erdp::default_heap->deallocate(ptr);
    }
}

void operator delete[](void *ptr) noexcept
{
    operator delete(ptr);
}

void operator delete(void *ptr, size_t) noexcept
{
    operator delete(ptr);
}

void operator delete[](void *ptr, size_t) noexcept
{
    operator delete(ptr);
}
#endif // ERDP_ENABLE_RTOS
