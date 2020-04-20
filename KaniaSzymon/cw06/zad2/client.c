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
#include <mqueue.h>
#include <sys/time.h>
#include <time.h>

mqd_t Q_ID;
char Q_NAME[30];
int SRV_Q_ID;
char SRV_Q_NAME[30];
int CLIENT_ID = -1;
int PARTNER_Q_ID = -1;
char PARTNER_Q_NAME[30];
bool CHAT_MODE = false;

int openWriteQ(char *name)
{
    int id = -1;
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
}

void sendIntMsg(int qid, int type, int a)
{
    char text[80];
    snprintf(text, sizeof(text), "%i", a);
    sendMsg(qid, type, text);
}

void handleDisconnect(struct msgbuf msg)
{
    printf("DISCONNECT\n");
    mq_close(PARTNER_Q_ID);
    sendIntMsg(SRV_Q_ID, CMD_DISCONNECT, CLIENT_ID);
    PARTNER_Q_ID = -1;
    CHAT_MODE = false;
}
void handleConnect(struct msgbuf msg)
{
    sscanf(msg.mtext, "%s", PARTNER_Q_NAME);
    PARTNER_Q_ID = openWriteQ(PARTNER_Q_NAME);
    printf("CONNECT\n");
    CHAT_MODE = true;
}

bool getMsgWait(int qid, bool wait)
{
    struct msgbuf msg;

    if (wait)
    {
        if (mq_receive(qid, msg.mtext, sizeof(msg.mtext), &(msg.mtype)) == -1)
        {
            perror("mq_receive");
            exit(EXIT_FAILURE);
        }
    }
    else
    {

        struct timespec tm;
        clock_gettime(0, &tm);
        tm.tv_nsec++;

        if (mq_timedreceive(qid, msg.mtext, sizeof(msg.mtext), &(msg.mtype), &tm) == -1)
        {
            if (errno != ETIMEDOUT)
            {
                perror("msgrcv");
                exit(EXIT_FAILURE);
            }
            else
            {
                return false;
            }
        }
    }

    printf("=>\n%s\n", msg.mtext);
    switch (msg.mtype)
    {
    case CMD_ID:
        printf("ID\n");
        CLIENT_ID = atoi(msg.mtext);
        break;
    case CMD_STOP:
        exit(0);
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

    return true;
}
void getMsg(int qid)
{
    getMsgWait(qid, true);
}

void sendInit()
{
    printf("INITIALIZING\n");
    sendMsg(SRV_Q_ID, CMD_INIT, Q_NAME);
    getMsg(Q_ID);
}
void sendList()
{
    printf("GETING LIST\n");
    sendIntMsg(SRV_Q_ID, CMD_LIST, CLIENT_ID);
    getMsg(Q_ID);
}
void sendDisconnect()
{
    getMsgWait(Q_ID, false);
    if (CHAT_MODE)
    {
        printf("DISCONNECTING\n");
        sendIntMsg(SRV_Q_ID, CMD_DISCONNECT, CLIENT_ID);
        sendIntMsg(PARTNER_Q_ID, CMD_DISCONNECT, CLIENT_ID);
        CHAT_MODE = false;
    }
}
void sendConnect(int num)
{
    printf("CONNECTING\n");
    char text[20];
    sprintf(text, "%i:%i", CLIENT_ID, num);
    sendMsg(SRV_Q_ID, CMD_CONNECT, text);
    getMsg(Q_ID);
}
void sendChat(char *text)
{
    sendMsg(PARTNER_Q_ID, CMD_CHAT, text);
    getMsg(Q_ID);
}
void fnExit()
{
    printf("Exiting \n");
    sendDisconnect();

    sendIntMsg(SRV_Q_ID, CMD_STOP, CLIENT_ID);
    mq_close(SRV_Q_ID);

    mq_close(Q_ID);
    mq_unlink(Q_NAME);
}
void sigFn()
{
    exit(0);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
        showError("Gib me server id");
    strcpy(SRV_Q_NAME, argv[1]);
    // sprintf(SRV_Q_NAME, "/server");
    SRV_Q_ID = openWriteQ(SRV_Q_NAME);

    atexit(fnExit);
    if (signal(SIGINT, sigFn) == SIG_ERR)
    {
        fputs("An error occurred while setting a signal handler.\n", stderr);
        return EXIT_FAILURE;
    }

    int key = ftok(getenv("HOME"), getpid());
    sprintf(Q_NAME, "/c%i", key);
    struct mq_attr attr;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_MSG_LEN;

    mq_unlink(Q_NAME);
    if ((Q_ID = mq_open(Q_NAME, O_RDONLY | O_CREAT | O_EXCL, 0666, &attr)) == -1)
    {
        perror("błąd mq_open");
        exit(EXIT_FAILURE);
    }

    printf("Client:\n%s\n", Q_NAME);

    sendInit();
    sendList();

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, stdin)) != -1)
    {
        getMsgWait(Q_ID, false);

        // printf("Retrieved line of length %zu:\n", read);
        line[strlen(line) - 1] = 0;
        printf("<=\n");

        if (CHAT_MODE)
        {
            if (strcmp("DIS", line) == 0 || strcmp("DISCONNECT", line) == 0)
                sendDisconnect();
            else
                sendChat(line);
        }
        else
        {
            int num;
            if (strcmp("R", line) == 0)
                ;
            else if (strcmp("LIST", line) == 0)
                sendList();
            else if (strcmp("STOP", line) == 0)
                exit(0);
            else if (sscanf(line, "CONNECT %d", &num) == 1)
                sendConnect(num);
            else
                printf("WRONG COMMAND\n");
        }
    }
    free(line);

    return 0;
}
