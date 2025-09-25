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

    // Criar socket UDP
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
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

        // Receber mensagem do cliente
        int n = recvfrom(sockfd, buffer, BUFSIZE, 0,
                         (struct sockaddr *)&cliaddr, &len);
        if (n < 0) {
            perror("recvfrom failed");
            continue;
        }
        buffer[n] = '\0';
        printf("Cliente [%s:%d]: %s\n", 
               inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), buffer);

        // Pedir resposta manual
        printf("Tu: ");
        if (!fgets(buffer, BUFSIZE, stdin)) {
            printf("Erro a ler do terminal.\n");
            continue;
        }
        buffer[strcspn(buffer, "\n")] = 0; // remover \n

        // Enviar resposta de volta ao cliente
        if (sendto(sockfd, buffer, strlen(buffer), 0,
                   (struct sockaddr *)&cliaddr, len) < 0) {
            perror("sendto failed");
        }
    }

    close(sockfd);
    return 0;
}
