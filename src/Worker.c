#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "../headers/Worker.h"
#include "../headers/Message.h"
#include "../headers/MessageBuffer.h"

#include "../headers/Concurrency.h"

typedef struct t_data{
    MessageBuffer* buffer;
    Message *msg;
}ThreadDataWorker;




void *t_GiveDataToWorker(void* data){
    ThreadDataWorker* tdw = (ThreadDataWorker*)data;
    LockBuffer("worker");
    AddMessageToBuffer(tdw->buffer, *tdw->msg);
    UnlockBuffer("worker");
}


void GiveDataToWorker(MessageBuffer *buffer, WorkerData wd){
    printf("Worker got data [%s] [%s] [%d]\n",wd.message,wd.address,wd.length);
    Message* msg = CreateMessageFromString(wd.message,wd.length);
    InitAwareNodes(&msg->aNodes);
    CheckAndAddNode(&msg->aNodes, wd.address);
    CheckAndAddNode(&msg->aNodes, msg->sender);
    PrintMessage(*msg);
    ThreadDataWorker* twd = (ThreadDataWorker*)malloc(sizeof(ThreadDataWorker));
    twd->buffer = buffer;
    twd->msg = msg;
    pthread_t t;
    pthread_create(&t,NULL,t_GiveDataToWorker, twd);
}


