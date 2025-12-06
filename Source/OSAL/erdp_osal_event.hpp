#ifndef __ERDP_OSAL_EVENT_HPP__
#define __ERDP_OSAL_EVENT_HPP__
#include "erdp_if_rtos.h"

namespace erdp
{
#ifdef ERDP_ENABLE_RTOS
    class Event
    {

    public:
        Event() : __handler(erdp_if_rtos_event_create()) {}

        Event(const Event &) = delete;
        Event &operator=(const Event &) = delete;

        ~Event()
        {
            vEventGroupDelete(__handler);
        }
        OS_EventBits set(OS_EventBits bits_to_set)
        {
            return erdp_if_rtos_set_event_bits(__handler, bits_to_set);
        }

        OS_EventBits clear(OS_EventBits bits_to_clear)
        {
            return erdp_if_rtos_clear_event_bits(__handler, bits_to_clear);
        }

        OS_EventBits get()
        {
            return erdp_if_rtos_get_event_bits(__handler);
        }

        OS_EventBits wait(OS_EventBits bits_to_wait, uint32_t ticks_to_wait = OS_WAIT_FOREVER, bool wait_for_all = true)
        {
            return xEventGroupWaitBits(__handler, bits_to_wait, false, wait_for_all, ticks_to_wait);
        }

        OS_EventBits sync(OS_EventBits bits_to_set, OS_EventBits bits_wait_for, uint32_t ticks_to_wait = OS_WAIT_FOREVER)
        {
            return erdp_if_rtos_event_sync(__handler, bits_to_set, bits_wait_for, ticks_to_wait);
        }

    private:
        OS_Event __handler;
    };
#endif // ERDP_ENABLE_RTOS
} // namespace erdp

#endif

