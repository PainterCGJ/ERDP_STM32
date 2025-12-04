#include "log_adapter.hpp"
#include "log.h"
#include "erdp_hal_uart.hpp"

log_t Logger::logger;
bool Logger::log_en = false;
#if (LOGGER_QUEUE_MODE == LOGGER_SINGLE_QUEUE_MODE)
erdp::Queue<uint8_t> Logger::log_queue;
erdp::Mutex Logger::log_mutex;
#elif (LOGGER_QUEUE_MODE == LOGGER_MULTI_QUEUE_MODE)
Logger::MsgHandler Logger::message_handler[LOG_MESSAGE_NUM];
erdp::Queue<uint8_t> Logger::order_queue;
#endif

// Logger 类的静态方法，用于处理日志输出
// 此方法可以访问 Logger 类的私有成员
void Logger::log_output_impl(const uint8_t *message, uint32_t len) {
    // 将日志信息输出到控制台
#if LOGGER_QUEUE_MODE == LOGGER_SINGLE_QUEUE_MODE
    log_mutex.lock();
    if (message != nullptr && len > 0) {
        while (len--) {
            log_queue.push(*message++);
        }
    }
    log_mutex.unlock();
#elif LOGGER_QUEUE_MODE == LOGGER_MULTI_QUEUE_MODE
    uint8_t i = 0;
    for (; i < LOG_MESSAGE_NUM; i++) {
        if (message_handler[i].event && (message_handler[i].event->get() & MSG_EVENT_IDLE)) {
            message_handler[i].event->clear(MSG_EVENT_IDLE);
            while (len > 0) {
                if (message_handler[i].log_queue.push(*message++)) {
                    len--;
                }
                else {
                    break;
                }
            }
            message_handler[i].event->set(MSG_EVENT_WAITING);
            order_queue.push(i);
            if (len == 0) {
                break;
            }
        }
    }
    if (i == LOG_MESSAGE_NUM) {
        // printf("log queue full\n");
    }
#endif
}

// 用户自定义的日志输出函数实现
// 此函数符合 log_output_func_t 类型定义
// 调用 Logger 类的静态方法来处理日志输出
void log_output(const uint8_t *message, uint32_t len) {
    Logger::log_output_impl(message, len);
}
void Logger::cal_time(char *str) {
    // 计算小时、分钟、秒和毫秒

    uint32_t timestamp = erdp::Thread::get_system_1ms_ticks();
    uint32_t hours = timestamp / (1000 * 60 * 60);
    timestamp %= (1000 * 60 * 60);
    uint32_t minutes = timestamp / (1000 * 60);
    timestamp %= (1000 * 60);
    uint32_t seconds = timestamp / 1000;
    uint32_t milliseconds = timestamp % 1000;

    // 使用 sprintf 将格式化的时间写入 str
    sprintf(str, "%02u:%02u:%02u.%03u", hours, minutes, seconds, milliseconds);
}

void Logger::i(const char *module, const char *format, ...) {
    if (!log_en) return;
    char time[20];
    cal_time(time);
    va_list args;
    va_start(args, format);
    log_vinfo(&logger, time, module, format, args);
    va_end(args);
}
void Logger::d(const char *module, const char *format, ...) {
    if (!log_en) return;
    char time[20];
    cal_time(time);
    va_list args;
    va_start(args, format);
    log_vdebug(&logger, time, module, format, args);
    va_end(args);
}
void Logger::e(const char *module, const char *format, ...) {
    if (!log_en) return;
    char time[20];
    cal_time(time);
    va_list args;
    va_start(args, format);
    log_verror(&logger, time, module, format, args);
    va_end(args);
}
void Logger::w(const char *module, const char *format, ...) {
    if (!log_en) return;
    char time[20];
    cal_time(time);
    va_list args;
    va_start(args, format);
    log_vwarn(&logger, time, module, format, args);
    va_end(args);
}
void Logger::t(const char *module, const char *format, ...) {
    if (!log_en) return;
    char time[20];
    cal_time(time);
    va_list args;
    va_start(args, format);
    log_vtrace(&logger, time, module, format, args);
    va_end(args);
}
// 打印十六进制数据
void Logger::t(const char *module, const uint8_t *data, uint32_t len) {
    if (!log_en) return;
    char time[20];
    cal_time(time);
    constexpr size_t LINE_BYTES = 32;
    char hex_line[LINE_BYTES * 3 + 1] = {0};    // 每字节两位+空格
    for (uint32_t i = 0; i < len; i += LINE_BYTES) {
        size_t line_len = (len - i > LINE_BYTES) ? LINE_BYTES : (len - i);
        char *p = hex_line;
        for (size_t j = 0; j < line_len; ++j) {
            sprintf(p, "%02X ", data[i + j]);
            p += 3;
        }
        *p = '\0';
        log_trace(&logger, time, module, "%s", hex_line);    // 使用C语言接口
    }
}
void Logger::log_thread_code() {
    uint8_t data;
    const erdp::UartDev *const &uart_dev = erdp::UartDev::get_debug_com();
    while (!uart_dev);
    while (true) {
#if LOGGER_QUEUE_MODE == LOGGER_SINGLE_QUEUE_MODE
        while (log_queue.pop(data)) {
            uart_dev->send(&data, 1);
        }
#elif LOGGER_QUEUE_MODE == LOGGER_MULTI_QUEUE_MODE
        uint8_t index = 0;
        while (order_queue.pop(index)) {
            message_handler[index].event->clear(MSG_EVENT_WAITING);
            while (message_handler[index].log_queue.pop(data)) {
                uart_dev->send(&data, 1);
            }
            message_handler[index].event->set(MSG_EVENT_IDLE);
        }
#endif
        erdp::Thread::delay_ms(2);    // 10ms
    }
}