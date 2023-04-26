// Client-server for sending one message to each other
// Client1
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>

#define MAXPENDING 2
#define BUFFLEN 256
#define NAMELEN 30
#define PDR 10          // drop probability 10%
#define MAXENTRIES 100  // Max number of entries in file
#define PORT 8888

char* file = "name.txt";
struct sockaddr_in server_addr;
char* server_ip = "127.0.1.1";
int c_sock;
int curr_data;

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

packet snd_pkt, rcv_ack;

typedef struct data
{
    int offset;
    char name[NAMELEN];
}data;

data name_data[MAXENTRIES];

void initialize()
{
    for(int i=0; i<MAXENTRIES; i++)
    {
        memset(name_data[i].name, '\0', NAMELEN);
        name_data[i].offset = -1;
    }

    printf("[+] Name data structure initialized!\n");
}

void populate_array()
{
    FILE* f = fopen(file, "r");
    int i=0,off=0;
    name_data[0].offset = 0;


    if(!f)
        error("[-] Error opening name.txt! Exiting ...\n");
    else
    {
        while(1)
        {
            char c = fgetc(f);

            // EOF
            if(c == EOF)
            {
                printf("[+] File ended\n");
                break;
            }
            
            // Name ended
            else if(c==',' || c=='\n'|| c=='.')
            {
                name_data[i+1].offset = name_data[i].offset + off;
                off = 0;
                i++;
            }

            else
            {
                name_data[i].name[off] = c;
                off++;
            }

        }
    }

    printf("Number of entries: %d\n",i+1);
    printf("[+] Array of structures populated\n");

    fclose(f);
}

int main()
{

    // Initialize
    initialize();
    populate_array();

    // for(int i=0; i<MAXENTRIES; i++)
    // {
    //     printf("Data: %s        Offset:%d\n",name_data[i].name,name_data[i].offset);
    // }

    /* CREATE TCP socket*/
    c_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(c_sock < 0)
        error("[-] Error creating the socket!....Exiting\n");
    else
        printf("[+] Client socket successfully created!\n");
    
    /* CONNECT to a server*/
    // Populate server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    // Use "ip addr show eth0" to get the IP of the system
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    // Connect
    int temp = connect(c_sock, (struct sockaddr*) &server_addr, sizeof(server_addr));
    if(temp < 0)
        error("[-] Error connecting to server! Exiting ...\n");
    else
        printf("[+] Successfully connected to server with IP: %s\n", server_ip);


    int state = 0;
    curr_data = 0;

    while(1)
    {
        switch(state)
        {
            case 0:     // Send first
                // Populate send packet
                strcpy(snd_pkt.data, name_data[curr_data].name);
                snd_pkt.seq_no = name_data[curr_data].offset;
                snd_pkt.size = strlen(snd_pkt.data);
                snd_pkt.client_no = 1;      // since c1
                snd_pkt.TYPE = 'd';

                // Send
                if(send(c_sock, &snd_pkt, sizeof(snd_pkt), 0) < 0)
                    error("[-] Error in sending packet! Exiting ...\n");
                else
                {
                    printf("[+] SENT PKT: Seq. No. = %d, Size = %d, Client No. = %d!\n", snd_pkt.seq_no, snd_pkt.size, snd_pkt.client_no);
                    state = 1;
                    break;
                }
            case 1:     // Wait for ACK
                if(recv(c_sock,&rcv_ack,sizeof(rcv_ack),0) < 0)
                    error("[-] Error receiving ack! Exiting ...\n");

                // Seq number is same as sent offset
                // This is packet dedicated for client 2
                if(rcv_ack.client_no == 1 && rcv_ack.seq_no == name_data[curr_data].offset)
                {
                    printf("[+] Received ack seq. no. %d\n",rcv_ack.seq_no);
                    state = 2;
                    break;
                }
                break;
            case 2:     // Send first
                // Populate send packet
                strcpy(snd_pkt.data, name_data[curr_data].name);
                snd_pkt.seq_no = name_data[curr_data].offset;
                snd_pkt.size = strlen(snd_pkt.data);
                snd_pkt.client_no = 1;      // since c1
                snd_pkt.TYPE = 'd';

                // Send
                if(send(c_sock, &snd_pkt, sizeof(snd_pkt), 0) < 0)
                    error("[-] Error in sending packet! Exiting ...\n");
                else
                {
                    printf("[+] SENT PKT: Seq. No. = %d, Size = %d, Client No. = %d!\n", snd_pkt.seq_no, snd_pkt.size, snd_pkt.client_no);
                    state = 3;
                    break;
                }
            case 3:     // Wait for ACK
                if(recv(c_sock,&rcv_ack,sizeof(rcv_ack),0) < 0)
                    error("[-] Error receiving ack! Exiting ...\n");

                // Seq number is same as sent offset
                // This is packet dedicated for client 2
                if(rcv_ack.client_no == 1 && rcv_ack.seq_no == name_data[curr_data].offset)
                {
                    printf("[+] Received ack seq. no. %d\n",rcv_ack.seq_no);
                    state = 0;
                    break;
                }
                break;
        }
    }

    close(c_sock);
    return 0;
}