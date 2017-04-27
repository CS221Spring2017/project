#include "stdlib.h"
#include "stdio.h"
#include <sys/types.h>
#include <unistd.h>

static inline unsigned long long rdtsc(void) {
    unsigned lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((unsigned long long )hi << 32) | (unsigned long long)lo);
}

int pipecs[2];
int counter=0;

void *func(void *tid) {
    unsigned long long end;
    end = rdtsc();

    write(pipecs[1], &end, sizeof(end));
    pthread_exit(NULL);
}

int main(int argc, char **argv) {   
    
    int nloops = atoi(argv[1]);
    unsigned long long total = 0;
    //unsigned long long start, end, difference;

    pipe(pipecs);
    
    for (int i = 0; i < nloops; i++) {
        long n;
        pthread_t thread;
        pthread_create(&thread, NULL, func, (void *)n);
        
        unsigned long long begin, end, difference;
        begin = rdtsc();
        
        pthread_join(thread, NULL);
        read(pipecs[0], &end, sizeof(end));

        if (end>begin)
            {
                difference = (end - begin);
                total += difference;
                counter++;
            }

    }

    printf("%llu cycles\n", total/counter);

    return 0;
}


