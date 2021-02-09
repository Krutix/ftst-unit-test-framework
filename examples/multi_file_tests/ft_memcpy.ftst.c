
#include "_ft_string.h"

#include "ftst.h"

TEST(ft_memcpy_basic_test)
{
    char buff[256];
    STR_EQ(ft_memcpy(buff, "aaaaaaa", 8), "aaaaaaa");
    STR_EQ(buff, "aaaaaaa");
    STR_EQ(ft_memcpy(buff, "bbbb", 4), "bbbbaaa");
    STR_EQ(buff, "bbbbaaa");
}
