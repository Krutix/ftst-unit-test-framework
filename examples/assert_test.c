
#define NAMESPACE_FTST
#include "ft_speed_test.h"

TEST(equal_test)
{
    EXPECT_EQ(1, 1);
    ASSERT_EQ(2, 2);
    EXPECT_EQ(1, 1);
}

TEST(bool_test)
{
    EXPECT_TRUE(342);
    EXPECT_FALSE(0);
    EXPECT_FALSE(423);
    ASSERT_TRUE(0);
}

TEST(error_test)
{
    EXPECT_EQ(15, 1);
    ASSERT_EQ(2, 42);
    EXPECT_EQ(1, 3);
}

int main()
{
    ftst_init(stderr);

    RUNTEST(equal_test);
    RUNTEST(error_test);
    RUNTEST(bool_test);

    ftst_exit();
}
