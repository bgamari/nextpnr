/*
 *  nextpnr -- Next Generation Place and Route
 *
 *  Copyright (C) 2018  Clifford Wolf <clifford@symbioticeda.com>
 *
 *  Permission to use, copy, modify, and/or distribute this software for any
 *  purpose with or without fee is hereby granted, provided that the above
 *  copyright notice and this permission notice appear in all copies.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#ifndef LOG_H
#define LOG_H

#include <functional>
#include <ostream>
#include <set>
#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <vector>

#include "nextpnr.h"

NEXTPNR_NAMESPACE_BEGIN

typedef std::function<void(std::string)> log_write_type;

struct log_cmd_error_exception
{
};

struct log_execution_error_exception
{
};

extern std::vector<FILE *> log_files;
extern std::vector<std::ostream *> log_streams;
extern FILE *log_errfile;
extern log_write_type log_write_function;

extern bool log_quiet_warnings;
extern std::string log_last_error;
extern void (*log_error_atexit)();

std::string stringf(const char *fmt, ...);
std::string vstringf(const char *fmt, va_list ap);

extern std::ostream clog;
void log(const char *format, ...) NPNR_ATTRIBUTE(format(printf, 1, 2));
void log_always(const char *format, ...) NPNR_ATTRIBUTE(format(printf, 1, 2));
void log_info(const char *format, ...) NPNR_ATTRIBUTE(format(printf, 1, 2));
void log_warning(const char *format, ...) NPNR_ATTRIBUTE(format(printf, 1, 2));
void log_warning_noprefix(const char *format, ...) NPNR_ATTRIBUTE(format(printf, 1, 2));
NPNR_NORETURN void log_error(const char *format, ...) NPNR_ATTRIBUTE(format(printf, 1, 2), noreturn);
NPNR_NORETURN void log_cmd_error(const char *format, ...) NPNR_ATTRIBUTE(format(printf, 1, 2), noreturn);

void log_break();
void log_flush();

static inline void log_assert_worker(bool cond, const char *expr, const char *file, int line)
{
    if (!cond)
        log_error("Assert `%s' failed in %s:%d.\n", expr, file, line);
}
#define log_assert(_assert_expr_)                                                                                      \
    NEXTPNR_NAMESPACE_PREFIX log_assert_worker(_assert_expr_, #_assert_expr_, __FILE__, __LINE__)

#define log_abort() log_error("Abort in %s:%d.\n", __FILE__, __LINE__)
#define log_ping() log("-- %s:%d %s --\n", __FILE__, __LINE__, __PRETTY_FUNCTION__)

NEXTPNR_NAMESPACE_END

#endif
