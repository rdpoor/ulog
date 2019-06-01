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

void logger_fn0(ulog_severity_t severity, char *msg) {
  assert(strcmp(msg, "Hello!") == 0);
  fn_calls[0]++;
}

void logger_fn1(ulog_severity_t severity, char *msg) {
  assert(strcmp(msg, "Hello!") == 0);
  fn_calls[1]++;
}

void logger_fn2(ulog_severity_t severity, char *msg) {
  assert(strcmp(msg, "Hello!") == 0);
  fn_calls[2]++;
}

void logger_fn3(ulog_severity_t severity, char *msg) {
  assert(strcmp(msg, "Hello!") == 0);
  fn_calls[3]++;
}

void logger_fn4(ulog_severity_t severity, char *msg) {
  assert(strcmp(msg, "Hello!") == 0);
  fn_calls[4]++;
}

void logger_fn5(ulog_severity_t severity, char *msg) {
  assert(strcmp(msg, "Hello!") == 0);
  fn_calls[5]++;
}

void logger_fn6(ulog_severity_t severity, char *msg) {
  // never actually called
}

void ulog_test() {
  ULOG_INIT();
  memset(fn_calls, 0, sizeof(fn_calls));

  assert(ULOG_SUBSCRIBE(logger_fn0, ULOG_TRACE) == ULOG_ERR_NONE);
  assert(ULOG_SUBSCRIBE(logger_fn1, ULOG_DEBUG) == ULOG_ERR_NONE);
  assert(ULOG_SUBSCRIBE(logger_fn2, ULOG_INFO) == ULOG_ERR_NONE);
  assert(ULOG_SUBSCRIBE(logger_fn3, ULOG_WARNING) == ULOG_ERR_NONE);
  assert(ULOG_SUBSCRIBE(logger_fn4, ULOG_ERROR) == ULOG_ERR_NONE);
  assert(ULOG_SUBSCRIBE(logger_fn5, ULOG_CRITICAL) == ULOG_ERR_NONE);

  assert(ULOG_SUBSCRIBE(logger_fn6, ULOG_TRACE) == ULOG_ERR_SUBSCRIBERS_EXCEEDED);

  ULOG(ULOG_TRACE, "Hello!");
  ULOG(ULOG_DEBUG, "Hello!");
  ULOG(ULOG_INFO, "Hello!");
  ULOG(ULOG_WARNING, "Hello!");
  ULOG(ULOG_ERROR, "Hello!");
  ULOG(ULOG_CRITICAL, "Hello!");

  assert(fn_calls[0] == 6);  // logger_fn0 is at trace level: all messages
  assert(fn_calls[1] == 5);
  assert(fn_calls[2] == 4);
  assert(fn_calls[3] == 3);
  assert(fn_calls[4] == 2);
  assert(fn_calls[5] == 1);  // logger_fn5 receives critical msgs only

  // reset counters.  Test reassigning levels...
  memset(fn_calls, 0, sizeof(fn_calls));

  assert(ULOG_SUBSCRIBE(logger_fn0, ULOG_CRITICAL) == ULOG_ERR_NONE);
  assert(ULOG_SUBSCRIBE(logger_fn1, ULOG_ERROR) == ULOG_ERR_NONE);
  assert(ULOG_SUBSCRIBE(logger_fn2, ULOG_WARNING) == ULOG_ERR_NONE);
  assert(ULOG_SUBSCRIBE(logger_fn3, ULOG_INFO) == ULOG_ERR_NONE);
  assert(ULOG_SUBSCRIBE(logger_fn4, ULOG_DEBUG) == ULOG_ERR_NONE);
  assert(ULOG_SUBSCRIBE(logger_fn5, ULOG_TRACE) == ULOG_ERR_NONE);

  assert(ULOG_SUBSCRIBE(logger_fn6, ULOG_TRACE) == ULOG_ERR_SUBSCRIBERS_EXCEEDED);

  ULOG(ULOG_TRACE, "Hello!");
  ULOG(ULOG_DEBUG, "Hello!");
  ULOG(ULOG_INFO, "Hello!");
  ULOG(ULOG_WARNING, "Hello!");
  ULOG(ULOG_ERROR, "Hello!");
  ULOG(ULOG_CRITICAL, "Hello!");

  assert(fn_calls[0] == 1);  // logger_fn0 receives critical messages only
  assert(fn_calls[1] == 2);
  assert(fn_calls[2] == 3);
  assert(fn_calls[3] == 4);
  assert(fn_calls[4] == 5);
  assert(fn_calls[5] == 6);  // logger_fn5 is at trace level: all messages

  // reset counters.  Test unsubscribe
  memset(fn_calls, 0, sizeof(fn_calls));

  assert(ULOG_UNSUBSCRIBE(logger_fn0) == ULOG_ERR_NONE);
  assert(ULOG_UNSUBSCRIBE(logger_fn1) == ULOG_ERR_NONE);
  assert(ULOG_UNSUBSCRIBE(logger_fn2) == ULOG_ERR_NONE);
  assert(ULOG_UNSUBSCRIBE(logger_fn3) == ULOG_ERR_NONE);
  assert(ULOG_UNSUBSCRIBE(logger_fn4) == ULOG_ERR_NONE);
  // leave logger_fn5 subscribed
  // assert(ULOG_UNSUBSCRIBE(logger_fn5, ULOG_TRACE) == ULOG_ERR_NONE);

  assert(ULOG_UNSUBSCRIBE(logger_fn6) == ULOG_ERR_NOT_SUBSCRIBED);

  ULOG(ULOG_TRACE, "Hello!");
  ULOG(ULOG_DEBUG, "Hello!");
  ULOG(ULOG_INFO, "Hello!");
  ULOG(ULOG_WARNING, "Hello!");
  ULOG(ULOG_ERROR, "Hello!");
  ULOG(ULOG_CRITICAL, "Hello!");

  assert(fn_calls[0] == 0);  // not subscribed...
  assert(fn_calls[1] == 0);
  assert(fn_calls[2] == 0);
  assert(fn_calls[3] == 0);
  assert(fn_calls[4] == 0);
  assert(fn_calls[5] == 6);  // logger_fn5 is at trace level: all messages

  // ulog_severity_name
  assert(strcmp(ulog_severity_name(ULOG_TRACE), "TRACE") == 0);
  assert(strcmp(ulog_severity_name(ULOG_DEBUG), "DEBUG") == 0);
  assert(strcmp(ulog_severity_name(ULOG_INFO), "INFO") == 0);
  assert(strcmp(ulog_severity_name(ULOG_WARNING), "WARNING") == 0);
  assert(strcmp(ulog_severity_name(ULOG_ERROR), "ERROR") == 0);
  assert(strcmp(ulog_severity_name(ULOG_CRITICAL), "CRITICAL") == 0);

}
