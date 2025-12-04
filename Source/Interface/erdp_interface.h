#ifndef ERDP_INTERFACE_H
#define ERDP_INTERFACE_H

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include <stdint.h>
// #include <stdio.h>
#include <stdbool.h>
#include "erdp_assert.h"
    typedef enum
    {
        ERDP_RESET = 0,
        ERDP_SET,
    } ERDP_Status_t;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // ERDP_INTERFACE_H
