#include "../headers/Logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
FILE *createdMessages, *sentMessages, *receivedMessages, *senderScanning, *bufferUpdate;


void InitLog(){
    createdMessages = fopen("createdMessages.log", "w+");
    sentMessages = fopen("sentMessages.log", "w+");
    receivedMessages = fopen("receivedMessages.log", "w+");
    senderScanning = fopen("senderScanning.log", "w+");
    bufferUpdate = fopen("bufferUpdate.log", "w+");

    fclose(createdMessages);
    fclose(senderScanning);
    fclose(sentMessages);
    fclose(receivedMessages);
    fclose(bufferUpdate);
}
void logg(char* data, int file){
    //printf("loggeeeeeeeeeeeeeeeeeeeeeeeeeer\n");
    FILE* temp;
    switch (file)
    {
    case LOGBUF:
        temp = fopen("bufferUpdate.log", "a");
        break;
    case LOGCREATE:
        temp = fopen("createdMessages.log", "a");
        break;
    case LOGREC:
        temp = fopen("receivedMessages.log", "a");
        break;
    case LOGSENDER:
        temp = fopen("senderScanning.log", "a");
        break;
    case LOGSENT:
        temp = fopen("sentMessages.log", "a");
        break;
    default:
        break;
    }

    fprintf(temp, "%d,%s\n",time(NULL) ,data);
    printf("%d,%s\n",time(NULL) ,data);
    fclose(temp);
}
