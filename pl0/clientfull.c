#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 9999
#define BUFSIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <IP ou DNS do servidor>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *server_name = argv[1];
    int sockfd;
    char buffer[BUFSIZE];
    struct sockaddr_storage servaddr;
    socklen_t addr_len;

    // Resolver endereço (IPv4 ou IPv6) usando DNS
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;      // IPv4 ou IPv6
    hints.ai_socktype = SOCK_DGRAM;   // UDP

    if (getaddrinfo(server_name, "9999", &hints, &res) != 0) {
        perror("Erro ao resolver o hostname");
        exit(EXIT_FAILURE);
    }

    // Criar socket
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd < 0) {
        perror("socket failed");
        freeaddrinfo(res);
        exit(EXIT_FAILURE);
    }

    memcpy(&servaddr, res->ai_addr, res->ai_addrlen);
    addr_len = res->ai_addrlen;
    freeaddrinfo(res);

    printf("Ligado a %s na porta %d\n", server_name, PORT);

    // Loop de envio/receção
    while (1) {
        printf("Tu: ");
        if (!fgets(buffer, BUFSIZE, stdin)) break;
        buffer[strcspn(buffer, "\n")] = 0;

        if (strcmp(buffer, "exit") == 0) break;

        if (sendto(sockfd, buffer, strlen(buffer), 0,
                   (struct sockaddr *)&servaddr, addr_len) < 0) {
            perror("sendto failed");
            continue;
        }

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
