#include <stdio.h>   
#include <stdlib.h>     
#include <string.h>     
#include <unistd.h>  
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <wiringPi.h>
#include <softPwm.h>
#include <softTone.h>
#include <pthread.h>
#include <signal.h>
#include <fcntl.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/resource.h>

enum{MAIN,LED,BRIGHT,TIMER };
enum {INPUT_, PRINT};


typedef struct packet{
int flag;
char buf[1024];
}packet;

#define PORT 60000
#define BUF_SIZE 1024
#define LED 1
#define JODO_LED 8
#define JODO 4
#define SPKR 6 
#define TOTAL 32 

//led 제어 함수
void* bright(void* arg);
void* ledon(void* arg);
void* ledoff(void* arg);


//조도센서 함수
void* jodoled(void* arg);
void* getjodo(void* arg);



//부저 함수
void* buzzerinit(void* arg);
void* sing(void* arg);


//타이머함수
void* timer(void *n);




