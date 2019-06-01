# ulog
Lightweight logging for embedded microcontrollers

## A quick intro by example:

```
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
void my_console_logger(ulog_severity_t severity, const char *msg) {
     printf("%s [%s]: %s\n",
         get_timestamp(),    // user defined function
         ulog_severity_name(severity),
         msg);
}

int main() {
    ULOG_INIT();

    // log to the console messages that are WARNING or more severe.  You
    // can re-subscribe at any point to change the severity level.
    ULOG_SUBSCRIBE(my_console_logger, ULOG_WARNING);
 
    // log to a file messages that are DEBUG or more severe
    ULOG_SUBSCRIBE(my_file_logger, ULOG_DEBUG);

    int arg = 42;
    ULOG(ULOG_INFO, "Arg is %d", arg);  // logs to file but not console
}
```

## Questions?  Comments?

I welcome comments and pull requests in https://github.com/rdpoor/ulog/issues

- R Dunbar Poor
  June 2019
  
