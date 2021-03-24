#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/msg.h>

#include "message_struct.h"
#include "database_struct.h"

main(){
    int running = 1;
    int rcv_msgid, snt_msgid; 
    struct message_struct rcv_msg, snt_msg;  // Received msg struct
    long int msg_to_receive = 0;

    // Create rcv message queue for receiving data from User, snt_msgid for User
    rcv_msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    // Handle errors
    if (rcv_msgid == -1) {
        fprintf(stderr, "msgget failed with errror: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    // Create snt message queue for sending data to User, rcv_msgid for User
    snt_msgid = msgget((key_t)1235, 0666 | IPC_CREAT);
    // Handle errors
    if (snt_msgid == -1) {
        fprintf(stderr, "msgget failed with errror: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    while(running){
        // receive message into rcv_msg, handle errors
        if(msgrcv(rcv_msgid, (void *)&rcv_msg, BUFSIZ, 
                    msg_to_receive, 0) == -1) {
                        fprintf(stderr, "msgrcv failed with error %d\n", errno);
                        exit(EXIT_FAILURE);
        }
        printf("Received message: %s", rcv_msg.msg_txt); 
        if (strncmp(rcv_msg.msg_txt, "end", 3) == 0){
            running = 0;
        }
    }

    // Delete message queue, handle errors
    if (msgctl(msgid, IPC_RMID, 0) == -1){
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}