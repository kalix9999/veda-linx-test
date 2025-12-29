#include "test.h"

extern pthread_mutex_t led_lock;

void* bright(void* arg)
{        
    pthread_mutex_lock(&led_lock);
    int bright; 
    bright = *(int*)arg;
    for(int i = 0; i <=bright; i++) {
        softPwmWrite(LED, i&255); 	/* PWM 값을 출력: LED 켜기 */
        delay(5);
    }
    pthread_mutex_unlock(&led_lock);

}

void * ledon(void* arg){
pthread_mutex_lock(&led_lock);
softPwmWrite(LED, 255);
pthread_mutex_unlock(&led_lock);
}


void* ledoff(void* arg){
pthread_mutex_lock(&led_lock);
softPwmWrite(LED,0);
pthread_mutex_unlock(&led_lock);

}