#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/AwareNodes.h"
#include "../headers/Logger.h"

AwareNodes knownNodes;

void setUpKnownNodes(){
    
    FILE* fp = fopen("awarenodes.txt","r");
    int length;
    fscanf(fp, "%d\n", &length);
    knownNodes.nodes = (char**)malloc(length*sizeof(char*));
    knownNodes.length = length;
    for(int i = 0; i < length; i++){
        knownNodes.nodes[i] = (char*)malloc(5*sizeof(char));
        fscanf(fp, "%s\n", knownNodes.nodes[i]);
    }
    fclose(fp);
}


int CheckDuplicate(AwareNodes an, char* nd){
    int length = an.length;
    if(length == 0){
        return 0;
    }

    for(int i = 0; i < length; i++){

        if(strcmp(an.nodes[i], nd)){
           continue;
        }
        return 1;
    }
    return 0;
}


void AddNewAwareNode(AwareNodes *an, char* node){

    int length = an->length;

    if(length == 0){
        an->nodes = (char**)malloc(sizeof(char*));
        an->nodes[0] = (char*)malloc(5*sizeof(char));
        memcpy(an->nodes[0], node, 5*sizeof(char));
        an->length++;
        return;
    }


    char** temp_an = (char**) malloc((length+1)*sizeof(char*));

    memcpy(temp_an, an->nodes, length*sizeof(char*));
    temp_an[length] = (char*)malloc(5*sizeof(char));
    memcpy(temp_an[length], node, 5*sizeof(char));
    free(an->nodes);
    an->nodes = temp_an;
    an->length++;

    return;
}

int CheckAndAddNode(AwareNodes *an, char* node){

    if(CheckDuplicate(*an, node)){
        logg("duplicate_node",LOGBUF);
        return 0;
    }
    AddNewAwareNode(an, node);
    logg("added_node",LOGBUF);
}


void PrintAwareNodes(AwareNodes an){
    int length = an.length;
    printf("----- Printing AwareNodes -----\n");
    for(int i = 0; i < length; i++){
        printf("Node %d) %s\n", i, an.nodes[i]);
    }
    printf("----- End Printing AwareNodes -----\n");
}

void InitAwareNodes(AwareNodes *an){
    an->length = 0;
    return;
}

void DeleteAwareNodes(AwareNodes *an){
    int length = an->length;
    for(int i = 0; i < length; i++){
        free(an->nodes[i]);
    }

    free(an->nodes);
}


/**
 * Returns all the knows that are not aware of the message
 * in a AwareNodes struct.
 */
AwareNodes* ReturnUnawareNodes(AwareNodes nd){
    AwareNodes *rd = (AwareNodes*)malloc(sizeof(AwareNodes));
    int loc[100];
    int counter = 0;
    for(int i = 0; i < knownNodes.length; i++){
        if(!CheckDuplicate(nd, knownNodes.nodes[i])){
            loc[counter++] = i;
        }
    }

    rd->length = counter;
    rd->nodes = (char**)malloc(counter*sizeof(char*));
    
    for(int i = 0; i < counter; i++){
        int pos = loc[i];
        rd->nodes[i] = (char*)malloc(5*sizeof(char));
        memcpy(rd->nodes[i], knownNodes.nodes[pos], 5*sizeof(char));
    }

    return rd;
}

void PrintKnownNodes(){
    PrintAwareNodes(knownNodes);
}


AwareNodes GetKnwonNodes(){
    return knownNodes;
}