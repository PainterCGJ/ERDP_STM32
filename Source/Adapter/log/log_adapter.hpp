#ifndef __LOG_ADAPTER_HPP__
#define __LOG_ADAPTER_HPP__

#include <stdio.h>
#include <string>
#include <stdint.h>
#include <cstdarg>

#include "erdp_hal_uart.hpp"
#include "erdp_osal.hpp"
#include "log.h"
#include "thread_config.h"

// 前向声明用户自定义的日志输出函数
extern "C" {
    void log_output(const uint8_t *message, uint32_t len);
}

#define LOGGER_SINGLE_QUEUE_MODE 0
#define LOGGER_MULTI_QUEUE_MODE  1
#define LOGGER_QUEUE_MODE        LOGGER_MULTI_QUEUE_MODE

#if (LOGGER_QUEUE_MODE == LOGGER_SINGLE_QUEUE_MODE)
#define LOG_MESSAGE_LEN 5 * 1024
#elif (LOGGER_QUEUE_MODE == LOGGER_MULTI_QUEUE_MODE)

#define LOG_MESSAGE_NUM 50
#define LOG_MESSAGE_LEN 102

#define MSG_EVENT_IDLE    (OS_EventBits)((uint32_t)1 << 0)
#define MSG_EVENT_WAITING (OS_EventBits)((uint32_t)1 << 1)
#endif

class Logger {
   public:
    Logger() : log_thread([this]() { this->log_thread_code(); }, "LoggerThread", LOG_PRIO, LOG_THREAD_STACK_SIZE) {
        log_init(&logger, log_output);
        log_set_pattern(&logger, "[%t][%L][%M]: %m\n");
        log_set_tag(&logger, LOG_LEVEL_ERROR, "\033[31mERROR\033[0m");
        log_set_tag(&logger, LOG_LEVEL_WARN, "\033[33mWARN\033[0m");
        log_set_tag(&logger, LOG_LEVEL_DEBUG, "\033[37mDEBUG\033[0m");
        log_set_tag(&logger, LOG_LEVEL_INFO, "\033[94mINFO\033[0m");
        log_set_tag(&logger, LOG_LEVEL_TRACE, "\033[90mTRACE\033[0m");

        log_set_level(&logger, LOG_LEVEL_TRACE);
        log_en = true;
    };
    ~Logger() = default;
    static bool log_en;

    static void t(const char *module, const char *format, ...);
    static void i(const char *module, const char *format, ...);
    static void d(const char *module, const char *format, ...);
    static void w(const char *module, const char *format, ...);
    static void e(const char *module, const char *format, ...);

    static void t(const char *module, const uint8_t *data, uint32_t len);

    static void set_pattern(const std::string &pattern) { log_set_pattern(&logger, pattern.c_str()); }
    static void log_output_impl(const uint8_t *message, uint32_t len);
    void start() {
#if (LOGGER_QUEUE_MODE == LOGGER_SINGLE_QUEUE_MODE)
        log_queue.init(LOG_MESSAGE_LEN);
#elif (LOGGER_QUEUE_MODE == LOGGER_MULTI_QUEUE_MODE)
        order_queue.init(LOG_MESSAGE_NUM);
        for (uint8_t i = 0; i < LOG_MESSAGE_NUM; i++) {
            message_handler[i].log_queue.init(LOG_MESSAGE_LEN);
            message_handler[i].event.set(MSG_EVENT_IDLE);
        }
#endif
        log_thread.join();
    }

    static void set_tag(log_level_t level, const std::string &tag) { log_set_tag(&logger, level, tag.c_str()); }

   private:
    static void cal_time(char *str);
    static log_t logger;
    void log_thread_code();
#if (LOGGER_QUEUE_MODE == LOGGER_SINGLE_QUEUE_MODE)
    static erdp::Queue<uint8_t> log_queue;
    static erdp::Mutex log_mutex;
#elif (LOGGER_QUEUE_MODE == LOGGER_MULTI_QUEUE_MODE)
    typedef struct MsgHandler {
        erdp::Event event;
        erdp::Queue<uint8_t> log_queue;
    } MsgHandler;
    static MsgHandler message_handler[LOG_MESSAGE_NUM];
    static erdp::Queue<uint8_t> order_queue;
#endif
    erdp::Thread log_thread;
};

#endif
