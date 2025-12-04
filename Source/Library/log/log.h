#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

// 日志级别枚举
typedef enum {
    LOG_LEVEL_ERROR = 0,
    LOG_LEVEL_WARN,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_TRACE
} log_level_t;

// 日志输出函数指针类型
// 用户需要实现此函数来定义日志的输出方式
typedef void (*log_output_func_t)(const uint8_t *message, uint32_t len);

// 日志结构体
typedef struct {
    log_level_t level;
    char log_pattern[128];
    char info_tag[32];
    char warn_tag[32];
    char error_tag[32];
    char trace_tag[32];
    char debug_tag[32];
    char message[256];
    char full_message[256];
    log_output_func_t output_func;  // 用户自定义的输出函数
} log_t;

// 默认消息最大大小
#define LOG_MSG_MAX_SIZE 256

/**
 * @brief 初始化日志结构体
 * @param log 日志结构体指针
 * @param output_func 日志输出函数指针（用户自定义）
 */
void log_init(log_t *log, log_output_func_t output_func);

/**
 * @brief 设置日志级别
 * @param log 日志结构体指针
 * @param level 日志级别
 */
void log_set_level(log_t *log, log_level_t level);

/**
 * @brief 设置日志标签
 * @param log 日志结构体指针
 * @param level 日志级别
 * @param tag 标签字符串
 */
void log_set_tag(log_t *log, log_level_t level, const char *tag);

/**
 * @brief 设置日志输出格式
 * @param log 日志结构体指针
 * @param pattern 格式字符串（支持 %t=时间, %L=级别, %M=模块, %m=消息）
 */
void log_set_pattern(log_t *log, const char *pattern);

/**
 * @brief 记录 TRACE 级别日志
 * @param log 日志结构体指针
 * @param time 时间字符串
 * @param module 模块名称
 * @param format 格式化字符串
 * @param ... 可变参数
 */
void log_trace(log_t *log, const char *time, const char *module, const char *format, ...);

/**
 * @brief 记录 TRACE 级别日志（使用 va_list）
 * @param log 日志结构体指针
 * @param time 时间字符串
 * @param module 模块名称
 * @param format 格式化字符串
 * @param args 可变参数列表
 */
void log_vtrace(log_t *log, const char *time, const char *module, const char *format, va_list args);

/**
 * @brief 记录 DEBUG 级别日志
 * @param log 日志结构体指针
 * @param time 时间字符串
 * @param module 模块名称
 * @param format 格式化字符串
 * @param ... 可变参数
 */
void log_debug(log_t *log, const char *time, const char *module, const char *format, ...);

/**
 * @brief 记录 DEBUG 级别日志（使用 va_list）
 * @param log 日志结构体指针
 * @param time 时间字符串
 * @param module 模块名称
 * @param format 格式化字符串
 * @param args 可变参数列表
 */
void log_vdebug(log_t *log, const char *time, const char *module, const char *format, va_list args);

/**
 * @brief 记录 INFO 级别日志
 * @param log 日志结构体指针
 * @param time 时间字符串
 * @param module 模块名称
 * @param format 格式化字符串
 * @param ... 可变参数
 */
void log_info(log_t *log, const char *time, const char *module, const char *format, ...);

/**
 * @brief 记录 INFO 级别日志（使用 va_list）
 * @param log 日志结构体指针
 * @param time 时间字符串
 * @param module 模块名称
 * @param format 格式化字符串
 * @param args 可变参数列表
 */
void log_vinfo(log_t *log, const char *time, const char *module, const char *format, va_list args);

/**
 * @brief 记录 WARN 级别日志
 * @param log 日志结构体指针
 * @param time 时间字符串
 * @param module 模块名称
 * @param format 格式化字符串
 * @param ... 可变参数
 */
void log_warn(log_t *log, const char *time, const char *module, const char *format, ...);

/**
 * @brief 记录 WARN 级别日志（使用 va_list）
 * @param log 日志结构体指针
 * @param time 时间字符串
 * @param module 模块名称
 * @param format 格式化字符串
 * @param args 可变参数列表
 */
void log_vwarn(log_t *log, const char *time, const char *module, const char *format, va_list args);

/**
 * @brief 记录 ERROR 级别日志
 * @param log 日志结构体指针
 * @param time 时间字符串
 * @param module 模块名称
 * @param format 格式化字符串
 * @param ... 可变参数
 */
void log_error(log_t *log, const char *time, const char *module, const char *format, ...);

/**
 * @brief 记录 ERROR 级别日志（使用 va_list）
 * @param log 日志结构体指针
 * @param time 时间字符串
 * @param module 模块名称
 * @param format 格式化字符串
 * @param args 可变参数列表
 */
void log_verror(log_t *log, const char *time, const char *module, const char *format, va_list args);

#ifdef __cplusplus
}
#endif

#endif  // __LOG_H__

