#ifndef FTST_H
# define FTST_H

# include <time.h>
# include <stdio.h>
# include <string.h>
# include <stdarg.h>
# include <stdint.h>
# include <stdbool.h>

/*************************************************
**              Default settings				**
*************************************************/

/*
** FTST_COLOR define colors for console output
*/
# ifndef        FTST_COLOR
#  define       FTST_COLOR          true
# endif
/*
** FTST_NAMESPACE
** if define as 'true' or '1' its define shortest names for common macro functions
*/
# ifndef        FTST_NAMESPACE 
#  define       FTST_NAMESPACE      true
# endif
/*
** FTST_SILENT
** if define as 'true' or '1' not an error message dont be printed in console
*/
# ifndef        FTST_SILENT
#  define       FTST_SILENT         false
# endif
/*
** FTST_ERROR_MESSAGE
** if define as 'false' or '0' error message dont be printed in console
*/
# ifndef        FTST_ERROR_MESSAGE
#  define       FTST_ERROR_MESSAGE  true
# endif
/*
** FTST_ALLOC_TEST
** if define as 'false' or '0' all allocation tests will be removed
*/
# ifndef        FTST_MALLOC_TEST
#  define       FTST_MALLOC_TEST     false
# endif
# ifndef        FTST_VAR_STR_BUFFER
#  define       FTST_VAR_STR_BUFFER 128
# endif

# if        FTST_NAMESPACE
#  define EQ            FTST_EQ
#  define NE            FTST_NE
#  define LESS          FTST_LESS
#  define LESSEQ        FTST_LESSEQ
#  define MORE          FTST_MORE
#  define MOREEQ        FTST_MOREEQ

#  define EQ_A          FTST_EQ_A
#  define NE_A          FTST_NE_A

#  define STR_EQ        FTST_STR_EQ
#  define STR_NE        FTST_STR_NE
#  define STR_LESS      FTST_STR_LESS
#  define STR_LESSEQ    FTST_STR_LESSEQ
#  define STR_MORE      FTST_STR_MORE
#  define STR_MOREEQ    FTST_STR_MOREEQ

#  define IS_TRUE       FTST_IS_TRUE
#  define IS_FALSE      FTST_IS_FALSE

#  define EXPECT        FTST_EXPECT
#  define ASSERT        FTST_ASSERT

#  define TEST          FTST_TEST
#  define RUNTEST       FTST_RUNTEST
# endif

# ifndef     FTST_MAIN_FILE
#  define   __FTST_EXTERN   extern
# else
#  define   __FTST_EXTERN
# endif

/************************************************/
/*					global data					*/

typedef struct {
    size_t      passed;
    size_t      launched;
    char const*	test_name;
}               __ftst_test_results;

typedef struct {
    FILE*       		stream;
    FILE*       		table;
}               __ftst_global;

__FTST_EXTERN __ftst_global __ftst_g;


/*****************************************************
**                   ASSERTION						**
*****************************************************/

# if        FTST_NAMESPACE
#  define   RUNTIME_ASSERT    FTST_RUNTIME_ASSERT
#  define   STATIC_ASSERT     FTST_STATIC_ASSERT
# endif

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

/* Runtime assert which triggered breakpoint when expression is false */
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

/*****************************************************
**                   PRETTY_PRINT					**
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

# define __FTST_PRETTY_PROCESSED(str)       __FTST_ANSI_COLOR_YELLOW    str     __FTST_ANSI_COLOR_RESET
# define __FTST_PRETTY_SUCCESS(str)         __FTST_ANSI_COLOR_GREEN     str     __FTST_ANSI_COLOR_RESET
# define __FTST_PRETTY_FAILED(str)          __FTST_ANSI_COLOR_RED       str     __FTST_ANSI_COLOR_RESET
# define __FTST_PRETTY_INFO(str)            __FTST_ANSI_COLOR_BLUE      str     __FTST_ANSI_COLOR_RESET
# define __FTST_PRETTY_TEST_CASE_NAME(str)  __FTST_ANSI_COLOR_CYAN      str     __FTST_ANSI_COLOR_RESET

/***********************************/
/*          STREAM OUTPUT          */

# ifdef FTST_MAIN_FILE
void    __ftst_vfprintf(FILE* stream, char const* format, va_list argptr)
{
    if (stream)
        vfprintf(stream, format, argptr);
}
# else
extern void    __ftst_vfprintf(FILE* stream, char const* format, ...);
# endif

# ifdef FTST_MAIN_FILE
void    __ftst_fprintf(FILE* stream, char const* format, ...)
{
    va_list arg_list;

    va_start(arg_list, format);
    __ftst_vfprintf(stream, format, arg_list);
    va_end(arg_list);
}
# else
extern void    __ftst_fprintf(FILE* stream, char const* format, ...);
# endif

# if !(FTST_SILENT)
#  define __FTST_STREAM_PRINTF(...)         __ftst_fprintf(__ftst_g.stream, __VA_ARGS__)
#  define __FTST_STREAM_VPRINTF(f, arg)     __ftst_vfprintf(__ftst_g.stream, f, arg)
#  define __FTST_FFLUSH_STREAM()            fflush(__ftst_g.stream)
# else
#  define __FTST_STREAM_PRINTF(...)
#  define __FTST_STREAM_VPRINTF(...)
#  define __FTST_FFLUSH_STREAM()
# endif

# define __FTST_TABLE_PRINTF(...)           __ftst_fprintf(__ftst_g.table, __VA_ARGS__)

# if (FTST_ERROR_MESSAGE)
#  define __FTST_ERROR_PRINTF(...)          __ftst_fprintf(__ftst_g.stream, __VA_ARGS__)
#  define __FTST_ERROR_VPRINTF(f, arg)      __ftst_vfprintf(__ftst_g.stream, f, arg)
# else
#  define __FTST_ERROR_PRINTF(...)
#  define __FTST_ERROR_VPRINTF(...)
# endif

/*****************************************************
**                  ALLOCATION TEST					**
*****************************************************/

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
    bool      is_active;
    size_t    d;
}   __ftst_alloc_limit;

typedef struct {
    __ftst_alloc_limit     limit;
    __ftst_alloc_limit     single_limit;
    __ftst_alloc_limit     fail_counter;
    bool                   error_happend;
}   __ftst_alloc;

__FTST_EXTERN __ftst_alloc __ftst_alloc_test;

#  define   __FTST_MALLOC_SET(value, test)      \
    {                                           \
        test.is_active = true;                  \
        test.d = value;                         \
    }

#  define   __FTST_MALLOC_CLEAN(test)           \
    {                                           \
        test.is_active = false;                 \
    }

#  define   FTST_MALLOC_COUNTER_SET(value)           __FTST_MALLOC_SET(value, __ftst_alloc_test.fail_counter)
#  define   FTST_MALLOC_COUNTER_CLEAN()              __FTST_MALLOC_CLEAN(__ftst_alloc_test.fail_counter)

#  define   FTST_MALLOC_LIMIT_SET(value)             __FTST_MALLOC_SET(value, __ftst_alloc_test.limit)
#  define   FTST_MALLOC_LIMIT_CLEAN()                __FTST_MALLOC_CLEAN(__ftst_alloc_test.limit)

#  define   FTST_MALLOC_SINGLE_LIMIT_SET(value)      __FTST_MALLOC_SET(value, __ftst_alloc_test.single_limit)
#  define   FTST_MALLOC_SINGLE_LIMIT_CLEAN()         __FTST_MALLOC_CLEAN(__ftst_alloc_test.single_limit)

/*
**
**  FTST_ALLOC_CLEAN call clean for all alloc limits
**
*/
#  define   FTST_MALLOC_CLEAN()             \
    {                                       \
        FTST_MALLOC_SINGLE_LIMIT_CLEAN();   \
        FTST_MALLOC_LIMIT_CLEAN();          \
        FTST_MALLOC_COUNTER_CLEAN();        \
    }

#  define   FTST_IS_MALLOC_ERROR    __ftst_alloc_test.error_happend

#  ifdef FTST_MAIN_FILE
static void*(*libc_malloc)(size_t);
static void(*libc_free)(void*);
#  endif

/*              LIST MANAGMENT FUNCTIONS          */

#  ifdef FTST_MAIN_FILE
typedef struct      __ftst_list_s
{
	struct __ftst_list_s*   next;
	void*                   ptr;
	size_t                  size;
}					__ftst_list;

static __ftst_list*  __ftst_memory_aggregator;

static __ftst_list*	__ftst_list_find(__ftst_list *begin_list, void *ptr_ref)
{
	while (begin_list)
	{
		if (begin_list->ptr == ptr_ref)
			return (begin_list);
		begin_list = begin_list->next;
	}
	return (NULL);
}

static __ftst_list*	__ftst_create_list(void *ptr, size_t size)
{
	__ftst_list *new_node;

	new_node = libc_malloc(sizeof(__ftst_list));
    __FTST_RUNTIME_ASSERT(new_node, "malloc can't allocate enough memory");
	new_node->next = NULL;
	new_node->ptr = ptr;
	new_node->size = size;
	return (new_node);
}

static void     __ftst_list_push_front(__ftst_list **begin_list, __ftst_list *node)
{
	node->next = *begin_list;
	*begin_list = node;
}

static void     __ftst_list_remove_if(__ftst_list **begin_list, void* ptr_ref)
{
	__ftst_list*    buff;
	__ftst_list*    prev;
	__ftst_list*    next;

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
			libc_free(buff);
            return ;
		}
		else
			prev = buff;
		buff = next;
	}
}

static void	__ftst_list_clear(__ftst_list *begin_list)
{
	__ftst_list *buff;

	while (begin_list)
	{
		buff = begin_list;
		begin_list = buff->next;
		libc_free(buff);
	}
}
#  endif

/***************************************************/
/*                 ALLOC FUNCTIONS                 */

#  ifdef FTST_MAIN_FILE
size_t        __ftst_malloc_size(void* ptr)
{
    __ftst_list* find_ptr = __ftst_list_find(__ftst_memory_aggregator, ptr);
    __FTST_RUNTIME_ASSERT(find_ptr, "can't find memory in aggregator");
    return (find_ptr->size);
}
#  else
extern size_t        __ftst_malloc_size(void* ptr);
#  endif

#  define FTST_MALLOC_SIZE(ptr)     __ftst_malloc_size(ptr)

/*********************************************************/
/*                      LEAKS CHECK                      */

#  ifdef FTST_MAIN_FILE
bool        __ftst_leaks(void)
{
    return (!!__ftst_memory_aggregator);
}
#  else
extern bool        __ftst_leaks(void);
#  endif

#  define FTST_LEAKS()          __ftst_leaks()

#  ifdef FTST_MAIN_FILE
void        __ftst_leak_stat_reset(void)
{
    __ftst_list_clear(__ftst_memory_aggregator);
    __ftst_memory_aggregator = NULL;
}
#  else
extern void        __ftst_leak_stat_reset(void);
#  endif

#  define   FTST_LEAK_RESET()   __ftst_leak_stat_reset();

/*******************************************************/
/*                    MALLOC REPLACE                   */

#  ifdef FTST_MAIN_FILE
static void*   __ftst_malloc_error()
{
    __ftst_alloc_test.error_happend = true;
    errno = ENOMEM;
    return (NULL);
}

void*   malloc(size_t size)
{
    void*   ptr;

    __ftst_alloc_test.error_happend = false;

    if (__ftst_alloc_test.single_limit.is_active &&
            __ftst_alloc_test.single_limit.d < size)
        return (__ftst_malloc_error());
    if (__ftst_alloc_test.limit.is_active)
    {
        if (__ftst_alloc_test.limit.d < size)
            return (__ftst_malloc_error());
        __ftst_alloc_test.limit.d -= size;
    }
    if (__ftst_alloc_test.fail_counter.is_active)
    {
        if (__ftst_alloc_test.fail_counter.d == 0)
            return (__ftst_malloc_error());
        __ftst_alloc_test.fail_counter.d--;
    }

    ptr = libc_malloc(size);
    __ftst_list_push_front(&__ftst_memory_aggregator, __ftst_create_list(ptr, size));

    return (ptr);
}

void    free(void *p)
{
    if (__ftst_alloc_test.limit.is_active)
        __ftst_alloc_test.limit.d += __ftst_malloc_size(p);

    __ftst_list_remove_if(&__ftst_memory_aggregator, p);
    libc_free(p);
}
#  endif

/************************************************/
/*                  ALLOC INIT                  */
#  ifdef FTST_MAIN_FILE
static void __ftst_init_alloc(void)
{
    libc_malloc = dlsym(RTLD_NEXT, "malloc");
    libc_free   = dlsym(RTLD_NEXT, "free");
}

static void __ftst_exit_alloc(void) { }
#  endif 

# else

#  define  FTST_MALLOC_COUNTER_SET(...)
#  define  FTST_MALLOC_COUNTER_CLEAN(...)

#  define  FTST_MALLOC_LIMIT_SET(...)
#  define  FTST_MALLOC_LIMIT_CLEAN(...)

#  define  FTST_MALLOC_SINGLE_LIMIT_SET(...)
#  define  FTST_MALLOC_SINGLE_LIMIT_CLEAN(...)

#  define  FTST_LEAK_RESET(...)

#  define  FTST_IS_MALLOC_ERROR     0

#  define  FTST_MALLOC_SIZE(...)    0
#  define  FTST_LEAKS(...)          0

# endif

# if       FTST_NAMESPACE
#  define  MALLOC_COUNTER_SET           FTST_MALLOC_COUNTER_SET
#  define  MALLOC_COUNTER_CLEAN         FTST_MALLOC_COUNTER_CLEAN

#  define  MALLOC_LIMIT_SET             FTST_MALLOC_LIMIT_SET
#  define  MALLOC_LIMIT_CLEAN           FTST_MALLOC_LIMIT_CLEAN

#  define  MALLOC_SINGLE_LIMIT_SET      FTST_MALLOC_SINGLE_LIMIT_SET
#  define  MALLOC_SINGLE_LIMIT_CLEAN    FTST_MALLOC_SINGLE_LIMIT_CLEAN

#  define  MALLOC_CLEAN                 FTST_MALLOC_CLEAN                                    

#  define  LEAK_RESET                  FTST_LEAK_RESET                   

#  define  MALLOC_SIZE                 FTST_MALLOC_SIZE
#  define  LEAKS                       FTST_LEAKS
# endif

/*******************************************************
*******************************************************/

# define __FTST_TEST_CASE(test_name)        __ftst_test_case_##test_name
# define __FTST_TEST_CASE_NAME(test_name)   #test_name

/*
**                           MULTI MACRO
**  allow to use same name for macro with different number of args
**
**  __FTST_MULTI_MACRO choose macro named FUNC and postfix with number of args
**
**  example:
**  __FTST_MULTI_MACRO(MY_MACRO, __VA_ARGS__)
**  MY_MACRO_3(a, b, c) REAL_FUNCTION(a, b, c)
**  MY_MACRO_2(a, b)    MY_MACRO(a, b, default_c)
**  MY_MACRO_1(a)       MY_MACRO(a, default_b, default_c)
**
*/

#define __FTST_FUNC_CHOOSER(_f0, _f1, _f2, _f3, _f4, _f5, _f6, _f7, _f8, _f9, _f10, _f11, _f12, _f13, _f14, _f15, _f16, ...) _f16
#define __FTST_FUNC_RECOMPOSER(args_with_parentheses) __FTST_FUNC_CHOOSER args_with_parentheses
#define __FTST_CHOOSE_FROM_ARG_COUNT(F, ...) __FTST_FUNC_RECOMPOSER((__VA_ARGS__,     \
            F##_16, F##_15, F##_14, F##_13, F##_12, F##_11, F##_10, F##_9, F##_8,     \
            F##_7, F##_6, F##_5, F##_4, F##_3, F##_2, F##_1, ))
#define __FTST_NO_ARG_EXPANDER(FUNC) ,,,,,,,,,,,,,,,,FUNC ## _0
#define __FTST_MACRO_CHOOSER(FUNC, ...) __FTST_CHOOSE_FROM_ARG_COUNT(FUNC, __FTST_NO_ARG_EXPANDER __VA_ARGS__ (FUNC))
#define __FTST_MULTI_MACRO(FUNC, ...) __FTST_MACRO_CHOOSER(FUNC, __VA_ARGS__)(__VA_ARGS__)


# define __FTST_SNPRINTF(name, size, format, value)                 \
                    char name[size];                                \
                    snprintf(name, sizeof(name), format, value);

/*
**  Type converter from printf style to real type
**
**  i/d     | int
**  li/ld   | long int
**  lli/lld | long long int
**  zi/zd   | ssize_t
**
**  u/x     | int
**  lu/lx   | long int
**  llu/llx | long long int
**  p       | void*
**  zu      | size_t
**
**  f/g     | double
**  Lg/Lf   | long double
**
**  c       | char
**  lc      | wchar_t
**  s       | char*
**  ls      | wchar_t*
**
*/

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

/*****************************************************
**              	TEST CASE						**
*****************************************************/

typedef     void(*__ftst_test_t)(__ftst_test_results*);

# define FTST_TEST(test_name)                           \
void    __FTST_TEST_CASE(test_name)(__ftst_test_results* __ftst_current)

/****************************************************/
/*					TEST BRANCH						*/

# define __FTST_SIMPLE_TEST(cond, error_funct)          \
{                                                       \
    __ftst_current->launched++;                         \
    if (cond) {                                         \
        __ftst_current->passed++;                       \
    } else {                                            \
        error_funct;                                    \
    }                                                   \
}

/****************************************************/
/*					ERROR MESSAGE					*/


# ifdef FTST_MAIN_FILE
void    __ftst_test_write_error(size_t const line, char const* test_case_name, char const* test_name,
                            char const *actual_str, const char* actual)
{
    __FTST_ERROR_PRINTF(
        "["__FTST_PRETTY_INFO("%s")"] test from '%s' [" __FTST_PRETTY_FAILED("failed")"]" \
        "\n%4zu:  " "%6s: " __FTST_PRETTY_INFO("%s")"[%s]\n",
            test_name, test_case_name, line, "actual", actual, actual_str);
}

void    __ftst_test_write_addition(char const* value_name,
                            char const *value_str, const char* value)
{
    __FTST_ERROR_PRINTF(
        "%13s: " __FTST_PRETTY_INFO("%s")"[%s]\n",
            value_name, value, value_str);
}

void    __ftst_test_write_description(char const* description_name,
                            char const *format, ...)
{
    va_list argptr;
    va_start(argptr, format);

    __FTST_ERROR_PRINTF(
        __FTST_PRETTY_PROCESSED("%13s: "),
            description_name);
    __FTST_ERROR_VPRINTF(
        format,
            argptr);
    __FTST_STREAM_PRINTF("\n");

    va_end(argptr);
}
# else
extern void    __ftst_test_write_error(size_t const line, char const* test_case_name,
                            char const* test_name, char const *actual_str, const char* actual);
extern void    __ftst_test_write_addition(char const* value_name,
                            char const *value_str, const char* value);
extern void    __ftst_test_write_description(char const* description_name,
                            char const *format, ...);
# endif

# define __FTST_TEST_WRITE_ERROR(_test_name, actual, actual_str) \
        __ftst_test_write_error(__LINE__, __ftst_current->test_name, \
                    _test_name, actual, actual_str)

/****************************************************/
/*					TEST TEMPLATES					*/

# define FTST_EXPECT
# define FTST_ASSERT return;

# define __FTST_VA_UNPACK_1(a1, ...) a1

# define __FTST_STR_CMP_REAL(operation, actual, expect, error_funct, ...)                                   \
        {                                                                                                   \
            char const* actual_v = actual;                                                                  \
            char const* expect_v = expect;                                                                  \
            __FTST_SIMPLE_TEST(strcmp(actual_v, expect_v) operation 0,                                      \
                            __FTST_TEST_WRITE_ERROR(#operation, #actual, actual_v);                         \
                            __ftst_test_write_addition("expected", #expect, expect_v);                      \
                            if (__FTST_VA_UNPACK_1(__VA_ARGS__))                                            \
                                __ftst_test_write_description("description", __VA_ARGS__);                  \
                            error_funct)                                                                    \
        }

# define __FTST_TWO_CMP_REAL(operation, actual, expect, type, error_funct, ...)                             \
        {                                                                                                   \
            __FTST_GET_TYPE(type) actual_v = actual;                                                        \
            __FTST_GET_TYPE(type) expect_v = expect;                                                        \
            __FTST_SIMPLE_TEST((actual_v) operation (expect_v),                                             \
                            __FTST_SNPRINTF(actual_str, FTST_VAR_STR_BUFFER, "%"#type, actual_v);           \
                            __FTST_SNPRINTF(expect_str, FTST_VAR_STR_BUFFER, "%"#type, expect_v);           \
                            __FTST_TEST_WRITE_ERROR(#operation, #actual, actual_str);                       \
                            __ftst_test_write_addition("expected", #expect, expect_str);                    \
                            if (__FTST_VA_UNPACK_1(__VA_ARGS__))                                            \
                                __ftst_test_write_description("description", __VA_ARGS__);                  \
                            error_funct)                                                                    \
        }

# define __FTST_ACCUR_CMP_REAL(name, o_l, o_logic, o_r, actual, expect, accur, type, error_funct, ...)      \
        {                                                                                                   \
            __FTST_GET_TYPE(type) actual_v = actual;                                                        \
            __FTST_GET_TYPE(type) expect_v = expect;                                                        \
            __FTST_GET_TYPE(type) accur_v = accur;                                                          \
            __FTST_SIMPLE_TEST(((actual_v) o_l (expect_v - accur_v)) o_logic ((actual_v) o_r (expect_v + accur_v)),\
                            __FTST_SNPRINTF(actual_str, FTST_VAR_STR_BUFFER, "%"#type, actual_v);           \
                            __FTST_SNPRINTF(expect_str, FTST_VAR_STR_BUFFER, "%"#type, expect_v);           \
                            __FTST_SNPRINTF(accur_str, FTST_VAR_STR_BUFFER, "%"#type, accur_v);             \
                            __FTST_TEST_WRITE_ERROR(name, #actual, actual_str);                             \
                            __ftst_test_write_addition("expected", #expect, expect_str);                    \
                            __ftst_test_write_addition("accuracy", #accur, accur_str);                      \
                            if (__FTST_VA_UNPACK_1(__VA_ARGS__))                                            \
                                __ftst_test_write_description("description", __VA_ARGS__);                  \
                            error_funct)                                                                    \
        }

# define __FTST_ONE_CMP_REAL(operation, name, actual, type, error_funct, ...)                               \
        {                                                                                                   \
            __FTST_GET_TYPE(type) actual_v = actual;                                                        \
            __FTST_SIMPLE_TEST(operation(actual_v),                                                         \
                            __FTST_SNPRINTF(actual_str, FTST_VAR_STR_BUFFER, "%"#type, actual_v);           \
                            __FTST_TEST_WRITE_ERROR(name, #actual, actual_str);                             \
                            if (__FTST_VA_UNPACK_1(__VA_ARGS__))                                            \
                                __ftst_test_write_description("description", __VA_ARGS__);                  \
                            error_funct)                                                                    \
        }

# define __FTST_STR_CMP_16(...)          __FTST_STR_CMP_REAL(__VA_ARGS__)
# define __FTST_STR_CMP_15(...)          __FTST_STR_CMP_REAL(__VA_ARGS__)
# define __FTST_STR_CMP_14(...)          __FTST_STR_CMP_REAL(__VA_ARGS__)
# define __FTST_STR_CMP_13(...)          __FTST_STR_CMP_REAL(__VA_ARGS__)
# define __FTST_STR_CMP_12(...)          __FTST_STR_CMP_REAL(__VA_ARGS__)
# define __FTST_STR_CMP_11(...)          __FTST_STR_CMP_REAL(__VA_ARGS__)
# define __FTST_STR_CMP_10(...)          __FTST_STR_CMP_REAL(__VA_ARGS__)
# define __FTST_STR_CMP_9(...)           __FTST_STR_CMP_REAL(__VA_ARGS__)
# define __FTST_STR_CMP_8(...)           __FTST_STR_CMP_REAL(__VA_ARGS__)
# define __FTST_STR_CMP_7(...)           __FTST_STR_CMP_REAL(__VA_ARGS__)
# define __FTST_STR_CMP_6(...)           __FTST_STR_CMP_REAL(__VA_ARGS__)
# define __FTST_STR_CMP_5(...)           __FTST_STR_CMP_REAL(__VA_ARGS__)
# define __FTST_STR_CMP_4(...)           __FTST_STR_CMP_5(__VA_ARGS__, NULL)
# define __FTST_STR_CMP_3(...)           __FTST_STR_CMP_4(__VA_ARGS__, FTST_EXPECT)
# define __FTST_STR_CMP_2(...)           __FTST_STATIC_ASSERT(0, str_eq_take_2_or_more_arguments_not_1)
# define __FTST_STR_CMP_1(...)           __FTST_STATIC_ASSERT(0, str_eq_take_2_or_more_arguments_not_0)
# define __FTST_STR_CMP_0()              __FTST_STATIC_ASSERT(0,)

# define __FTST_TWO_CMP_16(...)          __FTST_TWO_CMP_REAL(__VA_ARGS__)
# define __FTST_TWO_CMP_15(...)          __FTST_TWO_CMP_REAL(__VA_ARGS__)
# define __FTST_TWO_CMP_14(...)          __FTST_TWO_CMP_REAL(__VA_ARGS__)
# define __FTST_TWO_CMP_13(...)          __FTST_TWO_CMP_REAL(__VA_ARGS__)
# define __FTST_TWO_CMP_12(...)          __FTST_TWO_CMP_REAL(__VA_ARGS__)
# define __FTST_TWO_CMP_11(...)          __FTST_TWO_CMP_REAL(__VA_ARGS__)
# define __FTST_TWO_CMP_10(...)          __FTST_TWO_CMP_REAL(__VA_ARGS__)
# define __FTST_TWO_CMP_9(...)           __FTST_TWO_CMP_REAL(__VA_ARGS__)
# define __FTST_TWO_CMP_8(...)           __FTST_TWO_CMP_REAL(__VA_ARGS__)
# define __FTST_TWO_CMP_7(...)           __FTST_TWO_CMP_REAL(__VA_ARGS__)
# define __FTST_TWO_CMP_6(...)           __FTST_TWO_CMP_REAL(__VA_ARGS__)
# define __FTST_TWO_CMP_5(...)           __FTST_TWO_CMP_6(__VA_ARGS__, NULL)
# define __FTST_TWO_CMP_4(...)           __FTST_TWO_CMP_5(__VA_ARGS__, FTST_EXPECT)
# define __FTST_TWO_CMP_3(...)           __FTST_TWO_CMP_4(__VA_ARGS__, __FTST_EQ_DEFAULT_TYPE)
# define __FTST_TWO_CMP_2(...)           __FTST_STATIC_ASSERT(0, eq_take_2_or_more_arguments_not_1)
# define __FTST_TWO_CMP_1(...)           __FTST_STATIC_ASSERT(0, eq_take_2_or_more_arguments_not_0)
# define __FTST_TWO_CMP_0()              __FTST_STATIC_ASSERT(0,)
                                         
# define __FTST_ACCUR_CMP_16(...)        __FTST_ACCUR_CMP_REAL(__VA_ARGS__)
# define __FTST_ACCUR_CMP_15(...)        __FTST_ACCUR_CMP_REAL(__VA_ARGS__)
# define __FTST_ACCUR_CMP_14(...)        __FTST_ACCUR_CMP_REAL(__VA_ARGS__)
# define __FTST_ACCUR_CMP_13(...)        __FTST_ACCUR_CMP_REAL(__VA_ARGS__)
# define __FTST_ACCUR_CMP_12(...)        __FTST_ACCUR_CMP_REAL(__VA_ARGS__)
# define __FTST_ACCUR_CMP_11(...)        __FTST_ACCUR_CMP_REAL(__VA_ARGS__)
# define __FTST_ACCUR_CMP_10(...)        __FTST_ACCUR_CMP_REAL(__VA_ARGS__)
# define __FTST_ACCUR_CMP_9(...)         __FTST_ACCUR_CMP_10(__VA_ARGS__, NULL)
# define __FTST_ACCUR_CMP_8(...)         __FTST_ACCUR_CMP_9(__VA_ARGS__, FTST_EXPECT)
# define __FTST_ACCUR_CMP_7(...)         __FTST_ACCUR_CMP_8(__VA_ARGS__, __FTST_EQ_DEFAULT_TYPE)
# define __FTST_ACCUR_CMP_6(...)         __FTST_STATIC_ASSERT(0, eq_take_3_or_more_arguments_not_2)
# define __FTST_ACCUR_CMP_5(...)         __FTST_STATIC_ASSERT(0, eq_take_3_or_more_arguments_not_1)
# define __FTST_ACCUR_CMP_4(...)         __FTST_STATIC_ASSERT(0, eq_take_3_or_more_arguments_not_0)
# define __FTST_ACCUR_CMP_3(...)         __FTST_STATIC_ASSERT(0,)
# define __FTST_ACCUR_CMP_2(...)         __FTST_STATIC_ASSERT(0,)
# define __FTST_ACCUR_CMP_1(...)         __FTST_STATIC_ASSERT(0,)
# define __FTST_ACCUR_CMP_0()            __FTST_STATIC_ASSERT(0,)
                                         
# define __FTST_ONE_CMP_16(...)           __FTST_ONE_CMP_REAL(__VA_ARGS__)
# define __FTST_ONE_CMP_15(...)           __FTST_ONE_CMP_REAL(__VA_ARGS__)
# define __FTST_ONE_CMP_14(...)           __FTST_ONE_CMP_REAL(__VA_ARGS__)
# define __FTST_ONE_CMP_13(...)           __FTST_ONE_CMP_REAL(__VA_ARGS__)
# define __FTST_ONE_CMP_12(...)           __FTST_ONE_CMP_REAL(__VA_ARGS__)
# define __FTST_ONE_CMP_11(...)           __FTST_ONE_CMP_REAL(__VA_ARGS__)
# define __FTST_ONE_CMP_10(...)           __FTST_ONE_CMP_REAL(__VA_ARGS__)
# define __FTST_ONE_CMP_9(...)           __FTST_ONE_CMP_REAL(__VA_ARGS__)
# define __FTST_ONE_CMP_8(...)           __FTST_ONE_CMP_REAL(__VA_ARGS__)
# define __FTST_ONE_CMP_7(...)           __FTST_ONE_CMP_REAL(__VA_ARGS__)
# define __FTST_ONE_CMP_6(...)           __FTST_ONE_CMP_REAL(__VA_ARGS__)
# define __FTST_ONE_CMP_5(...)           __FTST_ONE_CMP_6(__VA_ARGS__, NULL)
# define __FTST_ONE_CMP_4(...)           __FTST_ONE_CMP_5(__VA_ARGS__, FTST_EXPECT)
# define __FTST_ONE_CMP_3(...)           __FTST_ONE_CMP_4(__VA_ARGS__, __FTST_EQ_DEFAULT_TYPE)
# define __FTST_ONE_CMP_2(...)           __FTST_STATIC_ASSERT(0, bool_cmp_take_1_or_more_arguments_not_0)
# define __FTST_ONE_CMP_1(...)           __FTST_STATIC_ASSERT(0,)
# define __FTST_ONE_CMP_0()              __FTST_STATIC_ASSERT(0,)

/********************************************************/
/*						REAL TEST						*/

# define FTST_EQ(...)           __FTST_MULTI_MACRO(__FTST_TWO_CMP, ==, __VA_ARGS__)
# define FTST_NE(...)           __FTST_MULTI_MACRO(__FTST_TWO_CMP, !=, __VA_ARGS__)
# define FTST_LESS(...)         __FTST_MULTI_MACRO(__FTST_TWO_CMP, < , __VA_ARGS__)
# define FTST_LESSEQ(...)       __FTST_MULTI_MACRO(__FTST_TWO_CMP, <=, __VA_ARGS__)
# define FTST_MORE(...)         __FTST_MULTI_MACRO(__FTST_TWO_CMP, > , __VA_ARGS__)
# define FTST_MOREEQ(...)       __FTST_MULTI_MACRO(__FTST_TWO_CMP, >=, __VA_ARGS__)

# define FTST_EQ_A(...)         __FTST_MULTI_MACRO(__FTST_ACCUR_CMP, "==", >=, &&, <=, __VA_ARGS__)
# define FTST_NE_A(...)         __FTST_MULTI_MACRO(__FTST_ACCUR_CMP, "!=", < , ||, > , __VA_ARGS__)

# define FTST_STR_EQ(...)       __FTST_MULTI_MACRO(__FTST_STR_CMP, ==, __VA_ARGS__)
# define FTST_STR_NE(...)       __FTST_MULTI_MACRO(__FTST_STR_CMP, !=, __VA_ARGS__)
# define FTST_STR_LESS(...)     __FTST_MULTI_MACRO(__FTST_STR_CMP, < , __VA_ARGS__)
# define FTST_STR_LESSEQ(...)   __FTST_MULTI_MACRO(__FTST_STR_CMP, <=, __VA_ARGS__)
# define FTST_STR_MORE(...)     __FTST_MULTI_MACRO(__FTST_STR_CMP, > , __VA_ARGS__)
# define FTST_STR_MOREEQ(...)   __FTST_MULTI_MACRO(__FTST_STR_CMP, >=, __VA_ARGS__)

# define FTST_IS_TRUE(...)      __FTST_MULTI_MACRO(__FTST_ONE_CMP,   , "true" , __VA_ARGS__)
# define FTST_IS_FALSE(...)     __FTST_MULTI_MACRO(__FTST_ONE_CMP,  !, "false", __VA_ARGS__)

/*************************************************************
**			Initialization and execution tests				**
*************************************************************/

# ifdef FTST_MAIN_FILE
static void    __ftst_init_table(char const* table_name)
{
    if (table_name != NULL)
    {
        char file_with_exp[512];
        snprintf(file_with_exp, sizeof(file_with_exp), "%s%s", table_name, ".csv");
        __ftst_g.table = fopen(file_with_exp, "w");
        __FTST_RUNTIME_ASSERT(__ftst_g.table != NULL, "Can't create file");
    }
}

static void    __ftst_init_stream(FILE* stream_output)
{
    __ftst_g.stream = stream_output;
}

static void    __ftst_init(FILE* stream_output, char const* table_name)
{
# if FTST_MALLOC_TEST
    __ftst_init_alloc();
# endif
    __ftst_init_stream(stream_output);
    __ftst_init_table(table_name);
}

# define __FTST_INIT_2(stream_output, result_file_name) \
        __ftst_init(stream_output, result_file_name);    \
        __ftst_test_results __ftst_main_test = (__ftst_test_results) { 0, 0, "main"}; \
        __ftst_test_results* __ftst_current = &__ftst_main_test

# define __FTST_INIT_1(stream_output)						__FTST_INIT_2(stream_output, NULL)
# define __FTST_INIT_0()									__FTST_INIT_1(stdout)

# define FTST_INIT(...) __FTST_MULTI_MACRO(__FTST_INIT, __VA_ARGS__)

static int    __ftst_exit(__ftst_test_results __ftst_results)
{
    if (__ftst_g.table)
        fclose(__ftst_g.table);

# if FTST_MALLOC_TEST
    __ftst_exit_alloc();
# endif
    __ftst_g.table   = NULL;
    __ftst_g.stream  = NULL;
    if (__ftst_results.passed == __ftst_results.launched)
        return (0);
    return (-1);
}

# define FTST_EXIT()	__ftst_exit(__ftst_main_test)
# endif

/********************************************************/
/*					PRINT RESULTS						*/

# ifdef FTST_MAIN_FILE

# define __FTST_PRETTY_TEST_STATUS(status, test_case_name) "%-20s : " __FTST_PRETTY_TEST_CASE_NAME("%s"), \
                                                          status,                          test_case_name

# define __FTST_PRETTY_START_TEST(test_case_name)        __FTST_PRETTY_TEST_STATUS( __FTST_PRETTY_PROCESSED( "[launched]"  ), test_case_name)
# define __FTST_PRETTY_SUCCESS_TEST(test_case_name)      __FTST_PRETTY_TEST_STATUS( __FTST_PRETTY_SUCCESS(   "[success]"    ), test_case_name)
# define __FTST_PRETTY_FAILED_TEST(test_case_name)       __FTST_PRETTY_TEST_STATUS( __FTST_PRETTY_FAILED(    "[failed]"     ), test_case_name)

static void    __ftst_pretty_print_start(char const *test_case_name)
{
    __FTST_STREAM_PRINTF(__FTST_PRETTY_START_TEST(test_case_name));
    __FTST_STREAM_PRINTF("\n");
}

static void    __ftst_pretty_print_result(
        char const* test_case_name, __ftst_test_results test, clock_t time)
{
    if (test.passed == test.launched)
        __FTST_STREAM_PRINTF(__FTST_PRETTY_SUCCESS_TEST(test_case_name));
    else
        __FTST_STREAM_PRINTF(__FTST_PRETTY_FAILED_TEST(test_case_name));

    __FTST_STREAM_PRINTF(" | ");

    if (test.passed == test.launched)
        __FTST_STREAM_PRINTF(
            __FTST_PRETTY_SUCCESS("%zu") "/" __FTST_PRETTY_SUCCESS("%zu"),
                                test.passed,                   test.launched);
    else
        __FTST_STREAM_PRINTF(
            __FTST_PRETTY_FAILED("%zu") "/" __FTST_PRETTY_SUCCESS("%zu"),
                                test.passed,                   test.launched);

    __FTST_STREAM_PRINTF(
        " [" __FTST_PRETTY_INFO("%.3fms") "]\n",
                            time / 1000.);
    
}

# endif

/********************************************************/
/*					TEST EXECUTION						*/

static inline clock_t ftst_start_timer() { return clock(); }
static inline clock_t ftst_time(clock_t start) { return clock() - start; }

# ifdef FTST_MAIN_FILE
__ftst_test_results      __ftst_run_test(__ftst_test_t test_case, char const* test_case_name)
{
    clock_t             time;
    __ftst_test_results test_results = (__ftst_test_results){ 0, 0, test_case_name };


    __ftst_pretty_print_start(test_case_name);

    __FTST_FFLUSH_STREAM();
    time = ftst_start_timer();
    test_case(&test_results);
    time = ftst_time(time);

    __ftst_pretty_print_result(test_case_name, test_results, time);


    __FTST_TABLE_PRINTF("%s,%zu/%zu,%.3fms\n",
        test_case_name, test_results.passed, test_results.launched, time);
    return (test_results);
}
# else
extern __ftst_test_results      __ftst_run_test(__ftst_test_t test_case, char const* test_case_name);
# endif

# define FTST_RUNTEST(test_name)                                                                \
    {                                                                                           \
        __ftst_test_results __results =                                                         \
                __ftst_run_test(&__FTST_TEST_CASE(test_name), __FTST_TEST_CASE_NAME(test_name));\
        __ftst_current->passed      += __results.passed;                                        \
        __ftst_current->launched    += __results.launched;                                      \
    }

#endif
