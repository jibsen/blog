/*
 * blog - basic logging
 *
 * Copyright (c) 2016-2019 Joergen Ibsen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifdef BLOG_NO_LOCKING
#  define flockfile(stream) ((void) 0)
#  define funlockfile(stream) ((void) 0)
#else
#  ifdef _WIN32
#    define flockfile(stream) _lock_file(stream)
#    define funlockfile(stream) _unlock_file(stream)
#  else
#    define _POSIX_C_SOURCE 200809L
#    include <unistd.h>
#    if !defined(_POSIX_THREAD_SAFE_FUNCTIONS) || (_POSIX_THREAD_SAFE_FUNCTIONS - 0 <= 0)
#      error "blog locking requires POSIX flockfile"
#    endif
#  endif
#endif

#include "blog.h"

#include <stdarg.h>

#ifdef BLOG_TIMESTAMP
#  include <time.h>
#  if defined(_WIN32) && !defined(_POSIX_C_SOURCE)
#    define localtime_r(t, res) (localtime_s((res), (t)) ? NULL : (res))
#  endif
#endif

#ifndef ARRAY_SIZE
#  define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

FILE *blog_stream = NULL;
int blog_level = BLOG_INFO;

static const char*
blog_level_name(int level)
{
	static const char *level_names[] = {
		"error",
		"warning",
		"info",
		"debug",
		"trace"
	};

	const char *name = "?";

	if (level >= 0 && level < ARRAY_SIZE(level_names)) {
		name = level_names[level];
	}

	return name;
}

void
blog_init(FILE *stream, int level)
{
	blog_stream = stream;
	blog_level = level;
	blog(BLOG_INFO, "blog initialized (level %d - %s)", level, blog_level_name(level));
}

void
blog_fprintf(FILE *stream, const char *file, int line, int level, const char *fmt, ...)
{
	va_list ap;

	if (stream == NULL) {
		stream = stderr;
	}

	flockfile(stream);

#ifdef BLOG_TIMESTAMP
	char timestamp[32];
	struct tm lt;
	time_t t = time(NULL);

	if (localtime_r(&t, &lt)
	 && strftime(timestamp, ARRAY_SIZE(timestamp), "[%Y-%m-%d %H:%M:%S] ", &lt)) {
		fputs(timestamp, stream);
	}
#endif

	fprintf(stream, "[%s] %s:%d: ", blog_level_name(level), file, line);

	va_start(ap, fmt);

	vfprintf(stream, fmt, ap);

	va_end(ap);

	fputs("\n", stream);

	funlockfile(stream);
}
