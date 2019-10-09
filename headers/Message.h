#ifndef MESSAGE_H
#define MESSAGE_H

#include "AwareNodes.h"

typedef struct  message{
    char sender[5];
    char receiver[5];
    char timestamp[11];
    char text[256];
    AwareNodes aNodes;
    int delivered;
}Message;



Message* CreateMessageFromString(char* data, int length);


/** 
 *  Returns 1 if the messages are equal.
 */
int CompareMessages(Message m1, Message m2);



void PrintMessage(Message msg);


void InitMessage(Message* msg);





#endif