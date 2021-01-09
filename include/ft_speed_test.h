#ifndef FT_SPEED_TEST_H
# define FT_SPEED_TEST_H

# include <time.h>
# include <stdlib.h>
# include <stdio.h>

FILE*       g_ftst_fptr;
clock_t     g_ftst_start;
typedef     void(*ftst_test_t)(void);

# define FTST_TEST_CASE(test_name) &ftst_test_case_##test_name
# define FTST_TEST_CASE_NAME(test_name) #test_name

# define FTST_TEST(test_name)                    \
void ftst_test_case_##test_name(void)

# define FTST_ERROR(error_message) ftst_error(#error_message)
void ftst_error(char const* error_message)
{
    fprintf(stderr, "ftst error | %s\n", error_message);
    exit(-1);
}

void ftst_start_timer() { g_ftst_start = clock(); }
clock_t ftst_end_timer() { return clock() - g_ftst_start; }

void ftst_init(char const* file_name)
{
    if (file_name != NULL)
    {
        char file_with_exp[512];
        snprintf(file_with_exp, sizeof(file_with_exp), "%s%s", file_name, ".csv");
        g_ftst_fptr = fopen(file_with_exp, "w");
        if (g_ftst_fptr == NULL)
            FTST_ERROR(file_cant_be_open);
    } else {
        g_ftst_fptr = stderr;
    }
}

void ftst_exit(void)
{
    fclose(g_ftst_fptr);
    g_ftst_fptr = NULL;
}

#define FTST_RUNTEST(test_name) ftst_run_test(FTST_TEST_CASE(test_name), FTST_TEST_CASE_NAME(test_name))
void ftst_run_test(ftst_test_t test_case, char const* test_case_name)
{
    clock_t test_time;

    ftst_start_timer();
    test_case();
    test_time = ftst_end_timer();
    fprintf(g_ftst_fptr, "%s,%fms\n", test_case_name, test_time / 1000.);
}

#endif
