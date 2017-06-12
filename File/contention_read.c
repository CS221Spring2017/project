#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

static inline unsigned long long rdtsc(void) {
    unsigned lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((unsigned long long)hi << 32) | (unsigned long long)lo);
}

off_t BLOCK_SIZE = 4 * 1024;
off_t FILE_SIZE  = 64 * 1024 * 1024;

void sequence_access_loop(char* filename, int n)
{

	int file = open(filename, O_RDONLY);	
	
	if(fcntl(file, F_NOCACHE, 1) == -1)
    	printf("Disable Cache Failed.\n");
	
    void *buffer = malloc(BLOCK_SIZE);
    int N = FILE_SIZE / BLOCK_SIZE;

    unsigned long long begin, end;
    unsigned long long total = 0;
    for(int j = 0; j < 100; j++)
    {
    	for(int i = 0; i < N; i++)
	    {
			read(file, buffer, BLOCK_SIZE);
	    }
    }
    close(file);
    free(buffer);
    printf("quit\n");
}
void sequence_access(char* filename, int n)
{

	int file = open(filename, O_RDONLY);	
	
	if(fcntl(file, F_NOCACHE, 1) == -1)
    	printf("Disable Cache Failed.\n");
	
    void *buffer = malloc(BLOCK_SIZE);
    int N = FILE_SIZE / BLOCK_SIZE;

    unsigned long long begin, end;
    unsigned long long total = 0;
    for(int j = 0; j < N; j++)
    {
    	begin = rdtsc();
		read(file, buffer, BLOCK_SIZE);
		end = rdtsc();
		total += end - begin;
    }
    close(file);
    free(buffer);
    printf("%d sequential= %f us\n", n, total*1.0*0.34/1e3/N);
}

void random_access_loop(char* filename, int n)
{
	int file = open(filename, O_RDONLY);
	if(fcntl(file, F_NOCACHE, 1) == -1) {
        printf("Failed to disable cache.\n");
    }
    
	void *buffer = malloc(BLOCK_SIZE);
	off_t N = FILE_SIZE / BLOCK_SIZE;
	for(int j = 0; j < 5; j++)
	{
		for(int i = 0; i < N; i++)
		{
			off_t offset = rand() % N;
			lseek(file, offset* BLOCK_SIZE, SEEK_SET);
			read(file, buffer, BLOCK_SIZE);
		}
	}
	free(buffer);
	close(file);
	printf("quit\n");
}

void random_access(char* filename, int n)
{
	int file = open(filename, O_RDONLY);
	if(fcntl(file, F_NOCACHE, 1) == -1) {
        printf("Failed to disable cache.\n");
    }
	void *buffer = malloc(BLOCK_SIZE);

	unsigned long long begin;
	unsigned long long end;
	unsigned long long total = 0;

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

	printf("%d random=%f us\n", n, total*1.0*0.34/1e3/N);
}


int main(int argc, const char* argv[])
{
	if(argc < 3){
		printf("usage ./program argument 1. argument ranges from [0, 14], 1=random access, 0=sequential access\n");
	}
	int n = atoi(argv[1]);
	int rand = atoi(argv[2]);
	char* filename[16];
	filename[0] = "data/file0";
	filename[1] = "data/file1";
	filename[2] = "data/file2";
	filename[3] = "data/file3";
	filename[4] = "data/file4";
	filename[5] = "data/file5";
	filename[6] = "data/file6";
	filename[7] = "data/file7";
	filename[8] = "data/file8";
	filename[9] = "data/file9";
	filename[10] = "data/file10";
	filename[11] = "data/file11";
	filename[12] = "data/file12";
	filename[13] = "data/file13";
	filename[14] = "data/file14";
	filename[15] = "data/file15";

	pid_t child[16];
	int i = 0;
	double time[16] = {0};
	for(; i < n; i++)
	{
		child[i] = fork();
		if(child[i]< 0)
		{
			printf("fork failed!\n");
			return 0;
		}
		else if(child[i] == 0)
		{
			if(rand == 1){
				if(i == n-1)
					random_access(filename[i], n);
				else
					random_access_loop(filename[i], n);
			}
			else{
				if(i == n-1)
					sequence_access(filename[i], n);
				else
					sequence_access_loop(filename[i], n);
			}
			exit(1);
		}else{
			//parent process
			//wait(NULL);
		}
	}
	return 0;
}
