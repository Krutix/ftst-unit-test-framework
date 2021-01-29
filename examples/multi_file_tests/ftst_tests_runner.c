#include "ftst.h"

TEST(ft_strlen_basic_test);
TEST(ft_memcpy_basic_test);

int main()
{
    FTST_INIT();

    RUNTEST(ft_strlen_basic_test);
    RUNTEST(ft_memcpy_basic_test);

    return FTST_EXIT();
}