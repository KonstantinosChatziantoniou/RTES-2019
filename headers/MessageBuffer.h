#ifndef MESSAGEBUFFER_H
#define MESSAGEBUFFER_H

#include "Message.h"

typedef struct msg_buffer
{
    Message*  buffer;
    int head;
    int tail;
    int maxLength;
    int isFull;
}MessageBuffer;



MessageBuffer*  CreateNewBuffer(int length);


void AddMessageToBuffer(MessageBuffer* buffer ,Message msg);


int CheckDuplicateInBuffer(MessageBuffer* buffer, Message msg);

void PrintBuffer(MessageBuffer* buffer);
#endif 