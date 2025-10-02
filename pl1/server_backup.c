#include <stdio.h>
#include <string.h> // bzero() 
#include <sys/socket.h> 
#include <arpa/inet.h> // inet_addr() 
#include <unistd.h> // read(), write(), close(), fork()
#include <sys/wait.h> // wait() 
#include <signal.h> // signal handling

void handle_client(int client_sock, struct sockaddr_in client_addr) {
    char server_message[2000], client_message[2000];
    
    // Clean buffers: 
    bzero(server_message, sizeof(server_message)); 
    bzero(client_message, sizeof(client_message)); 
    
    printf("[PID %d] Client connected at IP: %s and port: %i\n", getpid(),
           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port)); 
    
    // Receive client's message: 
    if( read(client_sock, client_message, sizeof(client_message)) < 0){ 
        printf("[PID %d] Can't read\n", getpid()); 
        close(client_sock);
        return; 
    } 
    
    printf("[PID %d] Msg from client: %s\n", getpid(), client_message); 
    
    // Respond to client: 
    strcpy(server_message, "This is the server's message."); 
    
    printf("[PID %d] Processing message from client...\n", getpid()); 
    sleep(10); //this will hold the server to reply the client's message for 10 seconds 
    
    if( write(client_sock, server_message, sizeof(server_message)) < 0){ 
        printf("[PID %d] Can't write\n", getpid()); 
        close(client_sock);
        return; 
    } 
    
    printf("[PID %d] Response sent to client\n", getpid());
    
    // Closing the client socket: 
    close(client_sock);
}

void sigchld_handler(int sig) {
    // Clean up zombie processes
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

int main() 
{ 
    int sockfd, client_sock; 
    struct sockaddr_in server_addr, client_addr; 
    pid_t pid; 
    
    // Set up signal handler for cleaning up child processes
    signal(SIGCHLD, sigchld_handler);
    
 // Create socket: 
 sockfd = socket(AF_INET, SOCK_STREAM, 0); 
 
 if(sockfd < 0){ 
 printf("Error while creating socket\n"); 
 return -1; 
 } 
 printf("Socket created successfully\n"); 
 
 // Set port and IP: 
 server_addr.sin_family = AF_INET; 
 server_addr.sin_port = htons(2000); 
 server_addr.sin_addr.s_addr = inet_addr("0.0.0.0"); 
 
 // Bind to the set port and IP: 
 if(bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr))<0){ 
 printf("Couldn't bind to the port\n"); 
close(sockfd);
 return -1; 
 } 
 printf("Done with binding\n"); 
 
 // Listen for clients: 
 if(listen(sockfd, 1) < 0){ 
 printf("Error while listening\n"); 
 close(sockfd); 
 return -1; 
 } 
 printf("\nListening for incoming connections.....\n"); 
 
 // Accept an incoming connection: 
 socklen_t slen=sizeof(client_addr); 
 client_sock = accept(sockfd, (struct sockaddr*)&client_addr, &slen); 
 
 if (client_sock < 0){ 
 printf("Can't accept\n"); 
 close(sockfd); 
 return -1; 
 } 
 printf("Client connected at IP: %s and port: %i\n", inet_ntoa(client_addr.sin_addr), 
ntohs(client_addr.sin_port)); 
 
 // Receive client's message: 
 if( read(client_sock, client_message, sizeof(client_message)) < 0){ 
 printf("Can't read\n"); 
 return -1; 
 } 
 
 printf("Msg from client: %s\n", client_message); 
 
 // Respond to client: 
 strcpy(server_message, "This is the server's message."); 
 
 printf("Processing message from client...\n"); 
 sleep(10); //this will hold the server to reply the client’s message for 10 seconds 
 
 if( write(client_sock, server_message, sizeof(server_message)) < 0){ 
 printf("Can't write\n"); 
 return -1; 
 } 
 
 
 // Closing the socket: 
 close(client_sock); 
 close(sockfd); 
 
 return 0; 
} 
