#define FTST_MALLOC_TEST true
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

    MALLOC_COUNTER_SET(1);
    for (size_t i = 0; i < 3; i++)
    {
        foo_ret = foo();

        if (FTST_IS_MALLOC_ERROR) {
            EQ(foo_ret, -1);
            break;
        }
        else
            EQ(foo_ret, 1);
    }
    MALLOC_CLEAN();

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