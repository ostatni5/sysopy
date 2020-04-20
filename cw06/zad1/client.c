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
int SRV_Q_ID;
int CLIENT_ID = -1;
int PARTNER_Q_ID = -1;
bool CHAT_MODE = false;

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
    //printf("%i : %s\n", qid, msg.mtext);
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
    sendIntMsg(SRV_Q_ID, CMD_DISCONNECT, CLIENT_ID);
    PARTNER_Q_ID = -1;
    CHAT_MODE = false;
}
void handleConnect(struct msgbuf msg)
{
    sscanf(msg.mtext, "%i", &PARTNER_Q_ID);
    printf("CONNECT\n");
    CHAT_MODE = true;
}

bool getMsgWait(int qid, bool wait)
{
    int flags = MSG_NOERROR;
    if (!wait)
        flags |= IPC_NOWAIT;

    struct msgbuf msg;

    if (msgrcv(qid, (void *)&msg, sizeof(msg.mtext), MSG_LOOKUP, flags) == -1)
    {
        if (errno != ENOMSG)
        {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }
        else
        {
            return false;
        }
    }
    else
    {
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
    sendIntMsg(SRV_Q_ID, CMD_INIT, Q_ID);
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
    msgctl(Q_ID, IPC_RMID, NULL);
}
void sigFn()
{
    exit(-1);
}
int main(int argc, char *argv[])
{
    if (argc < 2)
        showError("Gib me server id");
    SRV_Q_ID = atoi(argv[1]);

    if (signal(SIGINT, sigFn) == SIG_ERR) {
        fputs("An error occurred while setting a signal handler.\n", stderr);
        return EXIT_FAILURE;
    }

    int key = ftok(getenv("HOME"), getpid());
    Q_ID = msgget(key, IPC_CREAT | 0666);
    atexit(fnExit);
    printf("Client:\n%i\n", Q_ID);

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
