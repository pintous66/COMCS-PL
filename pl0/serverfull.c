#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 9999
#define BUFSIZE 1024

int main() {
    int sockfd;
    char buffer[BUFSIZE];
    struct sockaddr_storage cliaddr;
    socklen_t addr_len;
    char ipstr[INET6_ADDRSTRLEN];

    // Criar socket UDP (IPv6 permite também IPv4)
    sockfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Permitir que socket IPv6 aceite também IPv4
    int off = 0;
    if (setsockopt(sockfd, IPPROTO_IPV6, IPV6_V6ONLY, &off, sizeof(off)) < 0) {
        perror("setsockopt IPV6_V6ONLY");
        exit(EXIT_FAILURE);
    }

    // Bind na porta 9999
    struct sockaddr_in6 servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin6_family = AF_INET6;
    servaddr.sin6_port = htons(PORT);
    servaddr.sin6_addr = in6addr_any;

    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Servidor UDP à escuta na porta %d...\n", PORT);

    while (1) {
        addr_len = sizeof(cliaddr);
        int n = recvfrom(sockfd, buffer, BUFSIZE, 0,
                         (struct sockaddr *)&cliaddr, &addr_len);
        if (n < 0) {
            perror("recvfrom failed");
            continue;
        }
        buffer[n] = '\0';

        // Obter IP e porta do cliente
        void *addr;
        int port;
        if (cliaddr.ss_family == AF_INET) {
            struct sockaddr_in *s = (struct sockaddr_in *)&cliaddr;
            addr = &(s->sin_addr);
            port = ntohs(s->sin_port);
        } else { // AF_INET6
            struct sockaddr_in6 *s = (struct sockaddr_in6 *)&cliaddr;
            addr = &(s->sin6_addr);
            port = ntohs(s->sin6_port);
        }

        inet_ntop(cliaddr.ss_family, addr, ipstr, sizeof(ipstr));
        printf("Mensagem recebida de %s:%d -> %s\n", ipstr, port, buffer);

        // Espelhar string
        int len = strlen(buffer);
        for (int i = 0; i < len / 2; i++) {
            char tmp = buffer[i];
            buffer[i] = buffer[len - 1 - i];
            buffer[len - 1 - i] = tmp;
        }

        // Enviar de volta
        if (sendto(sockfd, buffer, len, 0,
                   (struct sockaddr *)&cliaddr, addr_len) < 0) {
            perror("sendto failed");
        }
    }

    close(sockfd);
    return 0;
}
