
# blog - basic logging


## About

blog provides basic logging functionality aimed at developers.

Most programmers are familiar with print(f) debugging, where you sprinkle
print statements around the code while trying to figure out what is happening.

`blog()` is a macro which you can use to insert such statements in your code.
The benefit is more structured output, and making it is easy to disable some
(based on level) or all of them.

If you need more functionality, please see the section on related projects.


## Usage

blog assigns a level to each message. The supported levels are:

  - `BLOG_ERROR` (0) - unrecoverable error
  - `BLOG_WARN` (1) - recoverable error
  - `BLOG_INFO` (2) - normal significant event
  - `BLOG_DEBUG` (3) - normal insignificant event
  - `BLOG_TRACE` (4) - implementation detail

Here is an example of a C function with some logging calls:

~~~c
#include "blog.h"

/* Merge sorted linked lists p and q into p (not stable) */
void merge(struct node **p, struct node **q)
{
    blog(BLOG_INFO, "merging lists");

    if (!*q) {
        blog(BLOG_DEBUG, "q is empty");

        return;
    }

    while (*p) {
        blog(BLOG_TRACE, "comparing %d to %d", (*p)->val, (*q)->val);

        if ((*p)->val > (*q)->val) {
            blog(BLOG_TRACE, "swapping tails");

            struct node *tmp = *p;
            *p = *q;
            *q = tmp;
        }
        p = &(*p)->next;
    }

    blog(BLOG_TRACE, "moving tail of larger elements");

    *p = *q;
    *q = NULL;
}
~~~

As you can see, `blog()` works like printf with the addition of the level as
the first parameter. With the default logging level of `BLOG_INFO`, the output
on stderr from calling this function would be something like:

    [info] llist.c:114: merging lists


### Controlling output

blog uses two global variables:

  - `blog_stream` - FILE pointer to write messages to (defaults to stderr if
    `NULL`)
  - `blog_level` - dynamic logging level, messages above this are ignored
    (defaults to `BLOG_INFO`)

Say we create a file for logging and set the level to `BLOG_TRACE`, either
manually, or using the `blog_init()` convenience function:

~~~c
FILE *log_file = fopen("log.txt", "w");

if (!log_file) {
    perror("cannot open log.txt for writing");
    exit(EXIT_FAILURE);
}

blog_init(log_file, BLOG_TRACE);
~~~

The output in `log.txt` from the above example might now be something like:

    [info] blog.c:84: blog initialized (level 4 - trace)
    [info] llist.c:114: merging lists
    [trace] llist.c:123: comparing 3 to 5
    [trace] llist.c:123: comparing 11 to 5
    [trace] llist.c:126: swapping tails
    [trace] llist.c:123: comparing 7 to 11
    [trace] llist.c:123: comparing 19 to 11
    [trace] llist.c:126: swapping tails
    [trace] llist.c:123: comparing 13 to 19
    [trace] llist.c:135: moving tail of larger elements

If blog is compiled with `BLOG_TIMESTAMP` defined, each line will be prefixed
with a timestamp in square brackets.

    [2019-01-28 13:08:10] [info] llist.c:114: merging lists

By default blog locks the output file while writing to it. This can be
disabled by compiling blog with `BLOG_NO_LOCKING`.


### Disabling output

If you define `NBLOG` before including `blog.h`, then `blog()` becomes a
no-op. This works like `NDEBUG` for `assert.h` in that you can include
`blog.h` multiple times and `blog()` will be redefined based on `NBLOG`.

Similarly, `BLOG_MAX_LEVEL` can be used to set a compile time logging level
before including `blog.h`. Any `blog()` above this level do nothing. With
optimization turned on, most compilers will remove such calls entirely.


## Notes

  - blog uses C99 variadic macros and POSIX or Windows file locking.


## Related Projects

This is a random sample of logging libraries from a quick web search:

  - [Boost.Log](https://github.com/boostorg/log)
  - [glog](https://github.com/google/glog)
  - [log.c](https://github.com/rxi/log.c)
  - [log4c](http://log4c.sourceforge.net/)
  - [Macro-Logger](https://github.com/dmcrodrigues/macro-logger)
  - [slog](https://github.com/kala13x/slog)
  - [spdlog](https://github.com/gabime/spdlog)
  - [zf_log](https://github.com/wonder-mice/zf_log)
  - [zlog](https://github.com/zma/zlog/)

I haven't tried all of them, so I cannot comment on the quality. Think of it
as a starting point if you want to look further.


## License

This projected is licensed under the [MIT License](LICENSE) (MIT).
