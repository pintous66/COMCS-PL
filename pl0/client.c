#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFSIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <IP do servidor>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *server_ip = argv[1];
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

    if (inet_pton(AF_INET, server_ip, &servaddr.sin_addr) <= 0) {
        fprintf(stderr, "IP inválido: %s\n", server_ip);
        exit(EXIT_FAILURE);
    }

    printf("Ligado a %s:%d\n", server_ip, PORT);

    // Loop de envio de mensagens
    while (1) {
        printf("Tu: ");
        if (!fgets(buffer, BUFSIZE, stdin)) break; // ler do terminal
        buffer[strcspn(buffer, "\n")] = 0; // remover \n

        if (strcmp(buffer, "exit") == 0) break; // sair do cliente

        // enviar mensagem
        if (sendto(sockfd, buffer, strlen(buffer), 0,
                   (struct sockaddr *)&servaddr, addr_len) < 0) {
            perror("sendto failed");
            continue;
        }

        // receber resposta
        int n = recvfrom(sockfd, buffer, BUFSIZE, 0,
                         NULL, NULL);
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
