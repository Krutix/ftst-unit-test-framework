#ifndef FTST_H
# define FTST_H

# include <time.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <stdint.h>
# include <stdbool.h>

/*************************************************
**              Default settings				**
*************************************************/

# ifndef        FTST_COLOR
#  define       FTST_COLOR          true
# endif
# ifndef        FTST_NAMESPACE 
#  define       FTST_NAMESPACE      true
# endif
# ifndef        FTST_SILENT
#  define       FTST_SILENT         false
# endif
# ifndef        FTST_ERROR_MESSAGE
#  define       FTST_ERROR_MESSAGE  true
# endif
# ifndef        FTST_ALLOC_TEST
#  define       FTST_ALLOC_TEST     true
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

# ifdef     FTST_SUB_TEST
#  define   __FTST_EXTERN   extern
# else
#  define   __FTST_EXTERN
# endif

/************************************************/
/*					COLORS						*/

# if FTST_COLOR
#  define __FTST_ANSI_COLOR_RED     "\x1b[31m"
#  define __FTST_ANSI_COLOR_GREEN   "\x1b[32m"
#  define __FTST_ANSI_COLOR_YELLOW  "\x1b[33m"
#  define __FTST_ANSI_COLOR_BLUE    "\x1b[34m"
#  define __FTST_ANSI_COLOR_MAGENTA "\x1b[35m"
#  define __FTST_ANSI_COLOR_CYAN    "\x1b[36m"
#  define __FTST_ANSI_COLOR_RESET   "\x1b[0m"
# else
#  define __FTST_ANSI_COLOR_RED
#  define __FTST_ANSI_COLOR_GREEN
#  define __FTST_ANSI_COLOR_YELLOW
#  define __FTST_ANSI_COLOR_BLUE
#  define __FTST_ANSI_COLOR_MAGENTA
#  define __FTST_ANSI_COLOR_CYAN
#  define __FTST_ANSI_COLOR_RESET
# endif

# define __FTST_PRETTY_PROCESSED(str)       __FTST_ANSI_COLOR_YELLOW    str     __FTST_ANSI_COLOR_RESET
# define __FTST_PRETTY_SUCCESS(str)         __FTST_ANSI_COLOR_GREEN     str     __FTST_ANSI_COLOR_RESET
# define __FTST_PRETTY_FAILED(str)          __FTST_ANSI_COLOR_RED       str     __FTST_ANSI_COLOR_RESET
# define __FTST_PRETTY_INFO(str)            __FTST_ANSI_COLOR_BLUE      str     __FTST_ANSI_COLOR_RESET
# define __FTST_PRETTY_TEST_CASE_NAME(str)  __FTST_ANSI_COLOR_CYAN      str     __FTST_ANSI_COLOR_RESET

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
# define FTST_STATIC_ASSERT(expr, error_message)                \
		__FTST_STATIC_ASSERT(expr, error_message)
# else
#  define FTST_STATIC_ASSERT(expr, error_message)
# endif

/*****************************************************
**                  ALLOCATION TEST					**
*****************************************************/

# if        FTST_ALLOC_TEST

#  if   defined(__linux__)
#   include <malloc.h>
#   define _GNU_SOURCE
#   define __USE_GNU
#  elif defined(__APPLE__)
#   include <malloc/malloc.h>
#  endif
#  include <dlfcn.h>
/*
** On linux use -ldl flag to link dlfcn lib
*/

__FTST_EXTERN void*(*libc_malloc)(size_t);
__FTST_EXTERN void(*libc_free)(void*);

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

#  define   __FTST_ALLOC_SET(value, test)       \
    {                                           \
        test.is_active = true;                  \
        test.d = value;                         \
    }

#  define   __FTST_ALLOC_CLEAN(test)            \
    {                                           \
        test.is_active = false;                 \
    }

#  define   FTST_ALLOC_COUNTER_SET(value)           __FTST_ALLOC_SET(value, __ftst_alloc_test.fail_counter)
#  define   FTST_ALLOC_COUNTER_CLEAN(value)         __FTST_ALLOC_CLEAN(__ftst_alloc_test.fail_counter)

#  define   FTST_ALLOC_LIMIT_SET(value)             __FTST_ALLOC_SET(value, __ftst_alloc_test.limit)
#  define   FTST_ALLOC_LIMIT_CLEAN(value)           __FTST_ALLOC_CLEAN(__ftst_alloc_test.limit)

#  define   FTST_ALLOC_SINGLE_LIMIT_SET(value)      __FTST_ALLOC_SET(value, __ftst_alloc_test.single_limit)
#  define   FTST_ALLOC_SINGLE_LIMIT_CLEAN(value)    __FTST_ALLOC_CLEAN(__ftst_alloc_test.single_limit)

#  define   FTST_ALLOC_IF_ERROR(check)          \
    if (__ftst_alloc_test.error_happend)        \
    {                                           \
        check;                                  \
    }

#  if       FTST_NAMESPACE
#   define  ALLOC_COUNTER_SET           FTST_ALLOC_COUNTER_SET
#   define  ALLOC_COUNTER_CLEAN         FTST_ALLOC_COUNTER_CLEAN

#   define  ALLOC_LIMIT_SET             FTST_ALLOC_LIMIT_SET
#   define  ALLOC_LIMIT_CLEAN           FTST_ALLOC_LIMIT_CLEAN

#   define  ALLOC_SINGLE_LIMIT_SET      FTST_ALLOC_SINGLE_LIMIT_SET
#   define  ALLOC_SINGLE_LIMIT_CLEAN    FTST_ALLOC_SINGLE_LIMIT_CLEAN

#   define  ALLOC_IF_ERROR              FTST_ALLOC_IF_ERROR         
#  endif

#  ifndef   FTST_SUB_TEST



static void*   __ftst_malloc_error()
{
    __ftst_alloc_test.error_happend = true;
    return (NULL);
}

void*   malloc(size_t size)
{
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

    return (libc_malloc(size));
}

void    free(void *p)
{
    libc_free(p);
}

#  endif
# endif

/*******************************************************
*******************************************************/

/************************************************/
/*					global data					*/

typedef struct {
    size_t      passed;
    size_t      launched;
}               __ftst_test_results;

typedef struct {
    FILE*       		stream;
    FILE*       		table;
    __ftst_test_results	test_results;
    char const*			current_test;
}               __ftst_global;

__FTST_EXTERN __ftst_global __ftst_global_test;


# define __FTST_IS_STREAM     (__ftst_global_test.stream != NULL)
# define __FTST_IS_TABLE      (__ftst_global_test.table  != NULL)

/*************************************************/

# define __FTST_TEST_CASE(test_name)        __ftst_test_case_##test_name
# define __FTST_TEST_CASE_NAME(test_name)   #test_name

# define __FTST_FATAL_ERROR(error_message)          __ftst_fatal_error(__LINE__, __FUNCTION__, error_message)
# define __FTST_FATAL_CASE_ERROR(error_message)     __ftst_fatal_error(__LINE__, __ftst_global_test.current_test, error_message)

static void    __ftst_fatal_error(size_t line, char const* function_name, char const* error_message)
{
    fprintf(stderr,
        __FTST_PRETTY_FAILED("ftst error\n") "%zu: [%s] - \"%s\" \n",
		line, function_name, error_message);
    exit(-1);
}

/*
**                           MULTI MACRO
**  allow to use same name for macro with different number of args
**
**  __FTST_MULTI_MACRO choose macro named FUNC and postfix with number of args
**
**  example:
**  __FTST_MULTI_MACRO(MY_MACRO, __VA_ARGS__)
**  MY_MACRO_1(a)
**  MY_MACRO_2(a, b)
**  MY_MACRO_3(a, b, c)
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
# define __FTST_TYPE_g                  double
# define __FTST_TYPE_Lg                 long double
# define __FTST_TYPE_f                  double
# define __FTST_TYPE_Lf                 long double
# define __FTST_TYPE_c                  char
# define __FTST_TYPE_lc                 wchar_t
# define __FTST_TYPE_s                  char*
# define __FTST_TYPE_ls                 wchar_t*

/*****************************************************
**              	TEST CASE						**
*****************************************************/

typedef     void(*__ftst_test_t)();

# define FTST_TEST(test_name)                           \
void    __FTST_TEST_CASE(test_name)(void)

/****************************************************/
/*					TEST BRANCH						*/

# define __FTST_SIMPLE_TEST(cond, error_funct)          \
{                                                       \
    __ftst_global_test.test_results.launched++;         \
    if (cond) {                                         \
        __ftst_global_test.test_results.passed++;       \
    } else {                                            \
        error_funct;                                    \
    }                                                   \
}

/****************************************************/
/*					ERROR MESSAGE					*/

# if FTST_ERROR_MESSAGE
#  define __FTST_TEST_ERROR(test_name, actual, actual_str, expect, expect_str) \
        __ftst_test_error(__LINE__, __ftst_global_test.current_test, \
                    test_name, actual, actual_str, expect, expect_str)
# else
#  define __FTST_TEST_ERROR(test_name, actual, actual_str, expect, expect_str)
# endif

/*TODO think about naming expression, condition etc*/
static void    __ftst_test_error(size_t const line, char const* test_case_name, char const* test_name,
                            char const *actual, const char* actual_value, char const* expect, char const* expect_value)
{
    if (__FTST_IS_STREAM)
    {
        fprintf(__ftst_global_test.stream,
            "["__FTST_PRETTY_INFO("%s")"] test from '%s' [" __FTST_PRETTY_FAILED("failed")"]" \
            "\n%zu:\t" "actual: " __FTST_PRETTY_INFO("%s")"[%s]",
                test_name, test_case_name, line, actual_value, actual);
        if (expect != NULL)
        {
            fprintf(__ftst_global_test.stream,
                ",   expected: " __FTST_PRETTY_INFO("%s")"[%s]",
                expect_value, expect);
        }
        fprintf(__ftst_global_test.stream, "\n");
    }
}

/****************************************************/
/*					TEST TEMPLATES					*/

# define FTST_EXPECT
# define FTST_ASSERT return;

# define __FTST_STR_CMP_REAL(actual, operation, expect, error_funct)                                        \
        {                                                                                                   \
            char const* actual_v = actual;                                                                  \
            char const* expect_v = expect;                                                                  \
            __FTST_SIMPLE_TEST(strcmp(actual_v, expect_v) operation 0,                                      \
                            __FTST_TEST_ERROR(#operation, #actual, actual_v, #expect, expect_v);            \
                            error_funct)                                                                    \
        }

# define __FTST_TWO_CMP_REAL(actual, operation, expect, error_funct, type)                                  \
        {                                                                                                   \
            __FTST_GET_TYPE(type) actual_v = actual;                                                        \
            __FTST_GET_TYPE(type) expect_v = expect;                                                        \
            __FTST_SIMPLE_TEST((actual_v) operation (expect_v),                                             \
                            __FTST_SNPRINTF(actual_str, FTST_VAR_STR_BUFFER, "%"#type, actual_v);           \
                            __FTST_SNPRINTF(expect_str, FTST_VAR_STR_BUFFER, "%"#type, expect_v);           \
                            __FTST_TEST_ERROR(#operation, #actual, actual_str, #expect, expect_str);        \
                            error_funct)                                                                    \
        }

# define __FTST_ONE_CMP_REAL(actual, operation, name, error_funct, type)                                    \
        {                                                                                                   \
            __FTST_GET_TYPE(type) actual_v = actual;                                                        \
            __FTST_SIMPLE_TEST(operation(actual_v),                                                         \
                            __FTST_SNPRINTF(actual_str, FTST_VAR_STR_BUFFER, "%"#type, actual_v);           \
                            __FTST_TEST_ERROR(name, #actual, actual_str, NULL, NULL);                       \
                            error_funct)                                                                    \
        }

# define __FTST_STR_CMP_4(operator, actual, expect, error_funct)                      __FTST_STR_CMP_REAL(actual, operator, expect, error_funct)
# define __FTST_STR_CMP_3(operator, actual, expect)                                   __FTST_STR_CMP_4(operator, actual, expect, FTST_EXPECT)
# define __FTST_STR_CMP_2(a, b)                                                       __FTST_STATIC_ASSERT(0, str_eq_take_2_or_more_arguments_not_1)
# define __FTST_STR_CMP_1(a)                                                          __FTST_STATIC_ASSERT(0, str_eq_take_2_or_more_arguments_not_0)
# define __FTST_STR_CMP_0()                                                           __FTST_STATIC_ASSERT(0,)

# define __FTST_TWO_CMP_5(operator, actual, expect, type, error_funct)                __FTST_TWO_CMP_REAL(actual, operator, expect, error_funct, type)
# define __FTST_TWO_CMP_4(operator, actual, expect, type)                             __FTST_TWO_CMP_5(operator, actual, expect, type, FTST_EXPECT)
# define __FTST_TWO_CMP_3(operator, actual, expect)                                   __FTST_TWO_CMP_4(operator, actual, expect, __FTST_EQ_DEFAULT_TYPE)
# define __FTST_TWO_CMP_2(a, b)                                                       __FTST_STATIC_ASSERT(0, eq_take_2_or_more_arguments_not_1)
# define __FTST_TWO_CMP_1(a)                                                          __FTST_STATIC_ASSERT(0, eq_take_2_or_more_arguments_not_0)
# define __FTST_TWO_CMP_0()                                                           __FTST_STATIC_ASSERT(0,)

# define __FTST_ONE_CMP_5(operator, name, actual, type, error_funct)                  __FTST_ONE_CMP_REAL(actual, operator, name, error_funct, type)
# define __FTST_ONE_CMP_4(operator, name, actual, type)                               __FTST_ONE_CMP_5(operator, name, actual, type, FTST_EXPECT)
# define __FTST_ONE_CMP_3(operator, name, actual)                                     __FTST_ONE_CMP_4(operator, name, actual, __FTST_EQ_DEFAULT_TYPE)
# define __FTST_ONE_CMP_2(a, b)                                                       __FTST_STATIC_ASSERT(0, bool_cmp_take_1_or_more_arguments_not_0)
# define __FTST_ONE_CMP_1(a)                                                          __FTST_STATIC_ASSERT(0,)
# define __FTST_ONE_CMP_0()                                                           __FTST_STATIC_ASSERT(0,)

/********************************************************/
/*						REAL TEST						*/

# define FTST_EQ(...)           __FTST_MULTI_MACRO(__FTST_TWO_CMP, ==, __VA_ARGS__)
# define FTST_NE(...)           __FTST_MULTI_MACRO(__FTST_TWO_CMP, !=, __VA_ARGS__)
# define FTST_LESS(...)         __FTST_MULTI_MACRO(__FTST_TWO_CMP, < , __VA_ARGS__)
# define FTST_LESSEQ(...)       __FTST_MULTI_MACRO(__FTST_TWO_CMP, <=, __VA_ARGS__)
# define FTST_MORE(...)         __FTST_MULTI_MACRO(__FTST_TWO_CMP, > , __VA_ARGS__)
# define FTST_MOREEQ(...)       __FTST_MULTI_MACRO(__FTST_TWO_CMP, >=, __VA_ARGS__)

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

static void    __ftst_init(FILE* stream_output, char const* result_file_name)
{
# if FTST_ALLOC_TEST
    libc_malloc = dlsym(RTLD_NEXT, "malloc");
    libc_free   = dlsym(RTLD_NEXT, "free");
# endif

    __ftst_global_test.stream = stream_output;

    if (result_file_name != NULL)
    {
        char file_with_exp[512];
        snprintf(file_with_exp, sizeof(file_with_exp), "%s%s", result_file_name, ".csv");
        __ftst_global_test.table = fopen(file_with_exp, "w");
        if (__ftst_global_test.table == NULL) __FTST_FATAL_ERROR("The file cant be created");
    }
}

# define __FTST_INIT_2(stream_output, result_file_name)		__ftst_init(stream_output, result_file_name)
# define __FTST_INIT_1(stream_output)						__FTST_INIT_2(stream_output, NULL)
# define __FTST_INIT_0()									__FTST_INIT_1(stdout)

# define FTST_INIT(...) __FTST_MULTI_MACRO(__FTST_INIT, __VA_ARGS__)

static void    __ftst_exit(void)
{
    if (__FTST_IS_TABLE)
        fclose(__ftst_global_test.table);

    __ftst_global_test.table   = NULL;
    __ftst_global_test.stream  = NULL;
}

# define FTST_EXIT()	__ftst_exit()

/********************************************************/
/*					PRINT RESULTS						*/

static void    __ftst_pretty_print_start(char const* test_case_name)
{
# if !(FTST_SILENT)
    if (__FTST_IS_STREAM)
        fprintf(__ftst_global_test.stream,
            __FTST_PRETTY_PROCESSED("[processed]") " : " __FTST_PRETTY_TEST_CASE_NAME("%s") "\n",
                                                                            test_case_name);
# endif
}

static void    __ftst_pretty_print_result(
        char const* test_case_name, __ftst_test_results test, clock_t time)
{
# if !(FTST_SILENT)
    if (__FTST_IS_STREAM)
    {
        if (test.passed == test.launched)
            fprintf(__ftst_global_test.stream, __FTST_PRETTY_SUCCESS("[success]"));
        else
            fprintf(__ftst_global_test.stream, __FTST_PRETTY_FAILED("[failed]"));

        fprintf(__ftst_global_test.stream,
            " : " __FTST_PRETTY_TEST_CASE_NAME("%s") " | ",
                                        test_case_name);

        if (test.passed == test.launched)
            fprintf(__ftst_global_test.stream,
                __FTST_PRETTY_SUCCESS("%zu") "/" __FTST_PRETTY_SUCCESS("%zu"),
                                    test.passed,                   test.launched);
        else
            fprintf(__ftst_global_test.stream,
                __FTST_PRETTY_FAILED("%zu") "/" __FTST_PRETTY_SUCCESS("%zu"),
                                    test.passed,                   test.launched);

        fprintf(__ftst_global_test.stream,
            " [" __FTST_PRETTY_INFO("%.3fms") "]\n",
                                time / 1000.);
    }
# endif
}

static void __ftst_pretty_print_table(
		char const* test_case_name, __ftst_test_results test, clock_t time)
{
    if (__FTST_IS_TABLE)
    {
        fprintf(__ftst_global_test.table, "%s,%zu/%zu,%.3fms\n",
            test_case_name, test.passed, test.launched, time / 1000.);
    }
}

/********************************************************/
/*					TEST EXECUTION						*/

# define FTST_RUNTEST(test_name) \
    __ftst_run_test(&__FTST_TEST_CASE(test_name), __FTST_TEST_CASE_NAME(test_name))

static clock_t ftst_start_timer() { return clock(); }
static clock_t ftst_time(clock_t start) { return clock() - start; }

static void    __ftst_run_test(__ftst_test_t test_case, char const* test_case_name)
{
    clock_t     time;

    __ftst_global_test.test_results = (__ftst_test_results){ 0, 0 };
    __ftst_global_test.current_test = test_case_name;

    __ftst_pretty_print_start(test_case_name);

    time = ftst_start_timer();
    test_case();
    time = ftst_time(time);

    __ftst_pretty_print_result(test_case_name, __ftst_global_test.test_results, time);
    __ftst_pretty_print_table(test_case_name, __ftst_global_test.test_results, time);

    __ftst_global_test.current_test = NULL;
}

#endif
