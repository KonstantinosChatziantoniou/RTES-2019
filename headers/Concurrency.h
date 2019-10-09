#ifndef CONC_H
#define CONC_H

#include <semaphore.h>


void InitSemaphoreForBuffer();

void LockBuffer(char* msg);
void UnlockBuffer(char* msg);


#endif