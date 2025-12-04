#include "log.h"
#include <string.h>

void log_init(log_t *log, log_output_func_t output_func) {
    if (log == NULL || output_func == NULL) {
        return;
    }
    
    memset(log, 0, sizeof(log_t));
    log->level = LOG_LEVEL_TRACE;
    log->output_func = output_func;
    
    // 设置默认标签
    strncpy(log->info_tag, "INFO", sizeof(log->info_tag) - 1);
    strncpy(log->warn_tag, "WARN", sizeof(log->warn_tag) - 1);
    strncpy(log->error_tag, "ERROR", sizeof(log->error_tag) - 1);
    strncpy(log->trace_tag, "TRACE", sizeof(log->trace_tag) - 1);
    strncpy(log->debug_tag, "DEBUG", sizeof(log->debug_tag) - 1);
    
    // 设置默认格式
    strncpy(log->log_pattern, "[%t][%L][%M]: %m\n", sizeof(log->log_pattern) - 1);
}

void log_set_level(log_t *log, log_level_t level) {
    if (log == NULL) {
        return;
    }
    log->level = level;
}

void log_set_tag(log_t *log, log_level_t level, const char *tag) {
    if (log == NULL || tag == NULL) {
        return;
    }
    
    switch (level) {
        case LOG_LEVEL_TRACE:
            strncpy(log->trace_tag, tag, sizeof(log->trace_tag) - 1);
            log->trace_tag[sizeof(log->trace_tag) - 1] = '\0';
            break;
        case LOG_LEVEL_DEBUG:
            strncpy(log->debug_tag, tag, sizeof(log->debug_tag) - 1);
            log->debug_tag[sizeof(log->debug_tag) - 1] = '\0';
            break;
        case LOG_LEVEL_INFO:
            strncpy(log->info_tag, tag, sizeof(log->info_tag) - 1);
            log->info_tag[sizeof(log->info_tag) - 1] = '\0';
            break;
        case LOG_LEVEL_WARN:
            strncpy(log->warn_tag, tag, sizeof(log->warn_tag) - 1);
            log->warn_tag[sizeof(log->warn_tag) - 1] = '\0';
            break;
        case LOG_LEVEL_ERROR:
            strncpy(log->error_tag, tag, sizeof(log->error_tag) - 1);
            log->error_tag[sizeof(log->error_tag) - 1] = '\0';
            break;
    }
}

void log_set_pattern(log_t *log, const char *pattern) {
    if (log == NULL || pattern == NULL) {
        return;
    }
    strncpy(log->log_pattern, pattern, sizeof(log->log_pattern) - 1);
    log->log_pattern[sizeof(log->log_pattern) - 1] = '\0';
}

// 内部函数：根据格式字符串格式化日志消息
static void log_with_level(log_t *log, const char *time, const char *level, 
                           const char *module, const char *message) {
    if (log == NULL || log->output_func == NULL) {
        return;
    }
    
    memset(log->full_message, 0, sizeof(log->full_message));
    
    const char *pos = log->log_pattern;
    while (*pos) {
        if (*pos == '\033') {  // 检测ANSI转义序列开始
            // 处理ANSI转义序列
            const char *ansi_start = pos;
            while (*pos && (*pos != 'm' && *pos != 'H' && *pos != 'J' && *pos != 'K')) {
                ++pos;
            }
            if (*pos) {
                ++pos;  // 包含结束字符
                size_t ansi_len = pos - ansi_start;
                if (strlen(log->full_message) + ansi_len >= sizeof(log->full_message) - 1) {
                    break;
                }
                strncat(log->full_message, ansi_start, ansi_len);
                continue;
            }
        } else if (*pos == '%') {
            ++pos;
            if (*pos == 't') {
                if (strlen(log->full_message) + strlen(time) >= sizeof(log->full_message) - 1) {
                    break;
                }
                strcat(log->full_message, time);
            } else if (*pos == 'L') {
                if (strlen(log->full_message) + strlen(level) >= sizeof(log->full_message) - 1) {
                    break;
                }
                strcat(log->full_message, level);
            } else if (*pos == 'M') {
                if (strlen(log->full_message) + strlen(module) >= sizeof(log->full_message) - 1) {
                    break;
                }
                strcat(log->full_message, module);
            } else if (*pos == 'm') {
                if (strlen(log->full_message) + strlen(message) >= sizeof(log->full_message) - 1) {
                    break;
                }
                strcat(log->full_message, message);
            } else {
                if (strlen(log->full_message) + 2 >= sizeof(log->full_message) - 1) {
                    break;
                }
                strcat(log->full_message, "%");
                strncat(log->full_message, pos, 1);
            }
        } else {
            if (strlen(log->full_message) + 1 >= sizeof(log->full_message) - 1) {
                break;
            }
            strncat(log->full_message, pos, 1);
        }
        ++pos;
    }
    
    // 调用用户自定义的输出函数
    log->output_func((const uint8_t *)(log->full_message), strlen(log->full_message));
}

void log_trace(log_t *log, const char *time, const char *module, const char *format, ...) {
    if (log == NULL || log->level < LOG_LEVEL_TRACE) {
        return;
    }
    va_list args;
    va_start(args, format);
    vsnprintf(log->message, sizeof(log->message), format, args);
    log_with_level(log, time, log->trace_tag, module, log->message);
    va_end(args);
}

void log_vtrace(log_t *log, const char *time, const char *module, const char *format, va_list args) {
    if (log == NULL || log->level < LOG_LEVEL_TRACE) {
        return;
    }
    vsnprintf(log->message, sizeof(log->message), format, args);
    log_with_level(log, time, log->trace_tag, module, log->message);
}

void log_debug(log_t *log, const char *time, const char *module, const char *format, ...) {
    if (log == NULL || log->level < LOG_LEVEL_DEBUG) {
        return;
    }
    va_list args;
    va_start(args, format);
    vsnprintf(log->message, sizeof(log->message), format, args);
    log_with_level(log, time, log->debug_tag, module, log->message);
    va_end(args);
}

void log_vdebug(log_t *log, const char *time, const char *module, const char *format, va_list args) {
    if (log == NULL || log->level < LOG_LEVEL_DEBUG) {
        return;
    }
    vsnprintf(log->message, sizeof(log->message), format, args);
    log_with_level(log, time, log->debug_tag, module, log->message);
}

void log_info(log_t *log, const char *time, const char *module, const char *format, ...) {
    if (log == NULL || log->level < LOG_LEVEL_INFO) {
        return;
    }
    va_list args;
    va_start(args, format);
    vsnprintf(log->message, sizeof(log->message), format, args);
    log_with_level(log, time, log->info_tag, module, log->message);
    va_end(args);
}

void log_vinfo(log_t *log, const char *time, const char *module, const char *format, va_list args) {
    if (log == NULL || log->level < LOG_LEVEL_INFO) {
        return;
    }
    vsnprintf(log->message, sizeof(log->message), format, args);
    log_with_level(log, time, log->info_tag, module, log->message);
}

void log_warn(log_t *log, const char *time, const char *module, const char *format, ...) {
    if (log == NULL || log->level < LOG_LEVEL_WARN) {
        return;
    }
    va_list args;
    va_start(args, format);
    vsnprintf(log->message, sizeof(log->message), format, args);
    log_with_level(log, time, log->warn_tag, module, log->message);
    va_end(args);
}

void log_vwarn(log_t *log, const char *time, const char *module, const char *format, va_list args) {
    if (log == NULL || log->level < LOG_LEVEL_WARN) {
        return;
    }
    vsnprintf(log->message, sizeof(log->message), format, args);
    log_with_level(log, time, log->warn_tag, module, log->message);
}

void log_error(log_t *log, const char *time, const char *module, const char *format, ...) {
    if (log == NULL || log->level < LOG_LEVEL_ERROR) {
        return;
    }
    va_list args;
    va_start(args, format);
    vsnprintf(log->message, sizeof(log->message), format, args);
    log_with_level(log, time, log->error_tag, module, log->message);
    va_end(args);
}

void log_verror(log_t *log, const char *time, const char *module, const char *format, va_list args) {
    if (log == NULL || log->level < LOG_LEVEL_ERROR) {
        return;
    }
    vsnprintf(log->message, sizeof(log->message), format, args);
    log_with_level(log, time, log->error_tag, module, log->message);
}

