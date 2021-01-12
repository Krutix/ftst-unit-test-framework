#ifndef FT_SPEED_TEST_H
# define FT_SPEED_TEST_H

# include <time.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>

/* Default value settings */
# ifndef        FTST_NO_COLOR
#  define       FTST_NO_COLOR       false
# endif

# ifndef        FTST_NAMESPACE 
#  define       FTST_NAMESPACE      true
# endif

# ifndef        FTST_ERROR_MESSAGE
#  define       FTST_ERROR_MESSAGE  true
# endif

# if !defined(FTST_BUFFER_SIZE) || FTST_BUFFER_SIZE < 1
#  define       FTST_BUFFER_SIZE 128
# endif

typedef struct {
    size_t      passed;
    size_t      launched;
}               __ftst_test;

FILE*       __g_ftst_stream;
FILE*       __g_ftst_table;

# define __FTST_IS_STREAM     (__g_ftst_stream != NULL)
# define __FTST_IS_TABLE      (__g_ftst_table  != NULL)

typedef     void(*__ftst_test_t)(__ftst_test*);

# if FTST_NO_COLOR
#  define __FTST_ANSI_COLOR_RED
#  define __FTST_ANSI_COLOR_GREEN
#  define __FTST_ANSI_COLOR_YELLOW
#  define __FTST_ANSI_COLOR_BLUE
#  define __FTST_ANSI_COLOR_MAGENTA
#  define __FTST_ANSI_COLOR_CYAN
#  define __FTST_ANSI_COLOR_RESET
# else
#  define __FTST_ANSI_COLOR_RED     "\x1b[31m"
#  define __FTST_ANSI_COLOR_GREEN   "\x1b[32m"
#  define __FTST_ANSI_COLOR_YELLOW  "\x1b[33m"
#  define __FTST_ANSI_COLOR_BLUE    "\x1b[34m"
#  define __FTST_ANSI_COLOR_MAGENTA "\x1b[35m"
#  define __FTST_ANSI_COLOR_CYAN    "\x1b[36m"
#  define __FTST_ANSI_COLOR_RESET   "\x1b[0m"
# endif

# define __FTST_PRETTY_PROCESSED(str)       __FTST_ANSI_COLOR_YELLOW    str     __FTST_ANSI_COLOR_RESET
# define __FTST_PRETTY_SUCCESS(str)         __FTST_ANSI_COLOR_GREEN     str     __FTST_ANSI_COLOR_RESET
# define __FTST_PRETTY_FAILED(str)          __FTST_ANSI_COLOR_RED       str     __FTST_ANSI_COLOR_RESET
# define __FTST_PRETTY_INFO(str)            __FTST_ANSI_COLOR_BLUE      str     __FTST_ANSI_COLOR_RESET
# define __FTST_PRETTY_TEST_CASE_NAME(str)  __FTST_ANSI_COLOR_CYAN      str     __FTST_ANSI_COLOR_RESET


# define __FTST_TEST_CASE(test_name)        __ftst_test_case_##test_name
# define __FTST_TEST_CASE_NAME(test_name)   #test_name
# define __FTST_TEST_CASE_NAME_FROM_FUNC    __FUNCTION__ + 17

# define __FTST_FATAL_ERROR(error_message)          __ftst_fatal_error(__LINE__, __FUNCTION__, error_message)
# define __FTST_FATAL_CASE_ERROR(error_message)     __ftst_fatal_error(__LINE__, "test "__FTST_TEST_CASE_NAME_FROM_FUNC, error_message)

static void    __ftst_fatal_error(size_t line, char const* function_name, char const* error_message)
{
    fprintf(stderr,
        "ftst error\n" \
        "%d: [%s] \n", line, function_name, error_message);
    exit(-1);
}

/* MULTI MACRO */
/* allow to use same name for macro with different number of args */
#define __FTST_FUNC_CHOOSER(_f0, _f1, _f2, _f3, _f4, _f5, _f6, _f7, _f8, _f9, _f10, _f11, _f12, _f13, _f14, _f15, _f16, ...) _f16
#define __FTST_FUNC_RECOMPOSER(args_with_parentheses) __FTST_FUNC_CHOOSER args_with_parentheses
#define __FTST_CHOOSE_FROM_ARG_COUNT(F, ...) __FTST_FUNC_RECOMPOSER((__VA_ARGS__, \
            F##_16, F##_15, F##_14, F##_13, F##_12, F##_11, F##_10, F##_9, F##_8,\
            F##_7, F##_6, F##_5, F##_4, F##_3, F##_2, F##_1, ))
#define __FTST_NO_ARG_EXPANDER(FUNC) ,,,,,,,,,,,,,,,,FUNC ## _0
#define __FTST_MACRO_CHOOSER(FUNC, ...) __FTST_CHOOSE_FROM_ARG_COUNT(FUNC, __FTST_NO_ARG_EXPANDER __VA_ARGS__ (FUNC))
#define __FTST_MULTI_MACRO(FUNC, ...) __FTST_MACRO_CHOOSER(FUNC, __VA_ARGS__)(__VA_ARGS__)


# define FTST_TEST(test_name)                    \
void    __FTST_TEST_CASE(test_name)(__ftst_test* test)

# define __FTST_SIMPLE_TEST(cond, error_funct)      \
{                                                       \
    test->launched++;                                   \
    if (cond) {                                         \
        test->passed++;                                 \
    } else {                                            \
        error_funct;                                    \
    }                                                   \
}

#define __FTST_TEST_ERROR(condition, actual, expect) \
        __ftst_test_error(__LINE__, __FTST_TEST_CASE_NAME_FROM_FUNC, \
                    condition, actual, expect)

static void    __ftst_test_error(size_t const line, char const* test_case_name,
                            char const *condition, const char* actual, char const* expect)
{
    if (__FTST_IS_STREAM && FTST_ERROR_MESSAGE)
    {
        fprintf(__g_ftst_stream,
            "'%s' test " __FTST_PRETTY_FAILED("[failed]") \
            "\n%d:\tFrom condition:  " __FTST_PRETTY_INFO("%s") \
            ",   actual:  " __FTST_PRETTY_INFO("%s") \
            ",   expected:  " __FTST_PRETTY_INFO("%s\n"),
                test_case_name, line, condition, actual, expect);
    }
}


# define FTST_EXPECT
# define FTST_ASSERT return;

/* TODO remove auto "%" on format str */
# define __FTST_SNPRINTF(name, size, format, value) \
                    char name[size]; \
                    snprintf(name, sizeof(name), "%"format, value);

# define __FTST_EQ_DEFAULT_FORMAT "d"
# define __FTST_EQ_REAL(cond, expect, error_funct, format) __FTST_SIMPLE_TEST((cond) == (expect), \
                {   __FTST_SNPRINTF(actual_value, FTST_BUFFER_SIZE, format, cond);       \
                    __FTST_SNPRINTF(expect_value, FTST_BUFFER_SIZE, format, expect);     \
                    __FTST_TEST_ERROR(#cond, actual_value, expect_value); } error_funct)

# define __FTST_EQ_0()                                      __FTST_FATAL_CASE_ERROR("EQ take 2 or more arguments, not 0");
# define __FTST_EQ_1(a)                                     __FTST_FATAL_CASE_ERROR("EQ take 2 or more arguments, not 1");
# define __FTST_EQ_2(expr, expect)                          __FTST_EQ_3(expr, expect, FTST_EXPECT)
# define __FTST_EQ_3(expr, expect, error_funct)             __FTST_EQ_4(expr, expect, error_funct, __FTST_EQ_DEFAULT_FORMAT)
# define __FTST_EQ_4(expr, expect, error_funct, format)     __FTST_EQ_REAL(expr, expect, error_funct, format)

# define __FTST_IS_BOOL_REAL(expect, cond, error_funct, format) __FTST_SIMPLE_TEST(cond,  \
                __FTST_SNPRINTF(actual_value, FTST_BUFFER_SIZE, format, cond);  \
                __FTST_TEST_ERROR(#cond, actual_value, expect); error_funct)

# define __FTST_IS_TRUE_0()                             __FTST_FATAL_CASE_ERROR("IS_TRUE take 1 or more arguments, not 0");
# define __FTST_IS_TRUE_1(cond)                         __FTST_IS_TRUE_2(cond, FTST_EXPECT)
# define __FTST_IS_TRUE_2(cond, error_funct)            __FTST_IS_TRUE_3(cond, error_funct, __FTST_EQ_DEFAULT_FORMAT)
# define __FTST_IS_TRUE_3(cond, error_funct, format)    __FTST_IS_BOOL_REAL("true", cond, error_funct, format)

# define __FTST_IS_FALSE_0()                             __FTST_FATAL_CASE_ERROR("IS_TRUE take 1 or more arguments, not 0");
# define __FTST_IS_FALSE_1(cond)                         __FTST_IS_FALSE_2(cond, FTST_EXPECT)
# define __FTST_IS_FALSE_2(cond, error_funct)            __FTST_IS_FALSE_3(cond, error_funct, __FTST_EQ_DEFAULT_FORMAT)
# define __FTST_IS_FALSE_3(cond, error_funct, format)    __FTST_IS_BOOL_REAL("false", !(cond), error_funct, format)

# define FTST_EQ(...)           __FTST_MULTI_MACRO(__FTST_EQ, __VA_ARGS__)
# define FTST_IS_TRUE(...)      __FTST_MULTI_MACRO(__FTST_IS_TRUE, __VA_ARGS__)
# define FTST_IS_FALSE(...)     __FTST_MULTI_MACRO(__FTST_IS_FALSE, __VA_ARGS__)


static void    ftst_init(FILE* stream_output, char const* result_file_name)
{
    __g_ftst_stream = stream_output;

    if (result_file_name != NULL)
    {
        char file_with_exp[512];
        snprintf(file_with_exp, sizeof(file_with_exp), "%s%s", result_file_name, ".csv");
        __g_ftst_table = fopen(file_with_exp, "w");
        if (__g_ftst_table == NULL) __FTST_FATAL_ERROR("The file cant be created");
    }
}

static void    ftst_exit(void)
{
    if (__FTST_IS_TABLE)
        fclose(__g_ftst_table);
    __g_ftst_table = NULL;
    __g_ftst_stream = NULL;
}


# define FTST_RUNTEST(test_name) \
    __ftst_run_test(&__FTST_TEST_CASE(test_name), __FTST_TEST_CASE_NAME(test_name))

static clock_t __ftst_start_timer() { return clock(); }
static clock_t __ftst_end_timer(clock_t start) { return clock() - start; }

static void    __ftst_pretty_print_start(char const* test_case_name)
{
    if (__FTST_IS_STREAM)
        fprintf(__g_ftst_stream,
            __FTST_PRETTY_PROCESSED("[processed]") " : " __FTST_PRETTY_TEST_CASE_NAME("%s") "\n",
                                                                            test_case_name);
}

static void    __ftst_pretty_print_result( \
        char const* test_case_name, __ftst_test test, clock_t time)
{
    if (__FTST_IS_STREAM)
    {
        if (test.passed == test.launched)
            fprintf(__g_ftst_stream, __FTST_PRETTY_SUCCESS("[success]"));
        else
            fprintf(__g_ftst_stream, __FTST_PRETTY_FAILED("[failed]"));

        fprintf(__g_ftst_stream,
            " : " __FTST_PRETTY_TEST_CASE_NAME("%s") " | ",
                                        test_case_name);

        if (test.passed == test.launched)
            fprintf(__g_ftst_stream,
                __FTST_PRETTY_SUCCESS("%zu") "/" __FTST_PRETTY_SUCCESS("%zu"),
                                    test.passed,                   test.launched);
        else
            fprintf(__g_ftst_stream,
                __FTST_PRETTY_FAILED("%zu") "/" __FTST_PRETTY_SUCCESS("%zu"),
                                    test.passed,                   test.launched);

        fprintf(__g_ftst_stream,
            " [" __FTST_PRETTY_INFO("%.3fms") "]\n",
                                time / 1000.);
    }
}

static void __ftst_pretty_print_table(char const* test_case_name, __ftst_test test, clock_t time)
{
    if (__FTST_IS_TABLE)
    {
        fprintf(__g_ftst_table, "%s,%d/%d,%.3fms\n",
            test_case_name, test.passed, test.launched, time / 1000.);
    }
}

static void    __ftst_run_test(__ftst_test_t test_case, char const* test_case_name)
{
    clock_t     time;
    __ftst_test test = (__ftst_test){ 0, 0 };

    __ftst_pretty_print_start(test_case_name);

    time = __ftst_start_timer();
    test_case(&test);
    time = __ftst_end_timer(time);

    __ftst_pretty_print_result(test_case_name, test, time);

    __ftst_pretty_print_table(test_case_name, test, time);
}


# if FTST_NAMESPACE
#  define EQ        FTST_EQ
#  define IS_TRUE   FTST_IS_TRUE
#  define IS_FALSE  FTST_IS_FALSE

#  define EXPECT    FTST_EXPECT
#  define ASSERT    FTST_ASSERT

#  define TEST      FTST_TEST
#  define RUNTEST   FTST_RUNTEST
# endif

#endif
