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
void my_console_logger(ulog_level_t severity, char *msg)
{
  printf("console: %s [%s]: %s\n",
         "time", // user defined function
#if defined ULOG_ENABLED
         ULOG_LEVEL_NAME(severity),
#endif
         msg);
}

void my_file_logger(ulog_level_t severity, char *msg)
{
  printf("file: %s [%s]: %s\n",
         "time", // user defined function
#if defined ULOG_ENABLED
         ULOG_LEVEL_NAME(severity),
#endif
         msg);
}

int main()
{
  int arg = 42;

  ULOG_INIT();

  // log messages with a severity of WARNING or higher to the console.  The
  // user must supply a method for my_console_logger, e.g. along the lines
  // of what is shown above.
  ULOG_SUBSCRIBE(my_console_logger, ULOG_DEBUG_LEVEL);

  // log messages with a severity of DEBUG or higher to a file.  The user must
  // provide a method for my_file_logger (not shown here).
  ULOG_SUBSCRIBE(my_file_logger, ULOG_WARNING_LEVEL);

  ULOG_INFO("Info, arg=%d", arg);         // logs to file but not console
  ULOG_CRITICAL("Critical, arg=%d", arg); // logs to file and console

  // dynamically change the threshold for a specific logger
  ULOG_SUBSCRIBE(my_console_logger, ULOG_INFO_LEVEL);

  ULOG_INFO("Info, arg=%d", arg); // logs to file and console

  // remove a logger
  ULOG_UNSUBSCRIBE(my_file_logger);

  ULOG_INFO("Info, arg=%d", arg); // logs to console only
}
