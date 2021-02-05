#define FTST_ALLOC_TEST true
#define FTST_MAIN_FILE
#include "ftst.h"

#include <stdlib.h>

int foo()
{
    void *p = malloc(124);
    if (p == NULL)
        return (-1);
    free(p);
    return (1);
}

TEST(malloc_test)
{
    LEAK_RESET();
    int foo_ret = 0;

    ALLOC_COUNTER_SET(1);
    for (size_t i = 0; i < 3; i++)
    {
        foo_ret = foo();

        ALLOC_IF_ERROR_ELSE(
            EQ(foo_ret, -1);
            break;
            ,
            EQ(foo_ret, 1));
    }
    ALLOC_CLEAN();

    void* ptr = malloc(16);
    EQ(MALLOC_SIZE(ptr), 16, zu);
    IS_TRUE(LEAKS());

    free(ptr);

    IS_FALSE(LEAKS());
}

int main()
{
    FTST_INIT();

    RUNTEST(malloc_test);

    return FTST_EXIT();
}