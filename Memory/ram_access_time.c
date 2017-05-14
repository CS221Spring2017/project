#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <mach/mach_time.h>

static inline unsigned long long rdtsc(void) {
    unsigned lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((unsigned long long)hi << 32) | (unsigned long long)lo);
}
//Size of memory region accessing ranging from 2^2KB to 2^22 kb = 4 GB
#define min_array_size 2
#define max_array_size 19
#define NANOS_CONVERT 1000000000.0
#define COUNT 10000
//Size of memory region that we will accessed, ranging from 4KB to 512MB
//int sizeList[18] = {4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288};
//The step length of KB
int strideList[7] = {4, 64, 128, 1024, 1048576, 4194304, 16777216};

void stride_array(int loops, FILE *output, int stride);


int main(int argc, const char * argv[])
{
    if(argc < 2) {
        printf ("please specify the loop time.\n");
        exit(0);
    }

    unsigned long long loops = atoi(argv[1]);
    

	FILE *file;

	file = fopen("ram_access_time", "w");

	stride_array(loops, file, 32);

    fclose(file);
    return 0;
}
 
/*
double cacheAccessTime(int size, int strideLength)
{
    int* A;
    A = (int *)malloc(size * 1024 / 4 * sizeof(int));
    int num, length;
    length = strideLength / 4;   
    num = size * 1024 / 4;
    int index;
    A[0] = 0;
    for (int i = 0; i < num; i++) {
        index = i + length;
        if (index >= num) {
            index %= num;
        }
        A[i] = index;
    }

    int x = 0;
    int64_t begin;
    int64_t end;
    int64_t total_time = 0;
    int count = COUNT;

    begin = rdtsc();
    for (int i = 0; i < count; i++) {
        x = A[x];
    }
    end = rdtsc();
    total_time = end - begin;
    double ans = (total_time - 106) / count - 6;
    free(A);
    return ans;
}

void fixedStride(int strideIndex, FILE *file, int ts)
{
	for (int i = 0; i < ts; i++) {
		for (int j = 0; j < 18; j++) {
			double averageTime = cacheAccessTime(sizeList[j], strideList[strideIndex]);
			fprintf(file, "%lf ", averageTime);
		}
		fprintf(file, "\n");
	}
	return ;
}
*/

void stride_array(int loops, FILE *output, int stride)
{
    for (int i=0;i< loops;i++){
        for (int size= min_array_size; size< max_array_size;size++){

            int64_t array_size = (1 << size) * 1024;

            int * Array;

            if (!(Array = (int *)malloc(array_size * sizeof(int))))
            {
                fprintf(stderr, "Malloc failure\n");
                exit(1);
            }


            Array[0] = 0;

            int64_t index;

            for (int64_t i = 0; i < array_size; i++) {
                index = i + stride;
                if (index >= array_size) {
                    index %= array_size;
                }
                Array[i] = index;
            }

            int64_t x = 0;
            int64_t begin;
            int64_t end;
            int64_t total_time = 0;

            begin = rdtsc();
            for (int i = 0; i < COUNT; i++) {
                x = Array[x];
                //printf("%lli\n", x);
            }
            end = rdtsc();

            total_time = end - begin;
            double time = (double)(total_time - 106) / COUNT - 6;
            free(Array);

            fprintf(output, "%lf\n", time);
            //printf("%lf\n", time);
        }
        fprintf(output, "\n");
    }

}

/*
void random(unsigned long long loops, int min,int max)
{   
    unsigned long long begin, end;
    srand(time(NULL));
    
    //Size of memory region ranging from 2^2KB to 512MB
    for(int size = min;size < max;size++) {
        
        unsigned long long arraysize = 1 << size;
        
        unsigned long long begin, end, diff;
        unsigned long long total = 0;
        char *array = (char*)malloc( sizeof(char) * arraysize);
        memset(array,'x',arraysize);
        
        begin = rdtsc();
        for(int i = 0;i < loops;i++) {
            
            int index = rand() % arraysize;
            
            char ch = array[index];
        }
        end = rdtsc();
        
        total += (end - begin);
        
        free(array);
        
        printf("Ramdom access array size : %d, RAM access cycle : %f\n", size, (total*1.0)/loops);
}
*/