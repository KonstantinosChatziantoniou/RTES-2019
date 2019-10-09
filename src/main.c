#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/AwareNodes.h"
#include "../headers/Message.h"
#include "../headers/Worker.h"
#include "../headers/MessageBuffer.h"
#include "../headers/Server.h"
#include "../headers/Client.h"
#include "../headers/Concurrency.h"
#include "../headers/Logger.h"

void testAwareNodes();
void testMessage();


void test(){


    static int i = 0;

    while(1){
        i++;
        if(i%1000 == 0){
            printf("%d\n",i);
            return;
        }

        if(i == 10001){
            exit(0);
        }
    }
}
int main(int argc, char** argv){

    InitLog();
    MessageBuffer* mb = CreateNewBuffer(2000);
    setUpKnownNodes();
    InitSemaphoreForBuffer();
    SetupClientWorker(mb);
    InitClientWorker();
    PrintKnownNodes();
    StartTCPServer(atoi(argv[1]), 2000, mb);
    //testAwareNodes();
    testMessage();
    while(1)
        test();

    return 0;
}


void testAwareNodes(){
    AwareNodes an;
    InitAwareNodes(&an);

    printf("Init length %d\n", an.length);

    AddNewAwareNode(&an, "8941");
    AddNewAwareNode(&an, "8872");

    PrintAwareNodes(an);

    int duplicate = CheckDuplicate(an, "8941");
    if(duplicate){
        printf("duplicate\n");
    }else{
        printf("eeeeeeeerrr\n");
    }

    duplicate = CheckAndAddNode(&an, "8941");
    if(!duplicate){
        printf("duplicate\n");
    }else{
        printf("eeeeeeeerrr\n");
    }

    duplicate = CheckAndAddNode(&an, "8991");
    if(!duplicate){
        printf("added new \n");
    }else{
        printf("eeeeeeeerrr\n");
    }

    PrintAwareNodes(an);

    DeleteAwareNodes(&an);
}


void testMessage(){
    char* m1 = "1234_4567_1234123412_asdasd";
    char* m2 = "1224_4567_1234123412_asdasaad";
    char* m3 = "1234_4567_1234123412_asdasd";

    Message* msg1 = CreateMessageFromString(m1, strlen(m1));
    
    Message* msg2 = CreateMessageFromString(m2, strlen(m2));
    Message* msg3 = CreateMessageFromString(m3, strlen(m3));

    PrintMessage(*msg1);
    PrintMessage(*msg2);

    int cmp = CompareMessages(*msg1, *msg2);

    if(cmp){    
        printf("m1 m2 equal\n");
    }else{
        printf("m1 m2 differnt\n");
    }


    cmp = CompareMessages(*msg1, *msg3);

    if(cmp){    
        printf("m1 m3 equal\n");
    }else{
        printf("m1 m3 differnt\n");
    }
    MessageBuffer* mb = CreateNewBuffer(200);
    WorkerData wd;
    wd.address = "8888";
    wd.message = m1;
    wd.length = strlen(m1);
    GiveDataToWorker(mb, wd);

    wd.address = "8878";
    wd.message = m1;
    wd.length = strlen(m1);
    GiveDataToWorker(mb, wd);



}