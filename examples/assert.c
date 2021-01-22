
#define FTST_ASSERT_LEVEL 2
#define FTST_NAMESPACE 1
#include "ftst.h"

int main()
{
    STATIC_ASSERT(4 == sizeof(int)); /* true */
    /* STATIC_ASSERT(1 == sizeof(float)); /* false */
    ASSERT(1 == 2, "error"); /* Runtime error */
}