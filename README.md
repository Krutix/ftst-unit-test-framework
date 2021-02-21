# FTST unit test framework
FTST is a small header-only unit test framework for C, developed for using in school21(42) projects

FTST это маленький header-only unit test framework для C, разработанный для использования в проектах школы 21(42)

## Tips
- [x] Compatible with C89
- [x] Compare with type specification
- [x] Allocation test (malloc fail, malloc size, leaks)
- [x] Unreadeble source on macro magic
- [ ] Compare with specific accuracy
- [ ] Launch block of tests
- [ ] Error custom comments

**More about FTST functionality on [wiki](https://github.com/Krutix/ft_speed_test/wiki)**

## Getting started
**More about getting started with FTST on [wiki](https://github.com/Krutix/ft_speed_test/wiki/Start-with-FTST)**

```c
#define FTST_MAIN_FILE
#include "ftst.h"
TEST(my_test)
{
    EQ(1, 1);
}

int main()
{
    FTST_INIT(stdout, NULL);

    RUN_TEST(my_test);

    return FTST_EXIT();
}
```
