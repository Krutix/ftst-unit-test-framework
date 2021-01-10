#ifndef FT_SPEED_TEST_H
# define FT_SPEED_TEST_H

# include <time.h>
# include <stdio.h>
# include <stdbool.h>

typedef struct {
    size_t      passed;
    size_t      launched;
}               __ftst_test;

FILE*       __g_ftst_stream;
FILE*       __g_ftst_table;
typedef     void(*__ftst_test_t)(__ftst_test*);

# ifdef FTST_PRETTY
# endif

# define __FTST_TEST_CASE(test_name) __ftst_test_case_##test_name
# define __FTST_TEST_CASE_NAME(test_name) #test_name
# define __FTST_TEST_CASE_NAME_FROM_FUNC __FUNCTION__ + 17

# define FTST_TEST(test_name)                    \
void    __FTST_TEST_CASE(test_name)(__ftst_test* test)

# define __FTST_SIMPLE_TEST(cond, else_funct)      \
{                                                       \
    test->launched++;                                   \
    if (cond) {                                         \
        test->passed++;                                 \
    } else {                                            \
        else_funct;                                     \
    }                                                   \
}

#define __FTST_TEST_ERROR(condition, actual, expect) \
        __ftst_test_error(__LINE__, __FTST_TEST_CASE_NAME_FROM_FUNC, \
                    condition, actual, expect)

void    __ftst_test_error(size_t const line, char const* test_case_name,
                            char const *condition, const char* actual, char const* expect)
{
    fprintf(__g_ftst_stream, "'%s' test failed\n%d:\tFrom condition: %s, actual: %s, expected: %s\n",
                    test_case_name, line, condition, actual, expect);
}


# define __FTST_EQ_DEFAULT(cond, expected, else_funct,...) __FTST_EQ_FORMAT(cond, expected, else_funct, d)
# define __FTST_EQ_FORMAT(cond, expected, else_funct, format,...) __FTST_SIMPLE_TEST((cond) == (expected), \
                { char actual_value[124]; snprintf(actual_value, sizeof(actual_value), "%"#format, cond);       \
                char expected_value[124]; snprintf(expected_value, sizeof(expected_value), "%"#format, expected);       \
                __FTST_TEST_ERROR(#cond, actual_value, expected_value); } else_funct)

# define __FTST_EQ_CHOOSER(_f1, _f2, _f3, ...) _f3 
# define __FTST_CHOOSE_EQ_MACRO(...) __FTST_EQ_CHOOSER(__VA_ARGS__, __FTST_EQ_FORMAT, __FTST_EQ_DEFAULT) // TODO proper work with 1 arg (no crutched comma)
# define __FTST_EQ(cond, expected, else_funct, ...) __FTST_CHOOSE_EQ_MACRO(__VA_ARGS__)(cond, expected, else_funct, __VA_ARGS__)


# define __FTST_FALSE(cond, else_funct) __FTST_SIMPLE_TEST(!cond,  \
                __FTST_TEST_ERROR(#cond, "true", "false"); else_funct)
# define __FTST_TRUE(cond, else_funct) __FTST_SIMPLE_TEST(cond,  \
                __FTST_TEST_ERROR(#cond, "false", "true"); else_funct)

# define __FTST_EXPECT_FUNCT 
# define FTST_EXPECT_EQ(cond, expected, ...) __FTST_EQ(cond, expected, __FTST_EXPECT_FUNCT, __VA_ARGS__)
# define FTST_EXPECT_TRUE(cond) __FTST_TRUE(cond, __FTST_EXPECT_FUNCT)
# define FTST_EXPECT_FALSE(cond) __FTST_FALSE(cond, __FTST_EXPECT_FUNCT)

# define __FTST_ASSERT_FUNCT return
# define FTST_ASSERT_EQ(cond, expected, ...) __FTST_EQ(cond, expected, __FTST_ASSERT_FUNCT, __VA_ARGS__)
# define FTST_ASSERT_TRUE(cond) __FTST_TRUE(cond, __FTST_ASSERT_FUNCT)
# define FTST_ASSERT_FALSE(cond) __FTST_FALSE(cond, __FTST_ASSERT_FUNCT)


# define __FTST_ERROR(error_message) __ftst_error(#error_message)

void    __ftst_error(char const* error_message)
{ fprintf(stderr, "ftst error | %s\n", error_message); }


void    ftst_init(FILE *stream_output, char const* result_file_name)
{
    __g_ftst_stream = stream_output;

    if (result_file_name != NULL)
    {
        char file_with_exp[512];
        snprintf(file_with_exp, sizeof(file_with_exp), "%s%s", result_file_name, ".csv");
        __g_ftst_table = fopen(file_with_exp, "w");
        if (__g_ftst_table == NULL) __FTST_ERROR(the_file_cant_be_created);
    }
}

void    ftst_exit(void)
{
    if (__g_ftst_table)
        fclose(__g_ftst_table);
    __g_ftst_table = NULL;
    __g_ftst_stream = NULL;
}


#define FTST_RUNTEST(test_name) __ftst_run_test(&__FTST_TEST_CASE(test_name), __FTST_TEST_CASE_NAME(test_name))

clock_t __ftst_start_timer() { return clock(); }
clock_t __ftst_end_timer(clock_t start) { return clock() - start; }

void    __ftst_run_test(__ftst_test_t test_case, char const* test_case_name)
{
    clock_t test_time;
    clock_t start;
    __ftst_test test = (__ftst_test){ 0, 0 };

    start = __ftst_start_timer();
    test_case(&test);
    test_time = __ftst_end_timer(start);
    fprintf(__g_ftst_stream, "%s,%d/%d,%.3fms\n", test_case_name, test.passed, test.launched, test_time / 1000.);
    if (__g_ftst_table)
        fprintf(__g_ftst_table, "%s,%d/%d,%.3fms\n", test_case_name, test.passed, test.launched, test_time / 1000.);
}


# ifdef NAMESPACE_FTST
#  define EXPECT_EQ     FTST_EXPECT_EQ
#  define EXPECT_TRUE   FTST_EXPECT_TRUE
#  define EXPECT_FALSE  FTST_EXPECT_FALSE

#  define ASSERT_EQ     FTST_ASSERT_EQ
#  define ASSERT_TRUE   FTST_ASSERT_TRUE
#  define ASSERT_FALSE  FTST_ASSERT_FALSE

#  define TEST          FTST_TEST
#  define RUNTEST       FTST_RUNTEST
# endif

#endif
