/**
MIT License

Copyright (c) 2019 R. Dunbar Poor <rdpoor@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/**
 * \file ulog_test.c
 *
 * \brief unit testing for uLog logging mechanism
 */

#include "ulog.h"
#include <assert.h>
#include <string.h>

int fn_calls[6];

void logger_fn0(ulog_level_t severity, char *msg) {
  assert(strcmp(msg, "Hello!") == 0);
  fn_calls[0]++;
}

void logger_fn1(ulog_level_t severity, char *msg) {
  assert(strcmp(msg, "Hello!") == 0);
  fn_calls[1]++;
}

void logger_fn2(ulog_level_t severity, char *msg) {
  assert(strcmp(msg, "Hello!") == 0);
  fn_calls[2]++;
}

void logger_fn3(ulog_level_t severity, char *msg) {
  assert(strcmp(msg, "Hello!") == 0);
  fn_calls[3]++;
}

void logger_fn4(ulog_level_t severity, char *msg) {
  assert(strcmp(msg, "Hello!") == 0);
  fn_calls[4]++;
}

void logger_fn5(ulog_level_t severity, char *msg) {
  assert(strcmp(msg, "Hello!") == 0);
  fn_calls[5]++;
}

void logger_fn6(ulog_level_t severity, char *msg) {
  // never actually called
}

void ulog_test() {
  ulog_init();
  memset(fn_calls, 0, sizeof(fn_calls));

  assert(ulog_subscribe(logger_fn0, ULOG_TRACE_LEVEL) == ULOG_ERR_NONE);
  assert(ulog_subscribe(logger_fn1, ULOG_DEBUG_LEVEL) == ULOG_ERR_NONE);
  assert(ulog_subscribe(logger_fn2, ULOG_INFO_LEVEL) == ULOG_ERR_NONE);
  assert(ulog_subscribe(logger_fn3, ULOG_WARNING_LEVEL) == ULOG_ERR_NONE);
  assert(ulog_subscribe(logger_fn4, ULOG_ERROR_LEVEL) == ULOG_ERR_NONE);
  assert(ulog_subscribe(logger_fn5, ULOG_CRITICAL_LEVEL) == ULOG_ERR_NONE);

  assert(ulog_subscribe(logger_fn6, ULOG_TRACE_LEVEL) == ULOG_ERR_SUBSCRIBERS_EXCEEDED);

  ULOG_TRACE("Hello!");
  ULOG_DEBUG("Hello!");
  ULOG_INFO("Hello!");
  ULOG_WARNING("Hello!");
  ULOG_ERROR("Hello!");
  ULOG_CRITICAL("Hello!");
  ULOG_ALWAYS("Hello!");

  assert(fn_calls[0] == 7);  // logger_fn0 is at trace level: all messages
  assert(fn_calls[1] == 6);
  assert(fn_calls[2] == 5);
  assert(fn_calls[3] == 4);
  assert(fn_calls[4] == 3);
  assert(fn_calls[5] == 2);  // logger_fn5 receives critical and alwasy only

  // ULOG with explicit severity parameter
  ULOG(ULOG_INFO_LEVEL, "Hello!");

  assert(fn_calls[0] == 8);  // logger_fn0 is at trace level: all messages
  assert(fn_calls[1] == 7);
  assert(fn_calls[2] == 6);  // logger_fn2 and lower get info messages
  assert(fn_calls[3] == 4);  // logger_fn3 and higher don't get info messages
  assert(fn_calls[4] == 3);
  assert(fn_calls[5] == 2);  // logger_fn5 receives critical msgs only

  // reset counters.  Test reassigning levels...
  memset(fn_calls, 0, sizeof(fn_calls));

  assert(ulog_subscribe(logger_fn0, ULOG_CRITICAL_LEVEL) == ULOG_ERR_NONE);
  assert(ulog_subscribe(logger_fn1, ULOG_ERROR_LEVEL) == ULOG_ERR_NONE);
  assert(ulog_subscribe(logger_fn2, ULOG_WARNING_LEVEL) == ULOG_ERR_NONE);
  assert(ulog_subscribe(logger_fn3, ULOG_INFO_LEVEL) == ULOG_ERR_NONE);
  assert(ulog_subscribe(logger_fn4, ULOG_DEBUG_LEVEL) == ULOG_ERR_NONE);
  assert(ulog_subscribe(logger_fn5, ULOG_TRACE_LEVEL) == ULOG_ERR_NONE);

  assert(ulog_subscribe(logger_fn6, ULOG_TRACE_LEVEL) == ULOG_ERR_SUBSCRIBERS_EXCEEDED);

  ULOG_TRACE("Hello!");
  ULOG_DEBUG("Hello!");
  ULOG_INFO("Hello!");
  ULOG_WARNING("Hello!");
  ULOG_ERROR("Hello!");
  ULOG_CRITICAL("Hello!");
  ULOG_ALWAYS("Hello!");

  assert(fn_calls[0] == 2);  // logger_fn0 receives critical and always msgs
  assert(fn_calls[1] == 3);
  assert(fn_calls[2] == 4);
  assert(fn_calls[3] == 5);
  assert(fn_calls[4] == 6);
  assert(fn_calls[5] == 7);  // logger_fn5 is at trace level: all messages

  // reset counters.  Test unsubscribe
  memset(fn_calls, 0, sizeof(fn_calls));

  assert(ulog_unsubscribe(logger_fn0) == ULOG_ERR_NONE);
  assert(ulog_unsubscribe(logger_fn1) == ULOG_ERR_NONE);
  assert(ulog_unsubscribe(logger_fn2) == ULOG_ERR_NONE);
  assert(ulog_unsubscribe(logger_fn3) == ULOG_ERR_NONE);
  assert(ulog_unsubscribe(logger_fn4) == ULOG_ERR_NONE);
  // leave logger_fn5 subscribed
  // assert(ulog_unsubscribe(logger_fn5, ULOG_TRACE) == ULOG_ERR_NONE);

  assert(ulog_unsubscribe(logger_fn6) == ULOG_ERR_NOT_SUBSCRIBED);

  ULOG_TRACE("Hello!");
  ULOG_DEBUG("Hello!");
  ULOG_INFO("Hello!");
  ULOG_WARNING("Hello!");
  ULOG_ERROR("Hello!");
  ULOG_CRITICAL("Hello!");
  ULOG_ALWAYS("Hello!");

  assert(fn_calls[0] == 0);  // not subscribed...
  assert(fn_calls[1] == 0);
  assert(fn_calls[2] == 0);
  assert(fn_calls[3] == 0);
  assert(fn_calls[4] == 0);
  assert(fn_calls[5] == 7);  // logger_fn5 is at trace level: all messages

  // ulog_level_name
  assert(strcmp(ulog_level_name(ULOG_TRACE_LEVEL), "TRACE") == 0);
  assert(strcmp(ulog_level_name(ULOG_DEBUG_LEVEL), "DEBUG") == 0);
  assert(strcmp(ulog_level_name(ULOG_INFO_LEVEL), "INFO") == 0);
  assert(strcmp(ulog_level_name(ULOG_WARNING_LEVEL), "WARNING") == 0);
  assert(strcmp(ulog_level_name(ULOG_ERROR_LEVEL), "ERROR") == 0);
  assert(strcmp(ulog_level_name(ULOG_CRITICAL_LEVEL), "CRITICAL") == 0);
  assert(strcmp(ulog_level_name(ULOG_ALWAYS_LEVEL), "ALWAYS") == 0);

}
