//
// Created by Ostatni5 on 16.04.2020.
//

#ifndef ZAD1_CODES_H
#define ZAD1_CODES_H

#define MAX_MSG_LEN 2048
#define MSG_LOOKUP -160
#define MAX_CONN 100
#define CMD_OFFSET 50
#define CMD_STOP 1
#define CMD_DISCONNECT  2
#define CMD_LIST  3
#define CMD_CONNECT  4
#define CMD_INIT  5
#define CMD_END  6
#define CMD_ID  7
#define CMD_CHAT 8

#include <stdlib.h>
struct msgbuf {
    unsigned int mtype;
    char mtext[MAX_MSG_LEN];
};

void showError(char *msg) {
    printf("---ERROR:  ");
    printf("%s", msg);
    printf("\n");
    exit(-1);
}

#endif //ZAD1_CODES_H
