#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 8080
#define BUFSIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <IP ou hostname do servidor>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *server_name = argv[1];
    int sockfd;
    char buffer[BUFSIZE];
    struct sockaddr_in servaddr;
    socklen_t addr_len = sizeof(servaddr);

    // Criar socket UDP
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);

    // Resolver DNS ou IP
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_DGRAM;  // UDP

    if (getaddrinfo(server_name, NULL, &hints, &res) != 0) {
        perror("Erro ao resolver o hostname");
        exit(EXIT_FAILURE);
    }

    memcpy(&servaddr.sin_addr, &((struct sockaddr_in*)res->ai_addr)->sin_addr,
           sizeof(struct in_addr));
    freeaddrinfo(res);

    printf("Ligado a %s:%d\n", server_name, PORT);

    // Loop de envio/receção de mensagens
    while (1) {
        printf("Tu: ");
        if (!fgets(buffer, BUFSIZE, stdin)) break; // ler do terminal
        buffer[strcspn(buffer, "\n")] = 0;          // remover \n

        if (strcmp(buffer, "exit") == 0) break;     // sair do cliente

        // enviar mensagem
        if (sendto(sockfd, buffer, strlen(buffer), 0,
                   (struct sockaddr *)&servaddr, addr_len) < 0) {
            perror("sendto failed");
            continue;
        }

        // receber resposta
        int n = recvfrom(sockfd, buffer, BUFSIZE, 0, NULL, NULL);
        if (n < 0) {
            perror("recvfrom failed");
            continue;
        }
        buffer[n] = '\0';
        printf("Servidor: %s\n", buffer);
    }

    close(sockfd);
    printf("Cliente terminado.\n");
    return 0;
}
