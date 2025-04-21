#include <stdio.h>
#include <pthread.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <string.h>
#include <ctype.h> // для isdigit()

void server(void) {
    int rcvid;
    int chid;
    char message[512];
    char response[512];

    printf("Server started.\n");

    chid = ChannelCreate(0);
    if (chid == -1) {
        perror("Failed to create channel");
    }

    printf("Channel ID: %d\n", chid);
    printf("PID: %d\n", getpid());

    while (1) {
        rcvid = MsgReceive(chid, message, sizeof(message), NULL);
        if (rcvid == -1) {
            perror("Failed to receive message");
            continue;
        }

        printf("Message received, rcvid: %X\n", rcvid);
        printf("Message: \"%s\"\n", message);

        // Распаковка строки
        int j = 0;
        for (int i = 0; message[i] != '\0'; ) {
            if (isdigit(message[i])) {
                int count = 0;
                while (isdigit(message[i])) {
                    count = count * 10 + (message[i] - '0');
                    i++;
                }

                char symbol = message[i];
                i++;

                for (int k = 0; k < count && j < sizeof(response) - 1; k++) {
                    response[j++] = symbol;
                }
            } else {
                response[j++] = message[i++];
            }
        }
        response[j] = '\0';

        // Отправка ответа
        if (MsgReply(rcvid, EOK, response, strlen(response) + 1) == -1) {
            perror("Failed to send response");
        } else {
            printf("Response sent: \"%s\"\n", response);
        }
    }

    ChannelDestroy(chid);
}

int main(void) {
    printf("Server program\n");
    server();
    printf("Sergeev Vladislav Vladimirovich i914B.\n");
    return 1;
}
