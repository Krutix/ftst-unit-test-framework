
#include "_ft_string.h"

#include "ftst.h"

static TEST(ft_strlen_basic_test)
{
    char buff[126] = "";
    for (size_t i = 0; i < 124; i++)
    {
        EQ(ft_strlen(buff), strlen(buff), zu, ASSERT);
        strcpy(buff, "-");
    }
}

static TEST(ft_strlen_bad)
{
    char *str = "---------------------\0-";
    EQ(ft_strlen(str), strlen(str), zu);
}

TEST(ft_strlen)
{
    RUNTEST(ft_strlen_basic_test);
    RUNTEST(ft_strlen_bad);
}