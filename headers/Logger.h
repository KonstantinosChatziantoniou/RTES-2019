#ifndef LOGGER_H
#define LOGGER_H

#define LOGREC 5
#define LOGSENDER 4
#define LOGBUF 3
#define LOGSENT 2
#define LOGCREATE 1


void InitLog();
void logg(char* data, int file);


#endif

