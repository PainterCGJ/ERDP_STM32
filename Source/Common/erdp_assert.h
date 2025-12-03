#ifndef ERDP_ASSERT_H
#define ERDP_ASSERT_H

#include "erdp_config.h"
#ifdef ERDP_ENABLE_ASSERT
#ifndef erdp_assert
#define erdp_assert(condition)                                                \
    do                                                                        \
    {                                                                         \
        if (!(condition))                                                     \
        {                                                                     \
            /* Handle the assertion failure */                                \
            /* For example, print an error message or trigger a breakpoint */ \
            /* You can also implement a custom assertion handler */           \
            printf("Assertion failed: %s, line %d\n", __FILE__, __LINE__);    \
            while (1)                                                         \
            {                                                                 \
            }                                                                 \
        }                                                                     \
    } while (0)
#endif // erdp_assert(condition)
#else  // ERDP_ENABLE_ASSERT
#define erdp_assert(condition) (void)0
#endif // ERDP_ENABLE_ASSERT
#endif // ERDP_ASSERT_H
