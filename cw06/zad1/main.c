#include <stdio.h>
#include "codes.h"
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>

int Q_ID;
int CLIENT_COUNT = 0;
int CLIENTS_Q_ID[MAX_CONN];
bool CLIENTS_BUSY[MAX_CONN];
bool CLIENTS_ALIVE[MAX_CONN];
int ALIVE_COUNT = 0;
bool STOPING = false;

void sendMsg(int qid, int type, char *text)
{
    struct msgbuf msg;
    msg.mtype = type;
    snprintf(msg.mtext, sizeof(msg.mtext), "%s", text);

    if (msgsnd(qid, (void *)&msg, sizeof(msg.mtext), IPC_NOWAIT) == -1)
    {
        perror("błąd msgsnd");
        exit(EXIT_FAILURE);
    }
    printf("%i<=\n%s\n", qid, msg.mtext);
}

void sendIntMsg(int qid, int type, int a)
{
    char text[80];
    snprintf(text, sizeof(text), "%i", a);
    sendMsg(qid, type, text);
}

void handleInit(struct msgbuf msg)
{
    printf("INIT\n");
    int id = CLIENT_COUNT;
    CLIENTS_Q_ID[id] = atoi(msg.mtext);    
    sendIntMsg(CLIENTS_Q_ID[id], CMD_ID, id);
    CLIENTS_BUSY[id] = false;
    CLIENTS_ALIVE[id] = true;
    CLIENT_COUNT++;
    ALIVE_COUNT++;
    // if (CLIENT_COUNT == 5)
    //     exit(0);
}
void handleList(struct msgbuf msg)
{
    int id = atoi(msg.mtext);
    printf("LIST\n");
    char listText[MAX_MSG_LEN] = "";
    for (int i = 0; i < CLIENT_COUNT; i++)
    {
        if (CLIENTS_ALIVE[i])
        {
            char formatT[30];
            sprintf(formatT, "%i:%s\n", i, CLIENTS_BUSY[i] ? "busy" : "free");
            strcat(listText, formatT);
        }
    }
    sendMsg(CLIENTS_Q_ID[id], CMD_LIST, listText);
}
void handleStop(struct msgbuf msg)
{
    int id = atoi(msg.mtext);
    printf("STOP\n");
    CLIENTS_ALIVE[id] = false;
    ALIVE_COUNT--;
}
void handleDisconnect(struct msgbuf msg)
{
    int id = atoi(msg.mtext);
    printf("DISCONNECT\n");
    CLIENTS_BUSY[id] = false;
}
void handleConnect(struct msgbuf msg)
{
    int id, idPartner;
    sscanf(msg.mtext, "%i:%i", &id, &idPartner);
    printf("CONNECT\n");
    if(idPartner < CLIENT_COUNT && !CLIENTS_BUSY[idPartner] && CLIENTS_ALIVE[idPartner])
    {
        CLIENTS_BUSY[id] = true;
        CLIENTS_BUSY[idPartner] = true;
        sendIntMsg(CLIENTS_Q_ID[idPartner], CMD_CONNECT, CLIENTS_Q_ID[id]);
        sendIntMsg(CLIENTS_Q_ID[id], CMD_CONNECT, CLIENTS_Q_ID[idPartner]);
    }
    else
    {
        handleList(msg);
    }
    
}


void getMsg(int qid)
{
    struct msgbuf msg;

    if (msgrcv(qid, (void *)&msg, sizeof(msg.mtext), MSG_LOOKUP, MSG_NOERROR) == -1)
    {
        if (errno != ENOMSG)
        {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }
        printf("Brak komunikatu dostępnego dla msgrcv()\n");
    }
    else
    {
        printf("=>\n%s\n", msg.mtext);
        switch (msg.mtype)
        {
        case CMD_INIT:
            handleInit(msg);
            break;
        case CMD_LIST:
            handleList(msg);
            break;
        case CMD_STOP:
            handleStop(msg);
            break;
        case CMD_DISCONNECT:
            handleDisconnect(msg);
            break;
        case CMD_CONNECT:
            handleConnect(msg);
            break;
        default:
            break;
        }
        if (!STOPING)
            getMsg(qid);
    }
}

void fnExit()
{
    printf("Closing Server\n");
    STOPING = true;
    for (int i = 0; i < CLIENT_COUNT; i++)
        if (CLIENTS_ALIVE[i])
            sendIntMsg(CLIENTS_Q_ID[i], CMD_STOP, -1);

    for (int i = 0; i < CLIENT_COUNT; i++)
        if (CLIENTS_ALIVE[i])
            getMsg(Q_ID);

    msgctl(Q_ID, IPC_RMID, NULL);
}
void sigFn()
{
    exit(-1);
}

int main(int argc, char *argv[])
{
    if (signal(SIGINT, sigFn) == SIG_ERR) {
        fputs("An error occurred while setting a signal handler.\n", stderr);
        return EXIT_FAILURE;
    }

    int key = ftok(getenv("HOME"), getpid());    
    Q_ID = msgget(key, IPC_CREAT | 0666);
    atexit(fnExit);
    printf("Server:\n%i\n", Q_ID);
    getMsg(Q_ID);

    return 0;
}
