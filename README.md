# FTST unit test framework
FTST это маленький header-only unit test framework для C, разрабатываемый в качестве учебного проекта

## Tips
- [x] Compatible with C89
- [x] Compare with type specification
- [x] Allocation test (malloc fail, malloc size, leaks)
- [x] Compare with specific accuracy
- [x] Castom error desctiption
- [x] Launch block of tests
- [x] Unreadeble source on macro magic
- [ ] Output to standart stream handle
- [ ] Advanced testrunner generator
- [ ] Segfault and other fatal probles handle

**More about FTST functionality on [wiki](https://github.com/Krutix/ftst-unit-test-framework/wiki)**

## Getting started
Примеры кода с тестами можно посмотреть в папке *examples/*, а более подробное описание на [wiki](https://github.com/Krutix/ftst-unit-test-framework/wiki)

**More about getting started with FTST on [wiki](https://github.com/Krutix/ftst-unit-test-framework/wiki/Start-with-FTST)**

```c
#define FTST_MAIN_FILE
#include "ftst.h"
TEST(my_test)
{
    EQ(1, 1);
}

int main()
{
    FTST_INIT();

    RUNTEST(my_test);

    return FTST_EXIT();
}
```
