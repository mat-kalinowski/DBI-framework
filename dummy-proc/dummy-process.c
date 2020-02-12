#include <unistd.h>
#include <time.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <signal.h>

int main(int args, char *argv[]){
    int i = 0;
    printf("Hello my PID is : %d\n", getpid());
    printf("dlopen: %p\n", (void*) dlopen);
    printf("mprotect: %p\n", (void*) mprotect);
    fflush(stdout);

    while(true){ 
      printf("Iteration: %d\n",i);
      fflush(stdout);
      i++;
      sleep(1); 
    }
    
}
