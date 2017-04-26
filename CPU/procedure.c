#include "stdlib.h"
#include "stdio.h"

#pragma OPTIMIZE OFF

static inline unsigned long long rdtsc(void) {
    unsigned lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((unsigned long long)hi << 32) | (unsigned long long)lo);
}

static inline void foo0(){}
static inline void foo1(int p1){}
static inline void foo2(int p1, int p2){}
static inline void foo3(int p1, int p2, int p3){}
static inline void foo4(int p1, int p2, int p3, int p4){}
static inline void foo5(int p1, int p2, int p3, int p4, int p5){}
static inline void foo6(int p1, int p2, int p3, int p4, int p5, int p6){}
static inline void foo7(int p1, int p2, int p3, int p4, int p5, int p6, int p7){}

int main(int argc, char* argv[]){
	int nloops = atoi(argv[1]);
	unsigned long long cycles = 0;
	unsigned long long begin;
	unsigned long long end;
	//0 argument
	cycles = 0;
	for(int i = 0; i < nloops; i++){
		begin = rdtsc();
		foo0();
		end = rdtsc();
		cycles += end - begin;
	}	
	printf("procedure call over head [0 argument] : %f cycles \n", cycles * 1.0 / nloops);

	//1 argument
	cycles = 0;
	
	for(int i = 0; i < nloops; i++){
		begin = rdtsc();
		foo1(1);
		end = rdtsc();
		cycles += end - begin;
	}
	printf("procedure call over head [1 argument] : %f cycles \n", cycles * 1.0 / nloops);

	//2 argument
	cycles = 0;	
	for(int i = 0; i < nloops; i++){
		begin = rdtsc();
		foo2(1, 2);
		end = rdtsc();
		cycles += end - begin;
	}
	printf("procedure call over head [2 argument] : %f cycles \n", cycles * 1.0 / nloops);

	//3 argument
	cycles = 0;	
	for(int i = 0; i < nloops; i++){
		begin = rdtsc();
		foo3(1, 2, 3);
		end = rdtsc();
		cycles += end - begin;
	}
	printf("procedure call over head [3 argument] : %f cycles \n", cycles * 1.0 / nloops);

	//4 argument
	cycles = 0;	
	for(int i = 0; i < nloops; i++){
		begin = rdtsc();
		foo4(1, 2, 3, 4);
		end = rdtsc();
		cycles += end - begin;
	}
	printf("procedure call over head [4 argument] : %f cycles \n", cycles * 1.0 / nloops);

	//5 argument
	cycles = 0;	
	for(int i = 0; i < nloops; i++){
		begin = rdtsc();
		foo5(1, 2, 3, 4, 5);
		end = rdtsc();
		cycles += end - begin;
	}
	printf("procedure call over head [5 argument] : %f cycles \n", cycles * 1.0 / nloops);

	//6 argument
	cycles = 0;	
	for(int i = 0; i < nloops; i++){
		begin = rdtsc();
		foo6(1, 2, 3, 4, 5, 6);
		end = rdtsc();
		cycles += end - begin;
	}
	printf("procedure call over head [6 argument] : %f cycles \n", cycles * 1.0 / nloops);

	//7 argument
	cycles = 0;	
	for(int i = 0; i < nloops; i++){
		begin = rdtsc();
		foo7(1, 2, 3, 4, 5, 6, 7);
		end = rdtsc();
		cycles += end - begin;
	}
	printf("procedure call over head [7 argument] : %f cycles \n", cycles * 1.0 / nloops);
	return 0;
}