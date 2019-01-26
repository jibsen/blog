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

#ifndef BLOG_H_INCLUDED
#define BLOG_H_INCLUDED

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	BLOG_ERROR, /**< Unrecoverable error */
	BLOG_WARN,  /**< Recoverable error */
	BLOG_INFO,  /**< Normal significant event */
	BLOG_DEBUG, /**< Normal insignificant event */
	BLOG_TRACE  /**< Implementation detail */
} blog_debug_level;

extern FILE *blog_stream;
extern int blog_level;

void
blog_init(FILE *stream, int level);

#if defined(__MINGW32__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 4))
__attribute__((format(__MINGW_PRINTF_FORMAT, 5, 6)))
#elif defined(__GNUC__) && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1))
__attribute__((format(printf, 5, 6)))
#elif defined(__has_attribute)
#  if __has_attribute(format)
__attribute__((format(printf, 5, 6)))
#  endif
#endif
void
blog_fprintf(FILE *stream, const char *file, int line, int level, const char *fmt, ...);

#if defined(NBLOG)

#define blog(level, ...) ((void) 0)

#else

#define blog(level, ...) \
	do { \
		if ((level) <= blog_level) { \
			blog_fprintf(blog_stream, __FILE__, __LINE__, level, __VA_ARGS__); \
		} \
	} while (0)

#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* BLOG_H_INCLUDED */
