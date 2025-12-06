#ifndef __ERDP_OSAL_TIMER_HPP__
#define __ERDP_OSAL_TIMER_HPP__
#include "erdp_if_rtos.h"
#include <functional>

namespace erdp {
#ifdef ERDP_ENABLE_RTOS
    class Timer {
       public:
        Timer(std::function<void()> callback, const char *name, uint32_t period_ms, bool auto_reload = true)
            : __usr_timer_func(callback) {
            __handler = erdp_if_rtos_timer_create(name, period_ms, auto_reload, __timer_callback, this);
        }

        ~Timer() { erdp_if_rtos_timer_delete(__handler); }

        bool start() { return erdp_if_rtos_timer_start(__handler); }

        bool stop() { return erdp_if_rtos_timer_stop(__handler); }

        bool set_period(uint32_t period_ms) { return erdp_if_rtos_timer_set_period(__handler, period_ms); }

       private:
        static void __timer_callback(void *parm) {
            Timer *timer = static_cast<Timer *>(parm);
            timer->__usr_timer_func();
        }
        std::function<void()> __usr_timer_func;
        OS_Timer __handler;
    };
#endif    // ERDP_ENABLE_RTOS
}    // namespace erdp

#endif

