#ifndef TOPAZ_TOPAZ_H
#define TOPAZ_TOPAZ_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#ifndef __FILENAME__
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#endif

// * To use this library, add the following lines to CMakeLists.txt:
// e.g. add_subdirectory(path/to/topaz)
//      target_link_libraries(my_app PRIVATE topaz)

// File - Function (Line) - Time: Message
#define DEFAULT_LOG_FORMAT "[%s] (%s) - [%s]: %s%s"
// * To overwrite the default log_format include the following line to CMakeLists.txt:
// There should also always be a total of 5 %s specifiers in the format
// e.g. add_definitions(-DLOG_FORMAT="[%s] - [%s] - [%s]: %s%s")

// * These values set the order in which the elements are substituted into the log format
// Setting them to 0 will not display them.
// e.g. add_definitions(-DLOG_FORMAT_FILE_INDEX=0)
#ifndef LOG_FORMAT
#define LOG_FORMAT DEFAULT_LOG_FORMAT
#endif
#ifndef LOG_FORMAT_FILE_INDEX
#define LOG_FORMAT_FILE_INDEX 1
#endif
#ifndef LOG_FORMAT_FUNCTION_INDEX
#define LOG_FORMAT_FUNCTION_INDEX 0
#endif
#ifndef LOG_FORMAT_LINE_INDEX
#define LOG_FORMAT_LINE_INDEX 2
#endif
#ifndef LOG_FORMAT_TIME_INDEX
#define LOG_FORMAT_TIME_INDEX 3
#endif

typedef enum {
    DEBUG,
    INFO,
    WARN,
    ERROR
} Level;

extern Level application_logging_level;
void set_application_logging_level(Level level);

void log_message(Level level, const char* file, int line, const char* func, char* format, ...);

#define LOG_DEBUG(format, ...) log_message(DEBUG, __FILENAME__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...)  log_message(INFO,  __FILENAME__, __LINE__, "", format, ##__VA_ARGS__)
#define LOG_WARN(format, ...)  log_message(WARN,  __FILENAME__, __LINE__, __func__, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) log_message(ERROR, __FILENAME__, __LINE__, __func__, format, ##__VA_ARGS__)

#endif //TOPAZ_TOPAZ_H
