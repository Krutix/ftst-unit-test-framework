#include <stdint.h>
#include <fcntl.h>
#include "ftst.h"

#define TEST_PRINTF(F, ...) printf(F, __VA_ARGS__)

#define NAME(name) #name
#define RESULT_FILE NAME(std) "_result"

FTST_TEST(simple_integer)
{
    for (int i = 0; i < 50000; i++)
        TEST_PRINTF("%i", i);
}

FTST_TEST(complex_format_int)
{
    for (long long i = INT64_MIN; i < INT64_MIN + 20000l; i++)
        TEST_PRINTF("%050lli", i);
    for (long long i = INT64_MAX; i > INT64_MAX - 20000l; i--)
        TEST_PRINTF("%+20lli", i);
}

int main()
{
    ftst_init(stderr, RESULT_FILE);

    FTST_RUNTEST(simple_integer);
    FTST_RUNTEST(complex_format_int);

    ftst_exit();
}
