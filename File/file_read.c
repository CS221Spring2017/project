#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

static inline unsigned long long rdtsc(void) {
    unsigned lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((unsigned long long)hi << 32) | (unsigned long long)lo);
}

off_t BLOCK_SIZE = 4 * 1024;
off_t FILE_SIZE;

double random_access(char* filename);
double sequence_access(char* filename);

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		printf("usage: ./cache 4 M cache4M\n");
		exit(0);
	}
	//This is a signed integer type used to represent file sizes. 
	FILE_SIZE = atoi(argv[1]);
	const char type = *argv[2];
	if(type == 'G')	
		FILE_SIZE = FILE_SIZE * 1024 * 1024 * 1024;
	if(type == 'M')
		FILE_SIZE = FILE_SIZE * 1024 * 1024;
	int number=atoi(argv[3]);
	char *addr="/Volumes/share/cache";

	if (number==1){
		double result_random  = random_access(addr);
		printf("random access cycle = %.2f\n", result_random);

	}
	else{
		double result_sequence = sequence_access(addr);
		printf("sequence access cycle = %.2f\n",result_sequence);

	}

	//double result_random  = random_access(argv[3]);
	//double result_sequence = sequence_access(argv[3]);
	//printf("random access cycle = %.2f, sequence access cycle = %.2f\n", result_random, result_sequence);
	return 0;
}

double sequence_access(char* filename)
{
	int file = open(filename, O_RDONLY);
	if(fcntl(file, F_NOCACHE, 1) == -1) {
        printf("Failed to disable cache.\n");
    }
    
	void *buffer = malloc(BLOCK_SIZE);

	unsigned long long begin;
	unsigned long long end;
	unsigned long long total;

	off_t N = FILE_SIZE / BLOCK_SIZE;
	for(int i = 0; i < N; i++)
	{
		begin = rdtsc();
		read(file, buffer, BLOCK_SIZE);
		end = rdtsc();
		total += end - begin;
	}
	free(buffer);
	close(file);

	return (double)total * 1.0 / N;
}

double random_access(char* filename)
{
	int file = open(filename, O_RDONLY);
	if(fcntl(file, F_NOCACHE, 1) == -1) {
        printf("Failed to disable cache.\n");
    }
	void *buffer = malloc(BLOCK_SIZE);

	unsigned long long begin;
	unsigned long long end;
	unsigned long long total;

	off_t N = FILE_SIZE / BLOCK_SIZE;
	for(int i = 0; i < N; i++)
	{
		off_t offset = rand() % N;
		begin = rdtsc();
		lseek(file, offset* BLOCK_SIZE, SEEK_SET);
		read(file, buffer, BLOCK_SIZE);
		end = rdtsc();
		total += end - begin;
	}
	free(buffer);
	close(file);

	return (double)total * 1.0 / N;
}