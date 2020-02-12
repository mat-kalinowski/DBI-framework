#include <unistd.h>

__attribute__((constructor)) void init_func(void){
    char buff[] = "hello world, from shared library \n";
    write(1, buff, sizeof(buff));
}