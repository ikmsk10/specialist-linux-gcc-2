#ifndef _CHAT_H
#define _CHAT_H

#define NAME_LEN        128
#define MSG_SIZE        256
#define MSG_STRUCT_SIZE (MSG_SIZE)+(NAME_LEN)+sizeof(unsigned long)+ sizeof(unsigned short)

struct __chat_msg {
    unsigned long   id_msg;
    char            nickname[NAME_LEN];
    unsigned short  cli_uid;
    char            msg[MSG_SIZE];
};

typedef struct sockaddr_in sockaddr_in_t;
typedef struct __chat_msg chat_msg_t;

#endif /*_CHAT_H */
