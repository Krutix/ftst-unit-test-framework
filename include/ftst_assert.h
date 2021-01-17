#ifndef FTST_ASSERT_H
# define FTST_ASSERT_H

# include <stdio.h>

# if defined(FTST_NAMESPACE) && FTST_NAMESPACE
#  define ASSERT FTST_ASSERT
#  define STATIC_ASSERT FTST_STATIC_ASSERT
# endif

# ifndef FTST_ASSERT_LEVEL
#  define FTST_ASSERT_LEVEL 2
# endif

# define __FTST_ERROR_MESSAGE(expr, error_message)\
                    fprintf(stderr, "%d [%s]: [%s] \"%s\"\n", __LINE__, __FUNCTION__, #expr, error_message); \
                    fflush(stderr)

# define __FTST_BREAK_POINT() __asm__("int $3")

# define __FTST_ASSERT_GLUE_(a, b) a ## b
# define __FTST_ASSERT_GLUE(a, b) __FTST_ASSERT_GLUE_(a, b)

/*
** Runtime assert which triggered breakpoint when expression is false
*/
# if FTST_ASSERT_LEVEL >= 2
#  define FTST_ASSERT(expr, error_message)              \
    {                                                   \
        if (!(expr)) {                                  \
            __FTST_ERROR_MESSAGE(expr, error_message);  \
            __FTST_BREAK_POINT();                       \
        }                                               \
    }
# else
#  define FTST_ASSERT(expr, error_message)
# endif

/*
** Compiletime assert which triggered compilation error when expression is false
*/
# if FTST_ASSERT_LEVEL >= 1
# define FTST_STATIC_ASSERT(expr)                       \
    enum {                                              \
        __FTST_ASSERT_GLUE(g_assert_fail_, __LINE__)    \
                = 1 / (int) (!!(expr))                  \
    }
# else
#  define FTST_STATIC_ASSERT(expr)
# endif
#endif
