#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/MessageBuffer.h"

#include "../headers/Logger.h"

MessageBuffer*  CreateNewBuffer(int length){
    MessageBuffer* bf = (MessageBuffer*)malloc(sizeof(MessageBuffer));

    bf->head = 0;
    bf->tail = 0;
    bf->isFull = 0;
    bf->maxLength = length;
    bf->buffer = (Message*)malloc(length*sizeof(Message));

    return bf;
}



void AddMessageToBuffer(MessageBuffer* buffer ,Message msg){
    printf("Adding Message\n");
    printf("%s %s %s %s\n",msg.sender, msg.receiver,msg.timestamp, msg.text);
    // If the msg is duplicate... update the aware nodes
    int duplicate = CheckDuplicateInBuffer(buffer, msg);
    if(duplicate > -1){
        printf("Message already exists\n");
        logg("duplicate_message",LOGBUF);
        CheckAndAddNode(&buffer->buffer[duplicate].aNodes, msg.aNodes.nodes[0]);
        if(msg.aNodes.length > 1){
            CheckAndAddNode(&buffer->buffer[duplicate].aNodes, msg.aNodes.nodes[1]);
        }
        PrintBuffer(buffer);
        return;
    }
    buffer->buffer[buffer->head].delivered = 0;
    memcpy(buffer->buffer[buffer->head].sender, msg.sender, 4*sizeof(char));
    buffer->buffer[buffer->head].sender[4] = '\0';
    memcpy(buffer->buffer[buffer->head].receiver, msg.receiver, 4*sizeof(char));
    buffer->buffer[buffer->head].receiver[4] = '\0';
    memcpy(buffer->buffer[buffer->head].timestamp, msg.timestamp, 10*sizeof(char));
    buffer->buffer[buffer->head].timestamp[10] = '\0';
    memcpy(buffer->buffer[buffer->head].text, msg.text, 256*sizeof(char));
    buffer->buffer[buffer->head].text[100] = '\0';
    InitAwareNodes(&buffer->buffer[buffer->head].aNodes);
    CheckAndAddNode(&buffer->buffer[buffer->head].aNodes, msg.aNodes.nodes[0]);
    if(msg.aNodes.length > 1){
        CheckAndAddNode(&buffer->buffer[buffer->head].aNodes, msg.aNodes.nodes[1]);
    }

    logg("Added",LOGBUF);
    printf("Added Message\n");

    if(buffer->isFull){
        buffer->tail++;
        buffer->tail = buffer->tail%buffer->maxLength;
    }

    buffer->head++;
    buffer->head = buffer->head%buffer->maxLength;
    buffer->isFull = buffer->head == buffer->tail;

    printf("Done\n");

    PrintBuffer(buffer);
}



int CheckDuplicateInBuffer(MessageBuffer* buffer, Message msg){
    
    int i = buffer->tail;
    while (1)
    {
        if(CompareMessages(buffer->buffer[i], msg)){
            printf("Duplicate Detected\n");
            return i;
        }
        i++;
        i = i%buffer->maxLength;
        if(i == buffer->tail)
            return -1;
    }
    
    return -1;
}


void PrintBuffer(MessageBuffer* buffer){
    int start,end;
    if(buffer->isFull){
        start = 0;
        end = buffer->maxLength;
    }else{
        start = 0; 
        end = buffer->head;
    }

    end = buffer->head;
    start = buffer->head - 10;
    if(start < 0){
        start = 0;
    }
    printf("========== Printing All Messages ========== \n");
    for(int i = start; i <end; i++){
        // printf("------ Message %d -------\n",i);
        // printf("\t Sender [%s]\n",buffer->buffer[i].sender);
        // printf("\t Receiver [%s]\n",buffer->buffer[i].receiver);
        // printf("\t Time [%s]\n",buffer->buffer[i].timestamp);
        // printf("\t Msg [%s]\n",buffer->buffer[i].text);
        PrintMessage(buffer->buffer[i]);
    }
    printf("======= Done Printing All Messages ======== \n");
}