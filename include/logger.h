#include <stdio.h>
#include <time.h>

// Set current log level (0=ERROR, 1=WARN, 2=INFO, 3=DEBUG)
#define CURRENT_LOG_LEVEL 3

#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define ITALIC  "\033[3m"
#define UNDER   "\033[4m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[94m"
#define BG_BLUE "\033[44m"

#define LOG(level_str, level_num, fmt, col, ...) do { \
    if (level_num <= CURRENT_LOG_LEVEL) { \
        time_t now = time(NULL); \
        char buf[20]; \
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now)); \
        fprintf(stderr, col "[%s] %-7s %15s:%-4d " fmt " (%s)\n" RESET, \
                buf, level_str, __FILE__, __LINE__, ##__VA_ARGS__, __FUNCTION__); \
    } \
} while (0)

#define LOG_ERROR(fmt, ...) LOG("[ERROR]", 0, fmt, RED, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  LOG("[WARN]",  1, fmt, YELLOW, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  LOG("[INFO]",  2, fmt, GREEN, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) LOG("[DEBUG]", 3, fmt, BLUE, ##__VA_ARGS__)

// int main(void) {
//     int port = 8080;
//     LOG_INFO("Server starting on port %d", port);
//     LOG_DEBUG("Initializing worker threads");
//     return 0;
// }