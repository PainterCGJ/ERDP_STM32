#ifndef ERDP_HAL_CONFIG_H
#define ERDP_HAL_CONFIG_H

/* ================================< user config >================================ */

#define ERDP_CONFIG_HEAP_SIZE ((size_t)(10 * 1024))

#define ERDP_CONFIG_ASSERT_ENABLED 1

#define ERDP_CONFIG_RTOS_ENABLED 1

#define ERDP_CONFIG_MAIN_THREAD_STACK_SIZE (1024)
/* =============================< end of user config >============================ */

#if ERDP_CONFIG_RTOS_ENABLED == 1
#define ERDP_ENABLE_RTOS 
#endif

#if ERDP_CONFIG_ASSERT_ENABLED == 1
#define ERDP_ENABLE_ASSERT
#endif

#endif
