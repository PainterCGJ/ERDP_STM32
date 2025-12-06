#ifndef __ERDP_OSAL_HPP__
#define __ERDP_OSAL_HPP__
#include <cstddef>
#include <functional>

#include "erdp_assert.h"
#include "erdp_config.h"
#include "erdp_heap.hpp"
#include "erdp_if_rtos.h"
#include "erdp_osal_event.hpp"
#include "erdp_osal_fifo.hpp"
#include "erdp_osal_queue.hpp"
#include "erdp_osal_ringbuffer.hpp"
#include "erdp_osal_semaphore.hpp"
#include "erdp_osal_timer.hpp"
#include "string.h"

namespace erdp {
    extern "C" {
        void erdp_task_run(void *parm);
    }

#ifdef ERDP_ENABLE_RTOS
    class Thread {
        friend void create_main_task();

       public:
        /**
         * @brief 创建线程，线程无传入参数，可用于直接创建线程对象
         * @param[in] task_code 线程代码
         * @param[in] name 线程名称
         * @param[in] priority 线程优先值
         * @param[in] starck_size 堆栈大小，默认值为Default_Starck_size
         * @example
         * void task(void* parg)
         * {
         *      printf("hello world\r\n");
         *      //运行结束，自动删除线程
         * }
         *
         * void rtos_main(void)
         * {
         *      Thread TASK(task, "task", 1);
         *      task.join();
         *      while(1){}
         * }
         */
        Thread(void (*task_code)(void *p_arg), const char *name, uint32_t priority,
               size_t starck_size = DEFAULT_STACK_SIZE)
            : __thread_code(task_code), __starck_size(starck_size) {
            strcpy(__name, name);
        }

        /**
         * @brief 创建线程，线程有传入参数，可用于直接创建线程对象
         * @param[in] task_code 线程代码
         * @param[in] p_arg 线程传入参数
         * @param[in] name 线程名称
         * @param[in] priority 线程优先值
         * @param[in] starck_size 堆栈大小，默认值为Default_Starck_size
         * @example
         * void task(void* parg)
         * {
         *      printf("a = %d", *((int*)(parg)));
         *      //运行结束，自动删除线程
         * }
         *
         * void rtos_main(void)
         * {
         *      int a = 10;
         *      Thread TASK(task, (void*)(&a), "task", 1);
         *      task.join();
         *      while(1){}
         * }
         */
        Thread(void (*task_code)(void *p_arg), void *p_arg, const char *name, uint32_t priority,
               size_t starck_size = DEFAULT_STACK_SIZE)
            : __thread_code(task_code), __p_arg(p_arg), __starck_size(starck_size) {
            strcpy(__name, name);
        }

        /**
         * @brief 当选择使用Thread作为基类定义派生类时，无需传入task_code线程代码
         * 需要在派生类中定义虚函数task_code，作为线程运行的主体，并在调用join后才
         * 真正的创建该线程，执行task_code里的内容。当task_code结束后，该线程将会被
         * 自动删除，即FreeRTOS删除任务
         * @param[in] name 线程名称
         * @param[in] starck_size 栈大小
         * @param[in] priority 线程优先值
         * @example
         * class TASK: public Thread
         * {
         *  public:
         *  TASK()：Thread("task",1,64){join();}
         *  virtual void task_code() override
         *  {
         *      printf("hello world\r\n");
         *  }
         * }
         */
        Thread(const char *name, uint32_t priority, size_t starck_size)
            : __priority(priority), __starck_size(starck_size) {
            strcpy(__name, name);
        }

        Thread(std::function<void()> handle, const char *name, uint32_t priority,
               size_t stack_size = DEFAULT_STACK_SIZE)
            : __thread_code_lambda(handle), __priority(priority), __starck_size(stack_size) {
            strcpy(__name, name);
        }

        ~Thread() {
            if (__join_flag) {
                erdp_if_rtos_task_delete(nullptr);
            }
        }

        void join() {
            if (!__join_flag) {
                __join_flag = 1;
                __handler = erdp_if_rtos_task_create(erdp_task_run, __name, __starck_size, this, __priority);
                erdp_assert(__handler != nullptr);
            }
        }

        void suspend(Thread *task) { erdp_if_rtos_task_suspend(task->get_thread_handler()); }

        void suspend() { erdp_if_rtos_task_suspend(__handler); }

        void resume(Thread *task) { erdp_if_rtos_task_resume(task->get_thread_handler()); }

        void resume() { erdp_if_rtos_task_resume(__handler); }

        void kill(OS_TaskHandle handler = nullptr) { erdp_if_rtos_task_delete(handler); }

        OS_TaskHandle get_thread_handler() { return __handler; }

        static void delay_ms(uint32_t ms) { erdp_if_rtos_delay_ms(ms); }

        static void start_scheduler() { erdp_if_rtos_start_scheduler(); }

        static uint32_t get_system_1ms_ticks() { return erdp_if_rtos_get_1ms_timestamp(); }

        virtual void thread_code() {
            if (__thread_code) {
                __thread_code(__p_arg);
            }
            if (__thread_code_lambda) {
                __thread_code_lambda();
            }
        }

       private:
        void (*__thread_code)(void *p_arg) = nullptr;
        std::function<void()> __thread_code_lambda = nullptr;
        void *__p_arg;
        char __name[configMAX_TASK_NAME_LEN + 1];
        uint32_t __priority;
        size_t __starck_size;
        OS_TaskHandle __handler;
        uint8_t __join_flag = 0;

        static OS_TaskHandle __main_task;
        static void main_thread(void *parm);
    };

#else    // ERDP_ENABLE_RTOS
    // 全局默认堆(需先初始化)
    extern Heap4 *default_heap;

#endif    // ERDP_ENABLE_RTOS

}    // namespace erdp

#endif
