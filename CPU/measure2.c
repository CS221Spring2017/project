#include <stdio.h>
#include <stdlib.h>

static inline unsigned long long rdtsc(void) {
    unsigned lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((unsigned long long)hi << 32) | (unsigned long long)lo);
}

int main(int argc, char* argv[]){
	int nloops = atoi(argv[1]);

	unsigned long long begin;
	unsigned long long end;

	begin = rdtsc();
	
	for(int i = 0; i < nloops; i++){}

	end = rdtsc();
	unsigned long long elapse = end - begin;
	
	printf("%f\n",  elapse *1.0 / nloops);
}