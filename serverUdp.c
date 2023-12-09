#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_BUFFER_SIZE 1024
#define QUIT_MESSAGE "quit"

int sockid;
int server_port = 7500;
char *server_ip = "127.0.0.1";
struct sockaddr_in server_addr;
pthread_t sendThread, receiveThread;

int design_gilCedrickS();

void handleErrors() {
    perror("Error occurred. Exiting...\n");
    exit(EXIT_FAILURE);
}

void sendData(const char *msg) {
    if (sendto(sockid, msg, strlen(msg), 0, (const struct sockaddr *)&server_addr, sizeof(struct sockaddr_in)) == -1) {
        handleErrors();
    }
}

void receiveData() {
    socklen_t addr_len = sizeof(struct sockaddr_in);
    char buffer[MAX_BUFFER_SIZE];

    ssize_t recv_len = recvfrom(sockid, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_addr, &addr_len);
    if (recv_len == -1) {
        handleErrors();
    }

    buffer[recv_len] = '\0';  // Null-terminate the received message

    printf("Client response: %s\n", buffer);
}

void *sendThreadFunc(void *arg) {
    while (1) {
        // Vous pouvez demander à l'utilisateur d'entrer le message depuis la console et l'envoyer.
        // Cela peut être fait de manière non bloquante pour ne pas attendre l'entrée de l'utilisateur.

        char input[MAX_BUFFER_SIZE];
        printf("Enter a message (or type 'quit' to exit): ");
        fgets(input, sizeof(input), stdin);

        input[strcspn(input, "\n")] = '\0';
        sendData(input);

        if (strcmp(input, QUIT_MESSAGE) == 0) {
            printf("Client 'quit'. Exiting...\n");
            close(sockid);
            exit(EXIT_SUCCESS);
        }
    }
    return NULL;
}

void *receiveThreadFunc(void *arg) {
    while (1) {
        receiveData();
    }
    return NULL;
}

int main(void) {
    design_gilCedrickS();
    sockid = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockid == -1) {
        fprintf(stderr, "[-] Initialization of the socket failed\n");
        handleErrors();
    }
    printf("[+] Initialization of the socket done\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    if (bind(sockid, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("[-] Binding failed\n");
        handleErrors();
    }
    printf("[+] Binding done\n");

    if (pthread_create(&sendThread, NULL, sendThreadFunc, NULL) != 0) {
        handleErrors();
    }
    if (pthread_create(&receiveThread, NULL, receiveThreadFunc, NULL) != 0) {
        handleErrors();
    }

    // Attendre la fin des threads (ce qui ne se produira jamais dans cet exemple)
    if (pthread_join(sendThread, NULL) != 0) {
        handleErrors();
    }
    if (pthread_join(receiveThread, NULL) != 0) {
        handleErrors();
    }

    return 0;
}
 
