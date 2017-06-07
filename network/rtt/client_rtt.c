#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>


static inline unsigned long long rdtsc(void) {
    unsigned lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((unsigned long long)hi << 32) | (unsigned long long)lo);
}
#define loops 5

int main(int argc , char *argv[])
{      
    int sockfd, port, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    //get port and create socket
    port = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0){
        perror("ERROR opening socket");
        exit(0);
    }

    server = gethostbyname(argv[1]);

    //serv_addr.sin_addr.s_addr=(u_long)server ->h_addr;
    //printf("%s\n", inet_ntoa(serv_addr.sin_addr));

    //printf("server:%s\n",server);
    //printf("port:%d\n",port );

    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    //connection between server and client
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(port);
    //if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
    //   perror("ERROR connecting");

    //calcaulate round time
    unsigned long long begin,end;
    unsigned long long total = 0;
    char buffer[64];
    bzero(buffer,64);
    int counter=0;

    for (int j=0; j < loops; j++) {
        begin = rdtsc();

        if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
            perror("ERROR connecting");
            end= begin;

        }
        else{
            send(sockfd, &buffer, 64, 0);
            int n=recv(sockfd, &buffer, 4, 0);
            if(n!=0){
                end = rdtsc();
                counter++;
            }
            else{
                end=begin;
            }
        }
        
        total += (end - begin);
    }

    printf ("Round Trip Cycles = %f\n", (total* 1.0*0.34/(counter*1e6)));
    
    return 0;

}
