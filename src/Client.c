#include "../headers/Client.h"
#include "../headers/MessageBuffer.h"
#include "../headers/Message.h"
#include "../headers/AwareNodes.h"
#include "../headers/Concurrency.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/time.h>
#include <netdb.h> 
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <sys/types.h>
#include <strings.h>
#include "../headers/Logger.h"

MessageBuffer* globalBuffer;
AwareNodes knownNodes;

#define SEND_RATE 500
#define CREATE_RATE 5000


int createConnectionAndSend(int port, char* msg);
void SetupClientWorker(MessageBuffer* mb){
    globalBuffer = mb;
    printf("asdasdasd\n");
    knownNodes = GetKnwonNodes();
    printf("asdasdakomplesd\n");
    return;
}


void *creatorThread(){
    printf("creatorThread started\n");
    int counter = 0;
    while(1){
        sleep(rand()%240 + 60);
        LockBuffer("creator");
        printf("creatorThread after lock\n");
        Message msg;
        strcpy(msg.sender, "8941");
        msg.sender[4] = '\0';
        int i = rand()%knownNodes.length;
        char i_s[16];
        sprintf(i_s, "%s", knownNodes.nodes[i]);
        i_s[4] = '\0';
        strcpy(msg.receiver, i_s);
        msg.receiver[4] = '\0';
        char timeNow[14];
        sprintf(timeNow,"%u", (unsigned)time(NULL) );
        timeNow[10] = '\0';
        strcpy(msg.timestamp,timeNow);
        msg.timestamp[10] = '\0';
        char mg[30];
        msg.aNodes.length = 1;
        msg.aNodes.nodes = (char**)malloc(sizeof(char*));
        msg.aNodes.nodes[0] = (char*)malloc(5*sizeof(char));
        strcpy(msg.aNodes.nodes[0], "8941");
        sprintf(mg, "kotsarelos#%d",counter);
        strcpy(msg.text,mg);
        char prt[500];
        sprintf(prt, "%s,%s,%s", msg.receiver, msg.timestamp, msg.text);
        logg(prt, LOGCREATE);
        PrintMessage(msg);
        AddMessageToBuffer(globalBuffer, msg);
        printf("Added New Message\n");
        counter++;
        UnlockBuffer("creator");
    }   
}

void *senderThread(){
    int pos = 0;
    while(1){
        if(globalBuffer->head == 0 && !globalBuffer->isFull){
            sleep(1);
            continue;
        }
        break;
    }
    while(1){
        sleep(1);
        LockBuffer("sender");
        Message* msg = &globalBuffer->buffer[pos];
        // while (msg->delivered)
        // {
        //     printf("POOOOOOOOOOOOOS %d\n",pos);

        //     pos++;
        //     pos = pos%globalBuffer->maxLength;
        //     if (pos == globalBuffer->head)  
        //     {
        //         if(pos == globalBuffer->head){
        //             pos = globalBuffer->tail;
                   
        //         }
        //     }
            
        //     msg = &globalBuffer->buffer[pos];
        // }
        
        AwareNodes *an = ReturnUnawareNodes(msg->aNodes);
        // for(int i = 0;i < an->length; i++){
        //     printf('UNAWARE\t\t\t\t%s\n',an->nodes[i]);
        // }
        if(an->length == 0){
            msg->delivered = 1; 
            char prt[500];
            sprintf(prt, "delivered,%s,%s,%s,%s",msg->sender, msg->receiver, msg->timestamp, msg->text);
            logg(prt, LOGSENDER);
        }
        if(!strcmp("8941",msg->receiver)){
            msg->delivered = 1;
             char prt[500];
            sprintf(prt, "forme,%s,%s,%s,%s",msg->sender, msg->receiver, msg->timestamp, msg->text);
            logg(prt, LOGSENDER);
        }
        if(!msg->delivered){
            for(int i = 0; i < an->length; i++){
                printf("------ Sending Message To %s\n", an->nodes[i]);
                char temp[512];
                sprintf(temp, "%s_%s_%s_%s", msg->sender, msg->receiver, msg->timestamp, msg->text);
                printf("message = %s\n", temp);
                if(!createConnectionAndSend(atoi(an->nodes[i]), temp)){
                    CheckAndAddNode(&msg->aNodes, an->nodes[i]);
                }else{
                    printf("EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEERRRRRRRRRRRRRRRRRRRR SENDING to %s\n", an->nodes[i]);
                    char pt[500];
                    sprintf(pt, "%s,%s","send failed", an->nodes[i]);
                    logg(pt,LOGSENT);
                }
            }
        }else{
            logg("alreadydelivered",LOGSENDER);
        }
        
        
        pos++;
        pos = pos%globalBuffer->maxLength;
        if (pos == globalBuffer->head)  
        {
            pos = globalBuffer->tail;
            logg("Starting from tail",LOGSENDER);
            // UnlockBuffer("sender");
            // sleep(1);
            // LockBuffer("sender");
        }
        UnlockBuffer("sender");
    }
}
void InitClientWorker(){
    pthread_t t1,t2;
    pthread_create(&t1, NULL, creatorThread, NULL);
    pthread_create(&t2, NULL, senderThread, NULL);
}



#define SA struct sockaddr 

int createConnectionAndSend(int port, char* msg){
    int sockfd, connfd; 
    struct sockaddr_in servaddr, cli; 
  
    // socket create and varification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        return 1; 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    int x1,x2;
    x1 = port%100;
    x2 = port/100;
    char dress[20];
    sprintf(dress, "10.0.%d.%d",x2,x1);
    servaddr.sin_addr.s_addr = inet_addr(dress); 
    servaddr.sin_port = htons(2288); 
    
    printf("%s addddddddddddddd\n",dress);
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    int cond;
    cond = connect(sockfd, (SA*)&servaddr, sizeof(servaddr));
    if (cond  != 0) { 
    printf("connection with the server failed...\n"); 
        return 1;
    } 
    else
        printf("connected to the server..\n"); 

    // function for chat 
    write(sockfd, msg, (strlen(msg)+1)*sizeof(char)); 
    char pp[500];
    sprintf(pp,"%s to %d",msg,port);
    logg(pp, LOGSENT);
    // close the socket 
    close(cond);
    close(sockfd); 
    return 0;
}
