#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_SIZE 100
#define MSG_KEY 1234
#define NUM_MESSAGES 10

struct msg_buffer {
    long msg_type;
    char msg_text[MSG_SIZE];
};

int main() {
    struct msg_buffer message;
    int msgid;

    msgid = msgget(MSG_KEY, 0666);
    if (msgid == -1) {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < NUM_MESSAGES; i++) {
        if (msgrcv(msgid, &message, sizeof(message), 0, 0) == -1) {
            perror("msgrcv failed");
            exit(EXIT_FAILURE);
        }
        printf("Received: %s\n", message.msg_text);
    }

    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("failed to remove message");
        exit(EXIT_FAILURE);
    }

    return 0;
}
