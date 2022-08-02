# uLog
Lightweight logging for embedded microcontrollers

## About uLog

uLog provides a structured logging mechanism for embedded microcontrollers or any system with limited resources.  
It inherits the some concepts behind the popular Log4c and Log4j platforms, but with lower overhead.

Some features of uLog:
* uLog is easy to incorporate into nearly any environment, comprising one header file and one source file, and is written in pure C.
* uLog provides familiar severity levels (CRITICAL, ERROR, WARNING, INFO, DEBUG, TRACE).
* uLog supports multiple user-defined outputs (console, log file, in-memory buffer, etc), each with its own reporting threshold level.
* uLog is "aggressively standalone" with minimal dependencies, requiring only stdio.h, string.h and stdarg.h.  
* uLog gets out of your way when you're not using it: if ULOG_ENABLED is undefined at compile time, no logging code is generated.
* uLog is well tested.  See the accompanying ulog_test.c file for details.

## A quick intro by example:

```
#include <stdio.h>
#include "ulog.h"

// To use uLog, you must define a function to process logging messages. It can
// write the messages to a console, to a file, to an in-memory buffer: the
// choice is yours.  And you get to choose the format of the message.  
//
// The following example prints to the console.  
//
// One caveat: msg is a static string and will be over-written at the next call
// to ULOG.  This means you may print it or copy it, but saving a pointer to it
// will lead to confusion and astonishment.
//
void my_console_logger(ulog_level_t severity, const char *msg) {
     printf("%s [%s]: %s\n",
         get_timestamp(),    // user defined function
         ulog_level_name(severity),
         msg);
}

int main() {
    int arg = 42;

 #ifdef ULOG_ENABLED
    ulog_init();

    // log messages with a severity of WARNING or higher to the console.  The
    // user must supply a method for my_console_logger, e.g. along the lines
    // of what is shown above.
    ulog_subscribe(my_console_logger, ULOG_WARNING_LEVEL);

    // log messages with a severity of DEBUG or higher to a file.  The user must
    // provide a method for my_file_logger (not shown here).
    ulog_subscribe(my_file_logger, ULOG_DEBUG_LEVEL);
#endif

    ULOG_INFO("Info, arg=%d", arg);        // logs to file but not console
    ULOG_CRITICAL("Critical, arg=%d", arg);  // logs to file and console

#ifdef ULOG_ENABLED
    // dynamically change the threshold for a specific logger
    ulog_subscribe(my_console_logger, ULOG_INFO_LEVEL);
#endif

    ULOG_INFO("Info, arg=%d", arg);          // logs to file and console

#ifdef ULOG_ENABLED
    // remove a logger
    ulog_unsubscribe(my_file_logger);
#endif

    ULOG_INFO("Info, arg=%d", arg);          // logs to console only
}
```

## Questions?  Comments?  Improvements?

Comments and pull requests are welcome in https://github.com/rdpoor/ulog/issues

- R Dunbar Poor
  June 2019
