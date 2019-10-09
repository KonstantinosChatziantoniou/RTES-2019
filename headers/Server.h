#ifndef SERVERH
#define SERVERH



#include "../headers/Worker.h"
#include "../headers/MessageBuffer.h"
int StartTCPServer (int port, int buffsize, MessageBuffer* mb);


#endif