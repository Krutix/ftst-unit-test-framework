#include <stdlib.h>
#include <malloc/malloc.h>
#include <stdio.h>
#include <dlfcn.h>

void*	malloc(size_t size)
{
    void *(*libc_malloc)(size_t) = (void *(*)(size_t))dlsym(RTLD_NEXT, "malloc");
    void * p = libc_malloc(size);
	printf("lol\n");
	return p;
}

int main()
{
	void* k = malloc(4);
	printf("%p", k);
}