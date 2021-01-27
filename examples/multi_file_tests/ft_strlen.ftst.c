
#include "ft_string.h"

#define FTST_SUB_TEST
#include "ftst.h"

TEST(ft_strlen_basic_test)
{
    char buff[126] = "";
    for (size_t i = 0; i < 124; i++)
    {
        EQ(ft_strlen(buff), strlen(buff), zu, ASSERT);
        strcpy(buff, "-");
    }
}
