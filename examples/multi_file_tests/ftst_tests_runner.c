#define FTST_MAIN_FILE
#include "ftst.h"

TEST(ft_strlen);
TEST(ft_memcpy_basic_test);

int main()
{
    FTST_INIT();

    RUNTEST(ft_strlen);
    RUNTEST(ft_memcpy_basic_test);

    return FTST_EXIT();
}