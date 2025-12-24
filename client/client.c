#include <stdio.h>   
#include <stdlib.h>     
#include <string.h>     
#include <unistd.h>  
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <signal.h>

#define PORT 60000
#define BUF_SIZE 1024

typedef struct packet{
int flag;
char buf[1024];
}packet;

enum {INPUT_, PRINT};

int main(){
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
        for(int i = 1; i<NSIG; i++){
        if (i == SIGINT) continue;
        signal(i,SIG_IGN);
    }

    packet pkt;
    int sock,user_input;
    struct sockaddr_in serv_addr;
    int tx_data;
    char rx_buf[BUF_SIZE];

    sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock<0){
        perror("socket");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("192.168.0.15");
    serv_addr.sin_family = AF_INET;

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))<0){
        perror("connect");
        exit(1);
    }

    printf("연결됨!\n");
    while(1){
        int len = read(sock,&pkt,sizeof(pkt));
        if(len<=0)break;

        printf("%s",pkt.buf);

        if(pkt.flag == INPUT_){
            scanf("%d", &user_input);
            write(sock,&user_input,sizeof(int));
        }
        else if(pkt.flag == PRINT){
            printf("\n");
        }

    }

    close(sock);
    return 0;
}