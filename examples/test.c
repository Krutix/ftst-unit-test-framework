#define FTST_MAIN_FILE
#include "ftst.h"

TEST(equal_test)
{
    EQ(1llu, 1);
    EQ(43, 43);
    EQ(43, 43, i, ASSERT);
    EQ(0., 0., g);
    LESS(15, 30);
    MORE(30, 15);
}

TEST(boolean_test)
{
    IS_TRUE(true);
    IS_FALSE(false);
    IS_TRUE(15 == 0);
    IS_FALSE(15 == 0);
    IS_FALSE(-1u, u, ASSERT);
    IS_TRUE(true);
}

TEST(string_cmp)
{
    STR_EQ("hello world", "hello world");
    STR_NE("hello worlb", "hello world");
}

TEST(cmp_with_accuracy)
{
    EQ_A(0.1 + 0.2, 0.3, 0.0, f, EXPECT, "real 0.1 + 0.2 = %.20f", ARGS(0.1 + 0.2));
    EQ_A(0.1 + 0.2, 0.3, 0.000005, f);
}

TEST(error_test)
{
    EQ(-1, 1);
    EQ(-1llu, -1llu, llu);
    LESS(-1llu, 1llu, llu);
    LESSEQ(-1llu, 1llu, llu, EXPECT, "castom comment %i:%i", ARGS(1, 2));
    EQ(-1llu, 1llu, llu, ASSERT, "some error happend");
    EQ(-1, 1);
}

int main()
{
    FTST_INIT();

    RUNTEST(equal_test);
    RUNTEST(boolean_test);
    RUNTEST(string_cmp);
    RUNTEST(cmp_with_accuracy);
    RUNTEST(error_test);

    return FTST_EXIT();
}
