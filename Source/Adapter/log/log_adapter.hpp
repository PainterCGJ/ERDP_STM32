#ifndef __LOG_ADAPTER_HPP__
#define __LOG_ADAPTER_HPP__
#include "erdp_hal_uart.hpp"
#include "erdp_osal.hpp"
#include "log.hpp"
#include "printf.h"
#include "thread_config.h"

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

class Logger : public LogInterface {
   public:
    Logger() : log_thread([this]() { this->log_thread_code(); }, "LoggerThread", LOG_PRIO, LOG_THREAD_STACK_SIZE) {
        set_pattern("[%t][%L][%M]: %m\n");
        set_tag(LogLevel::ERROR, "\033[31mERROR\033[0m");
        set_tag(LogLevel::WARN, "\033[33mWARN\033[0m");
        set_tag(LogLevel::DEBUG, "\033[37mDEBUG\033[0m");
        set_tag(LogLevel::INFO, "\033[94mINFO\033[0m");
        set_tag(LogLevel::TRACE, "\033[90mTRACE\033[0m");

        logger.set_level(LogLevel::TRACE);
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

    static void set_pattern(const std::string &pattern) { logger.set_pattern(pattern); }
    void log_output(const uint8_t *message, uint32_t len) override;
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

    static void set_tag(LogLevel level, const std::string &tag) { logger.set_tag(level, tag); }

   private:
    static void cal_time(char *str);
    static Log<Logger> logger;
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
