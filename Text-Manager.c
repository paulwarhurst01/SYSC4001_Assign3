#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

#include <sys/msg.h>
#include <sys/time.h>
#include <sys/types.h>

#include "message_struct.h"
#include "database_struct.h"

#define MICRO_SEC_IN_SEC 1000000

main(){
    int running = 1;
    char * str_loc;
    int rcv_msgid, snt_msgid; 
    struct message_struct rcv_msg, snt_msg;  // Received msg struct
    long int msg_to_receive = 0;
    // Initiate stored_str with a preiod as a marker to use for search and remove
    char stored_str[512] = ".";
    int stored_strlen;

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
        printf("Received message: %s\n", rcv_msg.msg_txt); 
        switch(rcv_msg.operation){
            case 0: 
                printf("Exiting text manager...\n");
                running = 0;
            case 1: 
                strcat(stored_str, rcv_msg.msg_txt);
                printf("Stored: %s\n", stored_str);
                break;
            case 2:
                while(str_loc = strstr(stored_str, rcv_msg.msg_txt)){
                    printf("Substring: %s\n", str_loc);
                    stored_strlen = strlen(stored_str);
                    memmove(str_loc, str_loc + rcv_msg.msg_size + 1, (stored_strlen - rcv_msg.msg_size) + 1);
                    printf("Stored: %s\n", stored_str);
                }
                break;
            case 3:
                str_loc = strstr(stored_str, rcv_msg.msg_txt);
                stored_strlen = strlen(stored_str);
                if((str_loc - 1)[0] == '.'){
                    // Ensures sentence is full sentence not just a sub string
                    // Otherwise would essentially be repeat of 'delete' function
                    memmove(str_loc, str_loc + rcv_msg.msg_size + 1, (stored_strlen - rcv_msg.msg_size) + 1);
                }
                printf("Stored: %s\n", stored_str);
                break;
            case 4:
                str_loc = strstr(stored_str, rcv_msg.msg_txt);
                int str_end = 0;
                // Decrement str_loc until beginning of sentence with word is found
                while(strncmp(str_loc - 1, ".", 1) != 0){
                   str_loc = str_loc - 1;
                   str_end++; // Increment the length of the string
                   printf("String length: %d\n", str_end);
                }
                printf("second loop\n");
                // Increment str_end until terminating period found
                while(strncmp((stored_str + str_end), ".", 1) != 0){
                   str_end++; // Increment the length of the string
                }
                snt_msg.my_msg_type = 1;
                strncpy(snt_msg.msg_txt, str_loc, str_end);
                printf("Word contained in sentece: %s\n", snt_msg.msg_txt);
                break;
        }
    }

    // Delete message queues, handle errors
    if (msgctl(rcv_msgid, IPC_RMID, 0) == -1){
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }
    if (msgctl(snt_msgid, IPC_RMID, 0) == -1){
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}