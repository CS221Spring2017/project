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
    
    int nloops = atoi(argv[1]);
    unsigned long long total = 0;
    //unsigned long long start, end, difference;
    
    for (int i = 0; i < nloops; i++) {
        
        int pipecs[2];
        pid_t cpid;
        char buf;
        
        pipe(pipecs); // create the pipe for contexgt switch
    
        cpid = fork(); // duplicate the current process
        if (cpid == -1) {
            printf("fork failed!\n");
            exit(-1);
        }
        if (cpid == 0) { //Child
            unsigned long long end;
            end = rdtsc();
            write(pipecs[1], &end, sizeof(end));
            return 0;
        } else { //Partent
            unsigned long long begin, end, difference;
            begin = rdtsc();
            read(pipecs[0], &end, sizeof(end));
            difference = (end - begin);
            total += difference;
            wait(NULL);
        }
    }

    printf("%llu cycles\n", total/nloops);

    return 0;
}
