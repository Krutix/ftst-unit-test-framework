#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>

#define FTST_ALLOC_TEST false
#define FTST_MAIN_FILE
#include "ftst.h"

#define TEST_PRINTF(F, ...) printf(F, __VA_ARGS__), fflush(stdout)
/*
**  fflush - force printf write data on screen, because standard printf cheat and dont write data on screen with every call (>2x faster without fflush)
**  Dont use it with your own ft_printf
*/

#define NAME(name) #name
#define RESULT_FILE NAME(std) "_result"

FTST_TEST(simple_integer)
{
    for (int i = 0; i < 10000; i++)
        TEST_PRINTF("%i", i);
    for (int i = 2100000000; i < INT32_MAX; i++)
        TEST_PRINTF("%i", i);
    for (int i = -2100000000; i > INT32_MIN; i--)
        TEST_PRINTF("%i", i);
}

FTST_TEST(complex_format_int)
{
    for (long long i = INT64_MIN; i < INT64_MIN + 20000ll; i++)
        TEST_PRINTF("%050lli", i);
    for (long long i = INT64_MAX; i > INT64_MAX - 20000ll; i--)
        TEST_PRINTF("%+20lli", i);
}

int main()
{
    FTST_INIT(stderr, RESULT_FILE);

    FTST_RUNTEST(simple_integer);
    FTST_RUNTEST(complex_format_int);

    return FTST_EXIT();
}
