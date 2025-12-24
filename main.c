#include "test.h"
#include <dlfcn.h>

pthread_mutex_t buzzer_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t led_lock = PTHREAD_MUTEX_INITIALIZER;



int main(){
    struct sigaction sa;
    struct rlimit rl;
    int fd0,fd1,fd2,i,pid;
    umask(0);
    if(getrlimit(RLIMIT_NOFILE,&rl)<0){
        perror("getlimit");
    }

    if((pid = fork())<0){
        perror("fork");
        
    }
    else if (pid !=0){
            return 0;
    }

    setsid();

    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
if(sigaction(SIGHUP,&sa,NULL)<0){ perror("sigaction");}
       for(int i = 1; i<NSIG; i++){
        signal(i,SIG_IGN);
    }
chdir("/");
if(rl.rlim_max == RLIM_INFINITY){
    rl.rlim_max = 1024;
}
for(int i = 0; i<rl.rlim_max;i++){
    close(i);
}

close(0); close(1); close(2);
fd0 = open("/dev/null", O_RDWR);
fd1 = dup(0);
fd2 = dup(0);
int serv_sock, clnt_sock,jodo_state;
struct sockaddr_in serv_addr, clnt_addr;
socklen_t clnt_addr_len;
pthread_t led_jodo, led_bright, buzzer_timer,buzzer_sing;

wiringPiSetup( ); 			/* wiringPi 초기화 */

//led init
pinMode(LED, OUTPUT); 
softPwmCreate(LED, 0, 255); 	/* PWM의 범위 설정 */

//buzzer init
softToneCreate(SPKR);


//jodo sensor,led init
pinMode(JODO_LED,OUTPUT);
pinMode(JODO,INPUT);


void*(*bright_ptr)(void*);
void*(*ledon_ptr)(void*);
void*(*ledoff_ptr)(void*);
void*(*jodoled_ptr)(void*);
void*(*getjodo_ptr)(void*);
void*(*sing_ptr)(void*);
void*(*timer_ptr)(void*);
void*(*buzzerinit_ptr)(void*);

void* handle = dlopen("./libveda_test.so",RTLD_LAZY);

bright_ptr = dlsym(handle,"bright");
ledon_ptr = dlsym(handle,"ledon");
ledoff_ptr = dlsym(handle,"ledoff");
jodoled_ptr = dlsym(handle,"jodoled");
getjodo_ptr = dlsym(handle,"getjodo");
sing_ptr = dlsym(handle,"sing");
timer_ptr = dlsym(handle,"timer");
buzzerinit_ptr = dlsym(handle,"buzzerinit");

buzzerinit_ptr(NULL); 
pthread_create(&led_jodo,NULL,jodoled_ptr,NULL);



serv_sock = socket(AF_INET,SOCK_STREAM,0);
if(serv_sock<0) {
    perror("socket");
        exit(1);
    }
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;   // 0.0.0.0
    serv_addr.sin_port = htons(PORT);
    
    if(bind(serv_sock, (struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
    {
        perror("bind");
        exit(1);
    }
    if(listen(serv_sock, 5)<0)
    {
        perror("listen");
        exit(1);
    }
    
    while(1){
    
    clnt_addr_len = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr, &clnt_addr_len);
    if(clnt_sock < 0 ) continue;
    
    
    int state = MAIN;
    packet pkt;
    int client_input;
    
    while(1){
        switch(state){
            case MAIN:
            pkt.flag = INPUT_;
            sprintf(pkt.buf,"\n[main menu]\n 1.LED 제어\n 2.조도센서\n 3.타이머\n 4.부저\nvalue: ");
            break;
            
            case LED:
            pkt.flag = INPUT_;
            sprintf(pkt.buf,"\n[LED control]\n 1.밝기조절\n 2.LED ON\n 3.LED OFF\n 4.뒤로가기\nvalue: ");
            break;

            case BRIGHT:
            pkt.flag = INPUT_;
            sprintf(pkt.buf,"\n[밝기 조절]\n 0~255사이 값을 입력하세요: ");
            break;

            case TIMER:
            pkt.flag = INPUT_;
            sprintf(pkt.buf,"\n[타이머]\n 1~9 사이 값을 입력하세요: ");
            break;
        }

       if(write(clnt_sock,&pkt,sizeof(pkt))< 0 ) break;
        if(pkt.flag == INPUT_) {
            if(read(clnt_sock,&client_input,sizeof(int))<= 0) break;

            if (state == MAIN){
                if(client_input == 1) state = LED;
                else if(client_input == 2){
                    pkt.flag = PRINT;
                    int jodo_val; 
                    getjodo_ptr(&jodo_val);
                    sprintf(pkt.buf,"조도센서 값:%d",jodo_val);
                    write(clnt_sock, &pkt,sizeof(pkt));
                }
                else if(client_input == 3) state = TIMER;
                else if (client_input == 4){
                pthread_create(&buzzer_sing,NULL,sing_ptr,NULL);
            }
            }
            else if(state == LED){
                if (client_input ==2){
                ledon(NULL);
                pkt.flag = PRINT;
                sprintf(pkt.buf,"LED on! 메인으로 돌아갑니다\n");
                write(clnt_sock,&pkt,sizeof(pkt));
                state = MAIN;
                }
                else if (client_input == 3){
                ledoff_ptr(NULL);
                pkt.flag = PRINT;
                sprintf(pkt.buf,"LED off! 메인으로 돌아갑니다\n");
                write(clnt_sock,&pkt,sizeof(pkt));
                    state = MAIN;
                } 
                else if(client_input == 4){state = MAIN;}
                else if(client_input == 1){state = BRIGHT;}
            }
            else if(state == BRIGHT){ 
                pthread_create(&led_bright,NULL,bright_ptr,&client_input);
                pkt.flag = PRINT;
                sprintf(pkt.buf,"조절 완료! 메인으로 돌아갑니다\n");
                write(clnt_sock,&pkt,sizeof(pkt));
                state = MAIN;
            }
            else if(state == TIMER){
                //타이머 함수 실행
                pthread_create(&buzzer_timer,NULL,timer_ptr,&client_input);
                state = MAIN;
                pkt.flag = PRINT;
                sprintf(pkt.buf,"설정 완료! 메인으로 돌아갑니다\n");
                write(clnt_sock,&pkt,sizeof(pkt));
            }
        }
    }
    
}
    return 0;
    
}


