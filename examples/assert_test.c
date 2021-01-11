
#include "ftst.h"

TEST(equal_test)
{
    FTST_EQ(-1llu, 1llu, FTST_ASSERT, "llu");
}

int main()
{
    ftst_init(stdout, NULL);

    RUNTEST(equal_test);

    ftst_exit();
}
