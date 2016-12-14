#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "chat.h"
#include <pthread.h>

#define MAX_CLI_COUNT   64

static int clients[MAX_CLI_COUNT];
static int server;
static chat_msg_t globalbuf[MAX_CLI_COUNT];

int __init_sever(int port);
int new_client();
void * resv_msg(void * id);
void * send_msg(void * id);

int main(int argc, char ** argv)
{
    int sock, port, cli_count = 0;
    pthread_t thread_r[MAX_CLI_COUNT],thread_w[MAX_CLI_COUNT];

    if (argc < 2)
    {
        fprintf(stderr,"usage: %s <port_number>\n", argv[0]);
        return EXIT_FAILURE;
    }

    port = atoi(argv[1]);
    __init_sever(port);

    while (cli_count <= MAX_CLI_COUNT)
    {
        clients[cli_count] = new_client();
        pthread_t * pthr_read = (pthread_t *) &thread_r[cli_count];
        pthread_t * pthr_wrie = (pthread_t *) &thread_w[cli_count];

        if (pthread_create(pthr_read, NULL, resv_msg, (void*)cli_count) != 0)
        {
            perror("Creating the thread");
            return EXIT_FAILURE;
        }
        //закрываем мьютекс на чтение из очереди
        if (pthread_create(pthr_wrie, NULL, send_msg, (void*)cli_count) != 0)
        {
            perror("Creating the thread");
            return EXIT_FAILURE;
        }
        cli_count++;
    }
    int i;
    for (i=0; i<cli_count; i++)
    {
        if (pthread_join(*&thread_r[i], NULL) != 0) {
            perror("Joining the thread");
            return EXIT_FAILURE;
        }
        if (pthread_join(*&thread_w[i], NULL) != 0) {
            perror("Joining the thread");
            return EXIT_FAILURE;
        }
    }

    close(server);
}

int new_client()
{
    int             socket;
    sockaddr_in_t   cli_addr;
    socklen_t       clen;

    clen = (socklen_t) sizeof(cli_addr);
    socket = accept(server, (struct sockaddr *) &cli_addr, &clen);

    if (socket < 0)
    {
        printf("accept() failed: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    return socket;
}


void * resv_msg(void * id)
{
    int _id = *(int *) id, bytes, i;
    chat_msg_t msg;
    do {
        bytes = read(clients[_id], &msg, MSG_STRUCT_SIZE-1);
        for (i =0; i< MAX_CLI_COUNT; i++)
        {
            if ( i != _id)
            {
                //здесь складыаем в очередь, то что нужно переслать всем клиентам
            }
        }
        //обработчик завершения потока
    }while(1);
}

void * send_msg(void * id)
{
    chat_msg_t msg;
    do {
        //тут нужен cond и чтение из очереди. записываем в сокет при получении нового сообщения
        // write(sock, buf, strlen(buf));
        //обработчик завершения потока
    }while(1);
}

int __init_sever(int port)
{
    sockaddr_in_t serv_addr;

    server = socket(AF_INET, SOCK_STREAM, 0);
    if (socket < 0)
    {
        printf("socket() failed: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (bind(server, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("bind() failed: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    listen(server, 1);
}
