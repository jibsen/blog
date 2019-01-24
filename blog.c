/*
 * blog - basic logging
 *
 * Copyright (c) 2016-2018 Joergen Ibsen
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

#include "blog.h"

#include <stdarg.h>

FILE *blog_stream = NULL;
int blog_level = BLOG_INFO;

static const char *blog_level_names[] = {
	"error",
	"warning",
	"info",
	"debug",
	"trace"
};

void
blog_init(FILE *stream, int level)
{
	blog_stream = stream;
	blog_level = level;
	blog(BLOG_INFO, "blog initialized (debug level %d - %s)", level, blog_level_names[level]);
}

void
blog_fprintf(FILE *stream, const char *file, int line, int level, const char *fmt, ...)
{
	va_list ap;

	if (stream == NULL) {
		stream = stderr;
	}

	fprintf(stream, "%s:%d:%s: ", file, line, blog_level_names[level]);

	va_start(ap, fmt);

	vfprintf(stream, fmt, ap);

	va_end(ap);

	fprintf(stream, "\n");
}
