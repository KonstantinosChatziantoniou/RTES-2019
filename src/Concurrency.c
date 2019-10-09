#include "../headers/Concurrency.h"


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

sem_t bufferSem;



void InitSemaphoreForBuffer(){
    sem_init(&bufferSem, 0, 1);
}


void LockBuffer(char* msg){
    sem_wait(&bufferSem);
    int a;
    sem_getvalue(&bufferSem, &a);
    printf("Locking%s %d\n", msg,a);
}

void UnlockBuffer(char* msg){
    sem_post(&bufferSem);
    int a;
    sem_getvalue(&bufferSem, &a);
    printf("Unlocking %s %d\n", msg,a);
}