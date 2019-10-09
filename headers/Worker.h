#ifndef WORKER_H
#define WORKER_H

#include "../headers/MessageBuffer.h"



typedef struct worker_in_data
{
    char* message;
    char* address;
    int length;
}WorkerData;




void GiveDataToWorker(MessageBuffer *buffer, WorkerData wd);











#endif