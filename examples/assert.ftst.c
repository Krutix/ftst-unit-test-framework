
#define FTST_ASSERT_LEVEL 2
#define FTST_NAMESPACE 1
#include "ftst.h"

int main()
{
    STATIC_ASSERT(4 == sizeof(int), int_must_be_4_bytes); /* true */
    /* STATIC_ASSERT(1 == sizeof(float), float_must_be_1_byte); /* false */
    RUNTIME_ASSERT(1 == 2, "error"); /* Runtime error with breakpoint */
    RUNTIME_ASSERT(1 == 1, "error 2"); /* nothing happend */
}
