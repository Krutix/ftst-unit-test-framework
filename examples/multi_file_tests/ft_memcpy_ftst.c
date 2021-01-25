
#include "ft_string.h"

#define FTST_SUB_TEST
#include "ftst.h"

TEST(ft_memcpy_basic_test)
{
    char buff[256];
    STR_EQ(ft_memcpy(buff, "aaaaaaa", 8), "aaaaaaa");
    STR_EQ(ft_memcpy(buff, "aaaa", 4), "aaaaaaa");
}
