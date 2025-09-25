#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFSIZE 1024

int main() {
    int sockfd;
    char buffer[BUFSIZE];
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;

    // Criar socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY; // recebe de qualquer IP
    servaddr.sin_port = htons(PORT);

    // Associar socket à porta
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Servidor à escuta no porto %d...\n", PORT);

    while (1) {
        len = sizeof(cliaddr);
        int n = recvfrom(sockfd, buffer, BUFSIZE, 0,
                         (struct sockaddr *)&cliaddr, &len);
        buffer[n] = '\0';
        printf("Mensagem recebida: %s\n", buffer);

        // responder ao cliente
        char *msg = "Mensagem recebida!";
        sendto(sockfd, msg, strlen(msg), 0,
               (const struct sockaddr *)&cliaddr, len);
    }

    close(sockfd);
    return 0;
}
