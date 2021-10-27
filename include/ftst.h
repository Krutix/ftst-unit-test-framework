#ifndef FTST_H
# define FTST_H

# include <time.h>
# include <stdio.h>
# include <string.h>
# include <stdarg.h>
# include <stdint.h>
# include <stdbool.h>

/*************************************************
**              Default settings                **
*************************************************/

/*
** FTST_COLOR
** if define as 'true' or '1' its define shortest names for common macro functions
*/
# ifndef        FTST_COLOR 
#  define       FTST_COLOR      true
# endif
/*
** FTST_ALLOC_TEST
** if define as 'false' or '0' all allocation tests will be removed
*/
# ifndef        FTST_MALLOC_TEST
#  define       FTST_MALLOC_TEST     false
# endif

/*!
* \defgroup FTST TOOLCHAIN
*/

//! \{
// TOOLCHAIN START

//! \defgroup BASIC DEFINES
//! \{
// BASIC DEFINES START
# define __FTST_CONCAT(l, r) l##r
# define __FTST_TOSTR(val) #val

# define __FTST_EXPAND(...)         __VA_ARGS__
# define __FTST_PARENTHESISE(...)  (__VA_ARGS__)
# define __FTST_UNPARENTHESISE(x)  __FTST_EXPAND(__FTST_EXPAND x)

# ifdef FTST_MAIN_FILE
#  define __FTST_FUNCTION(signature, body) \
signature \
body
# else
#  define __FTST_FUNCTION(signature, body) \
extern signature;
# endif

# ifdef FTST_MAIN_FILE
#  define __FTST_STATICFUNC(signature, body) \
static \
signature \
body
# else
#  define __FTST_STATICFUNC(signature, body)
# endif

# define __FTST_DEFINE_VAR(type, name)      \
__FTST_FUNCTION(type* __FTST_CONCAT(__ftst_get_, name)(void), \
{                               \
    static type name;           \
    return (&name);             \
})

# define __FTST_DEFINE_STATICVAR(type, name)      \
__FTST_STATICFUNC(type* __FTST_CONCAT(__ftst_get_, name)(void), \
{                               \
    static type name;           \
    return (&name);             \
})

# define __FTST_GET_VAR(name) (*__FTST_CONCAT(__ftst_get_, name)())
// BASIC DEFINES END
//! \}



/*! \defgroup MULTI_MACRO
 *  allow to use same name for macro with different number of args
 *
 * __FTST_MULTI_MACRO choose macro named FUNC and postfix with number of args
 *
 *  __FTST_MULTI_MACRO(MY_MACRO, __VA_ARGS__)
 *  MY_MACRO_3(a, b, c) REAL_FUNCTION(a, b, c)
 *  MY_MACRO_2(a, b)    MY_MACRO(a, b, default_c)
 *  MY_MACRO_1(a)       MY_MACRO(a, default_b, default_c)
 *
 */

//! \{
// MULTI MACRO START
# define __FTST_FUNC_CHOOSER(_f0, _f1, _f2, _f3, _f4, _f5, _f6, _f7, _f8, _f9, _f10, _f11, _f12, _f13, _f14, _f15, _f16, ...) _f16
# define __FTST_FUNC_RECOMPOSER(args_with_parentheses) __FTST_FUNC_CHOOSER args_with_parentheses
# define __FTST_CHOOSE_FROM_ARG_COUNT(F, ...) __FTST_FUNC_RECOMPOSER((__VA_ARGS__,     \
             F##_16, F##_15, F##_14, F##_13, F##_12, F##_11, F##_10, F##_9, F##_8,     \
             F##_7, F##_6, F##_5, F##_4, F##_3, F##_2, F##_1, ))
# define __FTST_NO_ARG_EXPANDER(FUNC) ,,,,,,,,,,,,,,,,FUNC ## _0
# define __FTST_MACRO_CHOOSER(FUNC, ...) __FTST_CHOOSE_FROM_ARG_COUNT(FUNC, __FTST_NO_ARG_EXPANDER __VA_ARGS__ (FUNC))
# define __FTST_MULTI_MACRO(FUNC, ...) __FTST_MACRO_CHOOSER(FUNC, __VA_ARGS__)(__VA_ARGS__)
// MULTI MACRO END
//! \}



/*! \defgroup ASSERTION
*/
//! \{
// ASSERTION START

# ifndef    FTST_ASSERT_LEVEL
#  define   FTST_ASSERT_LEVEL 2
# endif

# define __FTST_ASSERT_ERROR_MESSAGE(expr, error_message)               \
                    fprintf(stderr, "%d [%s]: [%s] \"%s\"\n",           \
                        __LINE__, __FUNCTION__, #expr, error_message);  \
                    fflush(stderr)

# define __FTST_BREAK_POINT() __asm__("int $3")

# define __FTST_ASSERT_GLUE_(line, a, b) line##a##b
# define __FTST_ASSERT_GLUE(a, b) __FTST_ASSERT_GLUE_(__LINE__, a, b)

/*! \brief Runtime assert which triggered breakpoint when expression is false
*
*/
#  define __FTST_RUNTIME_ASSERT(expr, error_message)            \
    {                                                           \
        if (!(expr)) {                                          \
            __FTST_ASSERT_ERROR_MESSAGE(expr, error_message);   \
            __FTST_BREAK_POINT();                               \
        }                                                       \
    }

# if FTST_ASSERT_LEVEL >= 2
#  define FTST_RUNTIME_ASSERT(expr, error_message)              \
        __FTST_RUNTIME_ASSERT(expr, error_message)
# else
#  define FTST_ASSERT(expr, error_message)
# endif

/* Compiletime assert which triggered compilation error when expression is false */
# define __FTST_STATIC_ASSERT(expr, error_message)              \
    enum {                                                      \
        __FTST_ASSERT_GLUE(_assert_fail_, error_message)        \
                = 1 / (int) (!!(expr))                          \
    }

# if FTST_ASSERT_LEVEL >= 1
# define  FTST_STATIC_ASSERT(expr, error_message)                \
        __FTST_STATIC_ASSERT(expr, error_message)
# else
#  define FTST_STATIC_ASSERT(expr, error_message)
# endif

// ASSERTION END
//! \}

/*****************************************************
**                   PRETTY_PRINT                   **
*****************************************************/

/*          COLORS          */

# if FTST_COLOR
#  define __FTST_ANSI_COLOR_RED     "\x1b[31m"
#  define __FTST_ANSI_COLOR_GREEN   "\x1b[32m"
#  define __FTST_ANSI_COLOR_YELLOW  "\x1b[33m"
#  define __FTST_ANSI_COLOR_BLUE    "\x1b[34m"
#  define __FTST_ANSI_COLOR_MAGENTA "\x1b[35m"
#  define __FTST_ANSI_COLOR_CYAN    "\x1b[36m"
#  define __FTST_ANSI_COLOR_RESET   "\x1b[0m"
#  define __FTST_ANSI_CLEAR_LINE    "\033[A\033[2K"
# else
#  define __FTST_ANSI_COLOR_RED
#  define __FTST_ANSI_COLOR_GREEN
#  define __FTST_ANSI_COLOR_YELLOW
#  define __FTST_ANSI_COLOR_BLUE
#  define __FTST_ANSI_COLOR_MAGENTA
#  define __FTST_ANSI_COLOR_CYAN
#  define __FTST_ANSI_COLOR_RESET
# endif

/*          PRETTY STRING DECORATORS          */

# define __FTST_PRETTY_WRAP_COLOR(color, str)   color str __FTST_ANSI_COLOR_RESET
# define __FTST_PRETTY_STR_RED(str)             __FTST_PRETTY_WRAP_COLOR(__FTST_ANSI_COLOR_RED, str)
# define __FTST_PRETTY_STR_GREEN(str)           __FTST_PRETTY_WRAP_COLOR(__FTST_ANSI_COLOR_GREEN, str)
# define __FTST_PRETTY_STR_YELLOW(str)          __FTST_PRETTY_WRAP_COLOR(__FTST_ANSI_COLOR_YELLOW, str)
# define __FTST_PRETTY_STR_BLUE(str)            __FTST_PRETTY_WRAP_COLOR(__FTST_ANSI_COLOR_BLUE, str)
# define __FTST_PRETTY_STR_MAGENTA(str)         __FTST_PRETTY_WRAP_COLOR(__FTST_ANSI_COLOR_MAGENTA, str)
# define __FTST_PRETTY_STR_CYAN(str)            __FTST_PRETTY_WRAP_COLOR(__FTST_ANSI_COLOR_BLUE, str)

/*          TYPE DECORATORS          */

# define __FTST_EQ_DEFAULT_TYPE i

# define __FTST_GET_TYPE(type) __FTST_TYPE_##type

# define __FTST_TYPE_i                  int
# define __FTST_TYPE_li                 long __FTST_TYPE_i
# define __FTST_TYPE_lli                long long __FTST_TYPE_i
# define __FTST_TYPE_zi                 __ssize_t
# define __FTST_TYPE_d                  __FTST_TYPE_i
# define __FTST_TYPE_ld                 __FTST_TYPE_li
# define __FTST_TYPE_lld                __FTST_TYPE_lli
# define __FTST_TYPE_zd                 __FTST_TYPE_i
# define __FTST_TYPE_u                  unsigned int
# define __FTST_TYPE_lu                 long unsigned int
# define __FTST_TYPE_llu                long long unsigned int
# define __FTST_TYPE_x                  __FTST_TYPE_u
# define __FTST_TYPE_lx                 __FTST_TYPE_lu
# define __FTST_TYPE_llx                __FTST_TYPE_llu
# define __FTST_TYPE_p                  void*
# define __FTST_TYPE_zu                 size_t
# define __FTST_TYPE_gf                 float
# define __FTST_TYPE_g                  double
# define __FTST_TYPE_Lg                 long double
# define __FTST_TYPE_ff                 float
# define __FTST_TYPE_f                  double
# define __FTST_TYPE_Lf                 long double
# define __FTST_TYPE_c                  char
# define __FTST_TYPE_lc                 wchar_t
# define __FTST_TYPE_s                  char*
# define __FTST_TYPE_ls                 wchar_t*

// TOOLCHAIN END
//! \}

/*!
* \defgroup ALLOCATION TEST
*/
//! \{
// ALLOCATION TEST START

# if        FTST_MALLOC_TEST

#  ifdef       FTST_MAIN_FILE
#   if   defined(__linux__)
#    include <malloc.h>
#    define _GNU_SOURCE
#    define __USE_GNU
#   elif defined(__APPLE__)
#    include <malloc/malloc.h>
#   endif
#   include <dlfcn.h>
#   include <errno.h>
/*
** On linux use -ldl flag to link dlfcn lib
*/
#  endif


typedef struct {
    bool error_happend;
}   __ftst_alloc_t;

typedef void*(*__libc_malloc_t)(size_t);
typedef void(*__libc_free_t)(void*);

__FTST_DEFINE_STATICVAR(__ftst_alloc_t, alloc);
__FTST_DEFINE_STATICVAR(__libc_malloc_t, libc_malloc);
__FTST_DEFINE_STATICVAR(__libc_free_t, libc_free);

/*              LIST MANAGMENT FUNCTIONS          */

typedef struct      __ftst_list_s
{
    struct __ftst_list_s*   next;
    void*                   ptr;
    size_t                  size;
}                   __ftst_list_t;

__FTST_DEFINE_STATICVAR(__ftst_list_t*, memory_list);

__FTST_STATICFUNC(
__ftst_list_t*	__ftst_list_find(__ftst_list_t *begin_list, void *ptr_ref),
{
    while (begin_list)
    {
        if (begin_list->ptr == ptr_ref)
            return (begin_list);
        begin_list = begin_list->next;
    }
    return (NULL);
})

__FTST_STATICFUNC(
__ftst_list_t*	__ftst_create_list(void *ptr, size_t size),
{
    __ftst_list_t *new_node;

    new_node = __FTST_GET_VAR(libc_malloc)(sizeof(__ftst_list_t));
    __FTST_RUNTIME_ASSERT(new_node, "malloc can't allocate enough memory");
    new_node->next = NULL;
    new_node->ptr = ptr;
    new_node->size = size;
    return (new_node);
})

__FTST_STATICFUNC(
void     __ftst_list_push_front(__ftst_list_t **begin_list, __ftst_list_t *node),
{
    node->next = *begin_list;
    *begin_list = node;
})

__FTST_STATICFUNC(
void     __ftst_list_remove_if(__ftst_list_t **begin_list, void* ptr_ref),
{
    __ftst_list_t*    buff;
    __ftst_list_t*    prev;
    __ftst_list_t*    next;

    buff = *begin_list;
    prev = NULL;
    while (buff)
    {
        next = buff->next;
        if (buff->ptr == ptr_ref)
        {
            if (prev)
                prev->next = buff->next;
            else
                *begin_list = buff->next;
            __FTST_GET_VAR(libc_free)(buff);
            return ;
        }
        else
            prev = buff;
        buff = next;
    }
})

__FTST_STATICFUNC(
void __ftst_list_clear(__ftst_list_t *begin_list),
{
    __ftst_list_t *buff;

    while (begin_list)
    {
        buff = begin_list;
        begin_list = buff->next;
        __FTST_GET_VAR(libc_free)(buff);
    }
})

/***************************************************/
/*                 ALLOC FUNCTIONS                 */

__FTST_FUNCTION(
size_t __ftst_malloc_size(void* ptr),
{
    __ftst_list_t* find_ptr = __ftst_list_find(__FTST_GET_VAR(memory_list), ptr);
    __FTST_RUNTIME_ASSERT(find_ptr, "can't find memory in aggregator");
    return (find_ptr->size);
})

#  define FTST_MALLOC_SIZE(ptr)     __ftst_malloc_size(ptr)

/*********************************************************/
/*                      LEAKS CHECK                      */

__FTST_FUNCTION(
bool        __ftst_leaks(void),
{
    return (!!__FTST_GET_VAR(memory_list));
})

#  define FTST_LEAKS()          __ftst_leaks()

__FTST_FUNCTION(
void        __ftst_leak_stat_reset(void),
{
    __ftst_list_clear(__FTST_GET_VAR(memory_list));
    __FTST_GET_VAR(memory_list) = NULL;
})

#  define   FTST_LEAK_RESET()   __ftst_leak_stat_reset();

/*******************************************************/
/*                    MALLOC REPLACE                   */

__FTST_STATICFUNC(
void* __ftst_malloc_error(),
{
    __FTST_GET_VAR(alloc).error_happend = true;
    errno = ENOMEM;
    return (NULL);
})

__FTST_FUNCTION(
void* malloc(size_t size),
{
    void*   ptr;

    __FTST_GET_VAR(alloc).error_happend = false;

    ptr = __FTST_GET_VAR(libc_malloc)(size);
    __ftst_list_push_front(&__FTST_GET_VAR(memory_list), __ftst_create_list(ptr, size));

    return (ptr);
})

__FTST_FUNCTION(
void free(void *p),
{
    __ftst_list_remove_if(&__FTST_GET_VAR(memory_list), p);
    __FTST_GET_VAR(libc_free)(p);
})

/************************************************/
/*                  ALLOC INIT                  */
__FTST_STATICFUNC(
void __ftst_init_alloc(void),
{
    __FTST_GET_VAR(libc_malloc) = dlsym(RTLD_NEXT, "malloc");
    __FTST_GET_VAR(libc_free)   = dlsym(RTLD_NEXT, "free");
})

__FTST_STATICFUNC(
void __ftst_exit_alloc(void), { })

# endif
// ALLOCATION TEST END
//! \}

/*******************************************************
*******************************************************/

# define __FTST_INPASSERT(cmp, fail)   \
{                                       \
    if (!(cmp))                         \
    {                                   \
        fail                            \
    }                                   \
}

# define __FTST_CREATE_VARINPLACE(type, num, get)          \
    __FTST_CONCAT(__FTST_TYPE_, type) __FTST_CONCAT(v,num) = get;

# define __FTST_CREATEVARS_0()         __FTST_STATIC_ASSERT(0,);
# define __FTST_CREATEVARS_1(...)      __FTST_STATIC_ASSERT(0,);
# define __FTST_CREATEVARS_2(type, get0)                    \
    __FTST_CREATE_VARINPLACE(type, 0, get1)
# define __FTST_CREATEVARS_3(type, get0, get1)             \
    __FTST_CREATEVARS_2(type, get0)                         \
    __FTST_CREATE_VARINPLACE(type, 1, get1)
# define __FTST_CREATEVARS_4(type, get0, get1, get2)       \
    __FTST_CREATEVARS_3(type, get0)                         \
    __FTST_CREATE_VARINPLACE(type, 2, get2)
# define __FTST_CREATEVARS_5(type, get0, get1, get2)       \
    __FTST_CREATEVARS_4(type, get0)                         \
    __FTST_CREATE_VARINPLACE(type, 3, get3)

# define __FTST_CREATEVARS(...)        __FTST_MULTI_MACRO(__FTST_CREATEVARS, __VA_ARGS__)


# define __FTST_ARGS_GETTYPE(type, fail) type
# define __FTST_ARGS_GETFAIL(type, fail) fail

# define __FTST_FAILACTION(args, msg)  \
    {                                   \
        __FTST_ARGS_GETFAIL(args)       \
    }

# define __FTST_COMPARE(cmp, getters, args, msg)                       \
    {                                                                   \
        __FTST_CREATE_VARINPLACE(__FTST_ARGS_GETTYPE(args), getters)    \
        __FTST_INPASSERT(cmp, __FTST_FAILACTION(args, msg))             \
    }

# define __FTST_ARG_2(...)          // real FTST_OPEN_ARG
# define __FTST_ARG_1(...)          __FTST_ARG_2(__VA_ARGS__, {})
# define __FTST_ARG_0()             __FTST_ARG_1(__FTST_EQ_DEFAULT_TYPE)

# define __FTST_MSG_1(...)          // real FTST_MSG
# define __FTST_MSG_0()             __FTST_MSG_1(NULL)

#endif
