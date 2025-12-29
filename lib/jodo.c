#include "test.h"

void* jodoled(void* arg){
    printf("조도센서 실행");
    while(1)
    {
        if(digitalRead(JODO) == HIGH){
            digitalWrite(JODO_LED,1);
        }
        else digitalWrite(JODO_LED,0);
        delay(500);
    }
}


void* getjodo(void* arg){
        int val = digitalRead(JODO);
        *(int*)arg = val;
}
