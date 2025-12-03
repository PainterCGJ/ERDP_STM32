#include "erdp_osal.hpp"
#include <new>
#include <cstring>
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
