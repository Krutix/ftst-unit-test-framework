#include <ftst.h>

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
    int foo_ret = 0;
    ALLOC_COUNTER_SET(1);

    foo_ret = foo();
    foo_ret = foo();
    ALLOC_IF_ERROR(EQ(foo_ret, -1));
}

int main()
{
    FTST_INIT();

    RUNTEST(malloc_test);

    FTST_EXIT();
}