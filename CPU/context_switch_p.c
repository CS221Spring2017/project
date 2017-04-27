#include "stdlib.h"
#include "stdio.h"
#include <sys/types.h>
#include <unistd.h>

static inline unsigned long long rdtsc(void) {
    unsigned lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((unsigned long long int)hi << 32) | (unsigned long long)lo);
}



int main(int argc, char **argv) {   
    
    int nloops = atoi(argv[1]);
    unsigned long long total = 0;
    //unsigned long long start, end, difference;
    
    int counter=0;

    for (int i = 0; i < nloops; i++) {
        
        int pipecs[2];
        pid_t cpid;
        char buf;
        
        pipe(pipecs); // create the pipe for context switch
    
        cpid = fork(); // duplicate the current process
        if (cpid == -1) {
            printf("fork failed!\n");
            exit(-1);
        }
        else if (cpid == 0) { //Child
            unsigned long long end;
            end = rdtsc();

            write(pipecs[1], &end, sizeof(end));
            return 0;
        } else { //Partent
            unsigned long long begin, end, difference;
            begin = rdtsc();

            wait(NULL);
            read(pipecs[0], &end, sizeof(end));
            if (end>begin)
            {
                difference = (end - begin);
                total += difference;
                counter++;
            }
        }
    }

    printf("%llu cycles\n", total/counter);

    return 0;
}
