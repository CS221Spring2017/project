#include "stdlib.h"
#include "stdio.h"
#include <sys/types.h>
#include <unistd.h>

static inline unsigned long long rdtsc(void) {
    unsigned lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((unsigned long long int)hi << 32) | lo);
}

int main(int argc, char **argv) {   
 
    unsigned long long time_switch = 133221;
    
    int nloops = atoi(argv[1]);
    unsigned long long total = 0;
    unsigned long long begin, end;
    
    for (int i = 0; i < nloops; i++) {
        
        begin = rdtsc();
        pid_t pid = fork();
        
        if (pid == -1) {
            printf("fork failed!\n");
            exit(-1);
        }
        
        if (pid == 0) {//child process
            exit(-1);
        } else {//parent process
            wait();
            end = rdtsc();
            unsigned long long diff = end - begin;
            total += (diff - 2*time_switch);
        }
    }

    printf("%llu cycles\n", total/nloops);

    return 0;
}
