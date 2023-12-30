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

    msgid = msgget(MSG_KEY, 0666 | IPC_CREAT);

    if (msgid == -1) {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < NUM_MESSAGES; i++) {
        message.msg_type = 1;
        sprintf(message.msg_text, "Message %d", i + 1);

        if (msgsnd(msgid, &message, sizeof(message), 0) == -1) {
            perror("msgsnd failed");
            exit(EXIT_FAILURE);
        }
    }

    printf("Sent %d messages\n", NUM_MESSAGES);
    return 0;
}
