#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../headers/Message.h"



void InitMessage(Message* msg){
    InitAwareNodes(&msg->aNodes);
    msg->delivered = 0;
}


Message* CreateMessageFromString(char* data, int length){
    Message* msg = (Message*)malloc(sizeof(Message));
    if(length < 5 + 5 + 11 + 1){
        exit(16);
    }
    memcpy(msg->sender, &data[0], 4*sizeof(char));
    msg->sender[4] = '\0';
    memcpy(msg->receiver, &data[5], 4*sizeof(char));
    msg->receiver[4] = '\0';
    memcpy(msg->timestamp, &data[10], 10*sizeof(char));
    msg->timestamp[10] = '\0';
    memcpy(msg->text, &data[21], (length-5-5-11)*sizeof(char));
    msg->text[length-5-5-11] = '\0';
    msg->delivered = 0;
    InitMessage(msg);

    return msg;
}


int CompareMessages(Message m1, Message m2){
    if(strcmp(m1.timestamp, m2.timestamp)){
        return 0;
    }

    // if(strcmp(m1.sender, m2.sender)){
    //     return 0;
    // }

    if(strcmp(m1.receiver, m2.receiver)){
        return 0;
    }

    if(strcmp(m1.text, m2.text)){
        return 0;
    }

    return 1;

    
}

void PrintMessage(Message msg){
    printf("---- Printing Message ----\n");
    printf("Sender:\t\t%s\n", msg.sender);
    printf("Receiver:\t%s\n", msg.receiver);
    printf("Timestamp:\t%s\n", msg.timestamp);
    printf("Text:\t\t%s\n", msg.text);
    PrintAwareNodes(msg.aNodes);
    printf("---- End Printing Message ----\n");
}