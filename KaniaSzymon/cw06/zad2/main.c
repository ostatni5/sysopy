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
#include <mqueue.h>

mqd_t Q_ID;
char Q_NAME[30];
int CLIENT_COUNT = 0;
char CLIENTS_Q_ID[MAX_CONN];
char CLIENTS_Q_NAME[MAX_CONN][50];
bool CLIENTS_BUSY[MAX_CONN];
bool CLIENTS_ALIVE[MAX_CONN];
int ALIVE_COUNT = 0;
bool STOPING = false;

int openWriteQ(char* name)
{
    int id = -1;
    printf("%s\n",name);
    if ((id = mq_open(name, O_WRONLY)) == -1)
    {
        perror("openWriteQ mq_open");
        exit(EXIT_FAILURE);
    }
    return id;
}

void sendMsg(int qid, int type, char *text)
{
    struct msgbuf msg;
    msg.mtype = type;
    snprintf(msg.mtext, sizeof(msg.mtext), "%s", text);

    if (mq_send(qid, msg.mtext, sizeof(msg.mtext), msg.mtype) == -1)
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
    strcpy( CLIENTS_Q_NAME[id],msg.mtext);   

    CLIENTS_Q_ID[id] = openWriteQ(CLIENTS_Q_NAME[id]);

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
    if (idPartner < CLIENT_COUNT && !CLIENTS_BUSY[idPartner] && CLIENTS_ALIVE[idPartner])
    {
        CLIENTS_BUSY[id] = true;
        CLIENTS_BUSY[idPartner] = true;
        sendMsg(CLIENTS_Q_ID[idPartner], CMD_CONNECT, CLIENTS_Q_NAME[id]);
        sendMsg(CLIENTS_Q_ID[id], CMD_CONNECT, CLIENTS_Q_NAME[idPartner]);
    }
    else
    {
        handleList(msg);
    }
}

void getMsg(int qid)
{
    struct msgbuf msg;

    // if (mq_receive(qid, msg.mtext, sizeof(msg.mtext), &(msg.mtype)) == -1)
    if (mq_receive(qid, msg.mtext, sizeof(msg.mtext), &(msg.mtype)) == -1)
    {
        perror("mq_receive");
        exit(EXIT_FAILURE);
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
            {
                sendIntMsg(CLIENTS_Q_ID[i], CMD_STOP, -1);
                mq_close(CLIENTS_Q_ID[i]);
            }

    for (int i = 0; i < CLIENT_COUNT; i++)
        if (CLIENTS_ALIVE[i])
            getMsg(Q_ID);

    mq_close(Q_ID);
    mq_unlink(Q_NAME);
}
void sigFn()
{
    exit(0);
}

int main(int argc, char *argv[])
{
    if (signal(SIGINT, sigFn) == SIG_ERR)
    {
        fputs("An error occurred while setting a signal handler.\n", stderr);
        return EXIT_FAILURE;
    }
    atexit(fnExit);

    int key = ftok(getenv("HOME"), getpid());
    sprintf(Q_NAME, "/s%i", key);
    struct mq_attr attr;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_MSG_LEN;
    sprintf(Q_NAME, "/server");
    mq_unlink(Q_NAME);
    if ((Q_ID = mq_open(Q_NAME, O_RDONLY | O_CREAT | O_EXCL, 0666, &attr)) == -1)
    {
        perror("błąd mq_open");
        exit(EXIT_FAILURE);
    }

    printf("Server:\n%s\n", Q_NAME);

    if (mq_getattr(Q_ID, &attr) == -1)
        perror("mq_getattr");

    getMsg(Q_ID);

    return 0;
}
