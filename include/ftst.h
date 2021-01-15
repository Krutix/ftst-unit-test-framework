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

# if !defined(FTST_VAR_STR_BUFFER) || FTST_VAR_STR_BUFFER < 1
#  define       FTST_VAR_STR_BUFFER 128
# endif

typedef struct {
    size_t      passed;
    size_t      launched;
}               __ftst_test;

FILE*       __g_ftst_stream;
FILE*       __g_ftst_table;
__ftst_test __g_ftst_test_results;

# define __FTST_IS_STREAM     (__g_ftst_stream != NULL)
# define __FTST_IS_TABLE      (__g_ftst_table  != NULL)

char const* __g_ftst_current_test;

typedef     void(*__ftst_test_t)();

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


# ifndef NAMESPACE_FTST
#  define EQ        FTST_EQ
#  define NE        FTST_NE
#  define LESS      FTST_LESS
#  define LESSEQ    FTST_LESSEQ
#  define MORE      FTST_MORE
#  define MOREEQ    FTST_MOREEQ

#  define STR_EQ     FTST_STR_EQ
#  define STR_NE     FTST_STR_NE
#  define STR_LESS   FTST_STR_LESS
#  define STR_LESSEQ FTST_STR_LESSEQ
#  define STR_MORE   FTST_STR_MORE
#  define STR_MOREEQ FTST_STR_MOREEQ

#  define IS_TRUE   FTST_IS_TRUE
#  define IS_FALSE  FTST_IS_FALSE

#  define EXPECT    FTST_EXPECT
#  define ASSERT    FTST_ASSERT

#  define TEST      FTST_TEST
#  define RUNTEST   FTST_RUNTEST
# endif


# define __FTST_PRETTY_PROCESSED(str)       __FTST_ANSI_COLOR_YELLOW    str     __FTST_ANSI_COLOR_RESET
# define __FTST_PRETTY_SUCCESS(str)         __FTST_ANSI_COLOR_GREEN     str     __FTST_ANSI_COLOR_RESET
# define __FTST_PRETTY_FAILED(str)          __FTST_ANSI_COLOR_RED       str     __FTST_ANSI_COLOR_RESET
# define __FTST_PRETTY_INFO(str)            __FTST_ANSI_COLOR_BLUE      str     __FTST_ANSI_COLOR_RESET
# define __FTST_PRETTY_TEST_CASE_NAME(str)  __FTST_ANSI_COLOR_CYAN      str     __FTST_ANSI_COLOR_RESET


# define __FTST_TEST_CASE(test_name)        __ftst_test_case_##test_name
# define __FTST_TEST_CASE_NAME(test_name)   #test_name

# define __FTST_FATAL_ERROR(error_message)          __ftst_fatal_error(__LINE__, __FUNCTION__, error_message)
# define __FTST_FATAL_CASE_ERROR(error_message)     __ftst_fatal_error(__LINE__, __g_ftst_current_test, error_message)

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

# ifndef FTST_BUFFER_SIZE
#  define FTST_BUFFER_SIZE 128
# endif

# define __FTST_SNPRINTF(name, size, format, value) \
                    char name[size]; \
                    snprintf(name, sizeof(name), format, value);

/* Type define */
# define __FTST_EQ_DEFAULT_TYPE d

# define __FTST_GET_TYPE(type) __FTST_TYPE_##type

# define __FTST_TYPE_d                  int
# define __FTST_TYPE_i                  __FTST_TYPE_d
# define __FTST_TYPE_ld                 long __FTST_TYPE_d
# define __FTST_TYPE_li                 long __FTST_TYPE_i
# define __FTST_TYPE_lld                long long __FTST_TYPE_d
# define __FTST_TYPE_lli                long long __FTST_TYPE_i
# define __FTST_TYPE_u                  unsigned int
# define __FTST_TYPE_lu                 long unsigned int
# define __FTST_TYPE_llu                long long unsigned int
# define __FTST_TYPE_z                  size_t
# define __FTST_TYPE_ff                 float
# define __FTST_TYPE_f                  double
# define __FTST_TYPE_Lf                 long double
# define __FTST_TYPE_p                  __intptr_t
# define __FTST_TYPE_c                  char
# define __FTST_TYPE_lc                 wchar_t
# define __FTST_TYPE_s                  char*
# define __FTST_TYPE_ls                 wchar_t*

# define FTST_TEST(test_name)                    \
void    __FTST_TEST_CASE(test_name)()

# define __FTST_SIMPLE_TEST(cond, error_funct)      \
{                                                       \
    __g_ftst_test_results.launched++;                                   \
    if (cond) {                                         \
        __g_ftst_test_results.passed++;                                 \
    } else {                                            \
        error_funct;                                    \
    }                                                   \
}

# if FTST_ERROR_MESSAGE
#  define __FTST_TEST_ERROR(test_name, actual, actual_str, expect, expect_str) \
        __ftst_test_error(__LINE__, __g_ftst_current_test, \
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
        fprintf(__g_ftst_stream,
            "["__FTST_PRETTY_INFO("%s")"] test from '%s' [" __FTST_PRETTY_FAILED("failed")"]" \
            "\n%d:\t" "actual: " __FTST_PRETTY_INFO("%s")"[%s]",
                test_name, test_case_name, line, actual_value, actual);
        if (expect != NULL)
        {
            fprintf(__g_ftst_stream,
                ",   expected: " __FTST_PRETTY_INFO("%s")"[%s]",
                expect_value, expect);
        }
        fprintf(__g_ftst_stream, "\n");
    }
}


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

# define __FTST_TWO_CMP_REAL(actual, operation, expect, error_funct, t_actual, t_expect)                    \
        {                                                                                                   \
            __FTST_GET_TYPE(t_actual) actual_v = actual;                                                    \
            __FTST_GET_TYPE(t_expect) expect_v = expect;                                                    \
            __FTST_SIMPLE_TEST(actual_v operation expect_v,                                                 \
                            __FTST_SNPRINTF(actual_str, FTST_VAR_STR_BUFFER, "%"#t_actual, actual_v);       \
                            __FTST_SNPRINTF(expect_str, FTST_VAR_STR_BUFFER, "%"#t_expect, expect_v);       \
                            __FTST_TEST_ERROR(#operation, #actual, actual_str, #expect, expect_str);        \
                            error_funct)                                                                    \
        }

# define __FTST_ONE_CMP_REAL(actual, operation, name, error_funct, t_actual)                                \
        {                                                                                                   \
            __FTST_GET_TYPE(t_actual) actual_v = actual;                                                    \
            __FTST_SIMPLE_TEST(operation(actual_v),                                                         \
                            __FTST_SNPRINTF(actual_str, FTST_VAR_STR_BUFFER, "%"#t_actual, actual_v);       \
                            __FTST_TEST_ERROR(name, #actual, actual_str, NULL, NULL);                       \
                            error_funct)                                                                    \
        }

# define __FTST_STR_CMP_0()                                                           __FTST_FATAL_CASE_ERROR("EQ take 2 or more arguments, not 0");
# define __FTST_STR_CMP_1(a)                                                          __FTST_FATAL_CASE_ERROR("EQ take 2 or more arguments, not 0");
# define __FTST_STR_CMP_2(a, b)                                                       __FTST_FATAL_CASE_ERROR("EQ take 2 or more arguments, not 1");
# define __FTST_STR_CMP_3(operator, actual, expect)                                   __FTST_STR_CMP_4(operator, actual, expect, FTST_EXPECT)
# define __FTST_STR_CMP_4(operator, actual, expect, error_funct)                      __FTST_STR_CMP_REAL(actual, operator, expect, error_funct)

# define __FTST_TWO_CMP_0()                                                           __FTST_FATAL_CASE_ERROR("EQ take 2 or more arguments, not 0");
# define __FTST_TWO_CMP_1(a)                                                          __FTST_FATAL_CASE_ERROR("EQ take 2 or more arguments, not 0");
# define __FTST_TWO_CMP_2(a, b)                                                       __FTST_FATAL_CASE_ERROR("EQ take 2 or more arguments, not 1");
# define __FTST_TWO_CMP_3(operator, actual, expect)                                   __FTST_TWO_CMP_4(operator, actual, expect, FTST_EXPECT)
# define __FTST_TWO_CMP_4(operator, actual, expect, error_funct)                      __FTST_TWO_CMP_5(operator, actual, expect, error_funct, __FTST_EQ_DEFAULT_TYPE)
# define __FTST_TWO_CMP_5(operator, actual, expect, error_funct, type)                __FTST_TWO_CMP_6(operator, actual, expect, error_funct, type, type)
# define __FTST_TWO_CMP_6(operator, actual, expect, error_funct, t_actual, t_expect)  __FTST_TWO_CMP_REAL(actual, operator, expect, error_funct, t_actual, t_expect)

# define __FTST_ONE_CMP_0()                                                           __FTST_FATAL_CASE_ERROR("IS_TRUE take 1 or more arguments, not 0");
# define __FTST_ONE_CMP_1(a)                                                          __FTST_FATAL_CASE_ERROR("IS_TRUE take 1 or more arguments, not 0");
# define __FTST_ONE_CMP_2(a, b)                                                       __FTST_FATAL_CASE_ERROR("IS_TRUE take 1 or more arguments, not 0");
# define __FTST_ONE_CMP_3(operator, name, actual)                                     __FTST_ONE_CMP_4(operator, name, actual, FTST_EXPECT)
# define __FTST_ONE_CMP_4(operator, name, actual, error_funct)                        __FTST_ONE_CMP_5(operator, name, actual, error_funct, __FTST_EQ_DEFAULT_TYPE)
# define __FTST_ONE_CMP_5(operator, name, actual, error_funct, type)                  __FTST_ONE_CMP_REAL(actual, operator, name, error_funct, type)

# define FTST_EQ(...)           __FTST_MULTI_MACRO(__FTST_TWO_CMP, ==, __VA_ARGS__)
# define FTST_NE(...)           __FTST_MULTI_MACRO(__FTST_TWO_CMP, !=, __VA_ARGS__)
# define FTST_LESS(...)         __FTST_MULTI_MACRO(__FTST_TWO_CMP, < , __VA_ARGS__)
# define FTST_LESSEQ(...)       __FTST_MULTI_MACRO(__FTST_TWO_CMP, <=, __VA_ARGS__)
# define FTST_MORE(...)         __FTST_MULTI_MACRO(__FTST_TWO_CMP, > , __VA_ARGS__)
# define FTST_MOREEQ(...)       __FTST_MULTI_MACRO(__FTST_TWO_CMP, >=, __VA_ARGS__)

# define FTST_STR_EQ(...)        __FTST_MULTI_MACRO(__FTST_STR_CMP, ==, __VA_ARGS__)
# define FTST_STR_NE(...)        __FTST_MULTI_MACRO(__FTST_STR_CMP, !=, __VA_ARGS__)
# define FTST_STR_LESS(...)      __FTST_MULTI_MACRO(__FTST_STR_CMP, < , __VA_ARGS__)
# define FTST_STR_LESSEQ(...)    __FTST_MULTI_MACRO(__FTST_STR_CMP, <=, __VA_ARGS__)
# define FTST_STR_MORE(...)      __FTST_MULTI_MACRO(__FTST_STR_CMP, > , __VA_ARGS__)
# define FTST_STR_MOREEQ(...)    __FTST_MULTI_MACRO(__FTST_STR_CMP, >=, __VA_ARGS__)

# define FTST_IS_TRUE(...)      __FTST_MULTI_MACRO(__FTST_ONE_CMP,   , "true" , __VA_ARGS__)
# define FTST_IS_FALSE(...)     __FTST_MULTI_MACRO(__FTST_ONE_CMP,  !, "false", __VA_ARGS__)


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

    __g_ftst_test_results = (__ftst_test){ 0, 0 };
    __g_ftst_current_test = test_case_name;
    __ftst_pretty_print_start(test_case_name);

    time = __ftst_start_timer();
    test_case();
    time = __ftst_end_timer(time);

    __ftst_pretty_print_result(test_case_name, __g_ftst_test_results, time);

    __ftst_pretty_print_table(test_case_name, __g_ftst_test_results, time);
    __g_ftst_current_test = NULL;
}

#endif
