#ifndef FT_SPEED_TEST_H
# define FT_SPEED_TEST_H

# include <time.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>

typedef struct {
    FILE*       fptr;
    char        owned;
} ftst_fptr;

typedef struct {
    size_t passed;
    size_t launched;
} ftst_test;

ftst_fptr   g_ftst_fptr;
typedef     void(*ftst_test_t)(ftst_test*);

# define FTST_TEST_CASE(test_name) ftst_test_case_##test_name
# define FTST_TEST_CASE_NAME(test_name) #test_name

# define FTST_TEST(test_name)                    \
void FTST_TEST_CASE(test_name)(ftst_test* test)

# define __FTST_SIMPLE_TEST(cond, else_funct)      \
{                                                       \
    test->launched++;                                   \
    if (cond) {                                         \
        test->passed++;                                 \
    } else {                                            \
        else_funct;                                     \
    }                                                   \
}

# define __FTST_EQ(cond, expected, else_funct) __FTST_SIMPLE_TEST((cond) == (expected), else_funct)
# define __FTST_FALSE(cond, else_funct) __FTST_SIMPLE_TEST(!cond, else_funct) 
# define __FTST_TRUE(cond, else_funct) __FTST_SIMPLE_TEST(cond, else_funct)

# define __FTST_EXPECT_FUNCT 
# define FTST_EXPECT_EQ(cond, expected) __FTST_EQ(cond, expected, __FTST_EXPECT_FUNCT)
# define FTST_EXPECT_TRUE(cond) __FTST_TRUE(cond, __FTST_EXPECT_FUNCT)
# define FTST_EXPECT_FALSE(cond) __FTST_FALSE(cond, __FTST_EXPECT_FUNCT)

# define __FTST_ASSERT_FUNCT return
# define FTST_ASSERT_EQ(cond, expected) __FTST_EQ(cond, expected, __FTST_ASSERT_FUNCT)
# define FTST_ASSERT_TRUE(cond) __FTST_TRUE(cond, __FTST_ASSERT_FUNCT)
# define FTST_ASSERT_FALSE(cond) __FTST_FALSE(cond, __FTST_ASSERT_FUNCT)


# define FTST_ERROR(error_message) ftst_error(#error_message)

void ftst_error(char const* error_message)
{
    fprintf(stderr, "ftst error | %s\n", error_message);
    exit(-1);
}


clock_t ftst_start_timer() { return clock(); }
clock_t ftst_end_timer(clock_t start) { return clock() - start; }


void ftst_init(FILE *file)
{
    g_ftst_fptr.fptr = file;
    g_ftst_fptr.owned = false;
}

void ftst_init_c(char const* file_name)
{
    if (file_name != NULL)
    {
        char file_with_exp[512];
        snprintf(file_with_exp, sizeof(file_with_exp), "%s%s", file_name, ".csv");
        g_ftst_fptr.fptr = fopen(file_with_exp, "w");
        g_ftst_fptr.owned = true;
        if (g_ftst_fptr.fptr == NULL) FTST_ERROR(file_cant_be_create);
    }
}

void ftst_exit(void)
{
    if (g_ftst_fptr.owned)
        fclose(g_ftst_fptr.fptr);
    g_ftst_fptr.owned = false;
    g_ftst_fptr.fptr = NULL;
}


#define FTST_RUNTEST(test_name) ftst_run_test(&FTST_TEST_CASE(test_name), FTST_TEST_CASE_NAME(test_name))

void ftst_run_test(ftst_test_t test_case, char const* test_case_name)
{
    clock_t test_time;
    clock_t start;
    ftst_test test = (ftst_test){ 0, 0 };

    start = ftst_start_timer();
    test_case(&test);
    test_time = ftst_end_timer(start);
    fprintf(g_ftst_fptr.fptr, "%s,%d/%d,%.3fms\n", test_case_name, test.passed, test.launched, test_time / 1000.);
}

#endif
