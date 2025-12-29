 #include "test.h"

extern pthread_mutex_t buzzer_lock;


const int segpins[7] = {25,24,23,22,21,30,14};

static const int nums[10][7]= {
    // A  B  C  D  E  F  G
    {1, 1, 1, 1, 1, 1, 0}, // 0
    {0, 1, 1, 0, 0, 0, 0}, // 1
    {1, 1, 0, 1, 1, 0, 1}, // 2
    {1, 1, 1, 1, 0, 0, 1}, // 3
    {0, 1, 1, 0, 0, 1, 1}, // 4
    {1, 0, 1, 1, 0, 1, 1}, // 5
    {1, 0, 1, 1, 1, 1, 1}, // 6
    {1, 1, 1, 0, 0, 0, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 1, 1, 0, 1, 1}  // 9
};

static int notes[] = { 	/* 학교종을 연주하기 위한 계이름 */
	391, 391, 440, 440, 391, 391, 329.63, 329.63, \
	391, 391, 329.63, 329.63, 293.66, 293.66, 293.66, 0, \
	391, 391, 440, 440, 391, 391, 329.63, 329.63, \
	391, 329.63, 293.66, 329.63, 261.63, 261.63, 261.63, 0
};


void* timer(void *n) {
    for(int i = *(int*)n; i>=0;i--){
        for(int j = 0; j<7; j++) digitalWrite(segpins[j],!nums[i][j]);
        if (i == 0){
                    pthread_mutex_lock(&buzzer_lock);
                for (int k = 0; k < TOTAL; ++k) {
                    softToneWrite(SPKR, notes[k]); /* 톤 출력 : 학교종 연주 */
                    delay(280); 		/* 음의 전체 길이만큼 출력되도록 대기 */
                }
                pthread_mutex_unlock(&buzzer_lock);
                break;
         } 
         delay(1000);  
    }
}

void* sing(void* arg){
    pthread_mutex_lock(&buzzer_lock);
    for (int i = 0; i < TOTAL; ++i) {
        softToneWrite(SPKR, notes[i]); /* 톤 출력 : 학교종 연주 */
        delay(280); 		/* 음의 전체 길이만큼 출력되도록 대기 */
    }
    pthread_mutex_unlock(&buzzer_lock);
}

void* buzzerinit(void* arg){
    for(int i = 0; i<7; i++)
    pinMode(segpins[i],OUTPUT);
}