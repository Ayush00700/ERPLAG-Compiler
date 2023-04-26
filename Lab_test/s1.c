// Client-server for sending one message to each other
// Server
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>

#define MAXPENDING 2
#define BUFFLEN 256
#define PORT 8888
#define PDR 10          // drop probability 10%
#define TIMEOUT 2       // timeout value

// Define function for error display
void error(char* msg)
{
    perror(msg);
    exit(1);
}

typedef struct packet
{
    char data[BUFFLEN];     // payload
    int size;               // payload size
    int seq_no;             // byte sequence number
    int client_no;          // to differentiate c1 and c2
    char TYPE;              // 'd' = data 'a' = ack
}packet;

int drop_at_random()
{
    // Generate random number
    srand(time(NULL));
    int r = rand()%PDR;
    int flag = (r > 0)? 0 : 1;

    return flag;
}

packet snd_ack, rcv_data;
struct sockaddr_in server_addr, client_addr1, client_addr2;
int s_sock, bind_s, listen_s, c1_sock, c2_sock;

void initialize()
{
    strcpy(snd_ack.data, "NULL");
    snd_ack.seq_no = 0;
    snd_ack.size = 0;
    snd_ack.TYPE = 'a';
    
    strcpy(rcv_data.data, "NULL");
    rcv_data.seq_no = 0;
    rcv_data.size = 0;
    rcv_data.TYPE = 'd';
}

int main()
{
    /* Create TCP Socket*/
    s_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(s_sock < 0)
        error("[-] Error in creating server socket! Exiting ...\n");
    else
        printf("[+] Server socket created!\n");

    /* BIND the soket to an address*/
    // Create local address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    printf("Server address: %s\n",inet_ntoa(server_addr.sin_addr));

    // Bind the socket to the address structure
    bind_s = bind(s_sock, (struct sockaddr*) &server_addr, sizeof(server_addr));
    if(bind_s < 0)
        error("[-] Error in binding socket! Exiting ...\n");
    else
        printf("[+] Server socket binding successful!\n");
    
    /* LISTEN for possible connections*/
    listen_s = listen(s_sock, MAXPENDING);
    if(listen_s < 0)
        error("[-] Error in socket listen! Exiting ...\n");
    else
        printf("[+] Server socket listening!\n");
    
    /* ACCEPT 2 TCP connections for c1 and c2*/
    // For c1
    int client_len1 = sizeof(client_addr1);
    c1_sock = accept(s_sock, (struct sockaddr*) &client_addr1, &client_len1);
    if(c1_sock < 0)
        error("[-] Error in accepting client c1! Exiting ...\n");
    else
    {
        printf("[+] Connection established with c1:\n");
        printf("[+] IP address: %s Port: %d", inet_ntoa(client_addr1.sin_addr), ntohs(client_addr1.sin_port));
    }

    // For c2
    int client_len2 = sizeof(client_addr2);
    c2_sock = accept(s_sock, (struct sockaddr*) &client_addr2, &client_len2);
    if(c2_sock < 0)
        error("[-] Error in accepting client c2! Exiting ...\n");
    else
    {
        printf("[+] Connection established with c2:\n");
        printf("[+] IP address: %s Port: %d", inet_ntoa(client_addr2.sin_addr), ntohs(client_addr2.sin_port));
    }


    /* CLOSE the connections*/
    close(s_sock);
    close(c1_sock);
    close(c2_sock);
    return 0;
}
