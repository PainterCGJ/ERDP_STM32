#ifndef __LOG_HPP__
#define __LOG_HPP__

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>

#include "printf.h"

class LogInterface {
   public:
    LogInterface() = default;
    ~LogInterface() = default;
    virtual void log_output(const uint8_t *message, uint32_t len) = 0;
};

enum class LogLevel : uint8_t { ERROR = 0, WARN, DEBUG, INFO, TRACE };

template <typename Interface, size_t MSG_MAX_SIZE = 256>
class Log {
   public:
    Log() = default;
    ~Log() = default;

    LogLevel level = LogLevel::TRACE;

    void set_level(LogLevel new_level) { level = new_level; }
    void set_tag(LogLevel level, const std::string &tag) {
        switch (level) {
            case LogLevel::TRACE:
                trace_tag = tag;
                break;
            case LogLevel::DEBUG:
                debug_tag = tag;
                break;
            case LogLevel::INFO:
                info_tag = tag;
                break;
            case LogLevel::WARN:
                warn_tag = tag;
                break;
            case LogLevel::ERROR:
                error_tag = tag;
                break;
        }
    }

    // 设置日志输出格式的函数
    void set_pattern(const std::string &pattern) { log_pattern = pattern; }

    void trace(const char *time, const char *module, const char *format, ...) {
        if (level < LogLevel::TRACE) return;
        va_list args;
        va_start(args, format);
        vsnprintf(message, sizeof(message), format, args);
        log_with_level(time, trace_tag.c_str(), module, message);
        va_end(args);
    }

    void vtrace(const char *time, const char *module, const char *format,
                va_list args) {
        if (level < LogLevel::TRACE) return;
        vsnprintf(message, sizeof(message), format, args);
        log_with_level(time, trace_tag.c_str(), module, message);
    }

    void debug(const char *time, const char *module, const char *format, ...) {
        if (level < LogLevel::DEBUG) return;
        va_list args;
        va_start(args, format);
        vsnprintf(message, sizeof(message), format, args);
        log_with_level(time, debug_tag.c_str(), module, message);
        va_end(args);
    }

    void vdebug(const char *time, const char *module, const char *format,
                va_list args) {
        if (level < LogLevel::DEBUG) return;
        vsnprintf(message, sizeof(message), format, args);
        log_with_level(time, debug_tag.c_str(), module, message);
    }

    void info(const char *time, const char *module, const char *format, ...) {
        if (level < LogLevel::INFO) return;
        va_list args;
        va_start(args, format);
        vsnprintf(message, sizeof(message), format, args);
        log_with_level(time, info_tag.c_str(), module, message);
        va_end(args);
    }

    void vinfo(const char *time, const char *module, const char *format,
               va_list args) {
        if (level < LogLevel::INFO) return;
        vsnprintf(message, sizeof(message), format, args);
        log_with_level(time, info_tag.c_str(), module, message);
    }

    void warn(const char *time, const char *module, const char *format, ...) {
        if (level < LogLevel::WARN) return;
        va_list args;
        va_start(args, format);
        vsnprintf(message, sizeof(message), format, args);
        log_with_level(time, warn_tag.c_str(), module, message);
        va_end(args);
    }

    void vwarn(const char *time, const char *module, const char *format,
               va_list args) {
        if (level < LogLevel::WARN) return;
        vsnprintf(message, sizeof(message), format, args);
        log_with_level(time, warn_tag.c_str(), module, message);
    }

    void error(const char *time, const char *module, const char *format, ...) {
        if (level < LogLevel::ERROR) return;
        va_list args;
        va_start(args, format);
        vsnprintf(message, sizeof(message), format, args);
        log_with_level(time, error_tag.c_str(), module, message);
        va_end(args);
    }

    void verror(const char *time, const char *module, const char *format,
                va_list args) {
        if (level < LogLevel::ERROR) return;
        vsnprintf(message, sizeof(message), format, args);
        log_with_level(time, error_tag.c_str(), module, message);
    }

   private:
    std::string log_pattern;            // 存储日志输出格式
    std::string info_tag = "INFO";      // 默认日志级别
    std::string warn_tag = "WARN";      // 默认警告级别
    std::string error_tag = "ERROR";    // 默认错误级别
    std::string trace_tag = "TRACE";    // 默认跟踪级别
    std::string debug_tag = "DEBUG";    // 默认调试级别

    char message[MSG_MAX_SIZE];
    char full_message[MSG_MAX_SIZE];

    void log_with_level(const char *time, const char *level, const char *module,
                        const char *message) {
        memset(full_message, 0, sizeof(full_message));

        const char *pos = log_pattern.c_str();
        while (*pos) {
            if (*pos == '\033')    // 检测ANSI转义序列开始
            {
                // 处理ANSI转义序列
                const char *ansi_start = pos;
                while (*pos && (*pos != 'm' && *pos != 'H' && *pos != 'J' &&
                                *pos != 'K')) {
                    ++pos;
                }
                if (*pos) {
                    ++pos;    // 包含结束字符
                    size_t ansi_len = pos - ansi_start;
                    if (strlen(full_message) + ansi_len >= MSG_MAX_SIZE - 1) {
                        break;
                    }
                    strncat(full_message, ansi_start, ansi_len);
                    continue;
                }
            } else if (*pos == '%') {
                ++pos;
                if(*pos == 't') {
                    if (strlen(full_message) + strlen(time) >= MSG_MAX_SIZE - 1) {
                        break;
                    }
                    strcat(full_message, time);
                }
                else if (*pos == 'L') {
                    if (strlen(full_message) + strlen(level) >=
                        MSG_MAX_SIZE - 1) {
                        break;
                    }
                    strcat(full_message, level);
                } else if (*pos == 'M') {
                    if (strlen(full_message) + strlen(module) >=
                        MSG_MAX_SIZE - 1) {
                        break;
                    }
                    strcat(full_message, module);
                } else if (*pos == 'm') {
                    if (strlen(full_message) + strlen(message) >=
                        MSG_MAX_SIZE - 1) {
                        break;
                    }
                    strcat(full_message, message);
                } else {
                    if (strlen(full_message) + 2 >= MSG_MAX_SIZE - 1) {
                        break;
                    }
                    strcat(full_message, "%");
                    strncat(full_message, &pos[-1], 1);
                }
            } else {
                if (strlen(full_message) + 1 >= MSG_MAX_SIZE - 1) {
                    break;
                }
                strncat(full_message, pos, 1);
            }
            ++pos;
        }
        logger.log_output(reinterpret_cast<const uint8_t *>(full_message),
                          strlen(full_message));
    }
    // 获取日志接口实例并输出日志
    Interface logger;
};

#endif
