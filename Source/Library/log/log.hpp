#ifndef __LOG_HPP__
#define __LOG_HPP__

// C语言实现的日志库头文件
// 直接包含C语言版本的log.h
#include "log.h"

#ifdef __cplusplus
extern "C" {
#endif

// 声明用户自定义的日志输出函数
// 此函数需要在 log_adapter.cpp 中实现
// 函数类型为 log_output_func_t（已在 log.h 中定义）
void log_output(const uint8_t *message, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif  // __LOG_HPP__
