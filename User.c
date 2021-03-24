#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/msg.h>

#include "message_struct.h"

void main(){
    int running = 1, ready = 0, stop_running = 0;
    int rcv_msgid, snt_msgid, msg_size; 
    struct message_struct rcv_msg, snt_msg;  // Received msg struct
    long int msg_to_receive = 0;
    char buffer[BUFSIZ]; 

    // Create snt message queue for receiving data from User, rcv_msgid for Text-Manager
    snt_msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    // Handle errors
    if (snt_msgid == -1) {
        fprintf(stderr, "msgget failed with errror: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    // Create rcv message queue for sending data to User, snt_msgid for Text-Manager
    rcv_msgid = msgget((key_t)1235, 0666 | IPC_CREAT);
    // Handle errors
    if (rcv_msgid == -1) {
        fprintf(stderr, "msgget failed with errror: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    char subbuff[5];
    memcpy(subbuff, &buffer[6], 4); // In theory subbuff[4] = '\0'

    while(running){
        printf("Enter the command you wish to perform: ");
        fgets(buffer, BUFSIZ, stdin); 
        /* If User of User enters "Append" */
        if(strncmp(buffer, "Append", 6) == 0) {
            printf("Enter the sentence you want to append: ");
            fgets(buffer, BUFSIZ, stdin);
            msg_size = strlen(buffer) - 1;      // -1 to remove '/0'
            if( msg_size > 35 ){
                printf("Your sentence was %d characters long\n", msg_size);
                printf("Please enter a sentence less than 35 char long\n");
            }
            // If user left full stop off
            else if (buffer[msg_size - 1] != '.'){
                printf("Please try again, your sentence must end in period.\n");
            }
            else {
                snt_msg.msg_size = msg_size;
                snt_msg.operation = 1;
                strcpy(snt_msg.msg_txt, buffer);
                ready = 1;
            }
        }

        /* If User of User enters "Delete" */
        if(strncmp(buffer, "Delete", 6) == 0) {
            printf("Enter the single word you would like deleted: ");
            fgets(buffer, BUFSIZ, stdin);
            msg_size = strlen(buffer) - 1;
            if( msg_size > 35 ){
                printf("Your sentence was %d characters long\n", msg_size);
                printf("Please enter a sentence less than 35 char long\n");
            }
            else if (strchr(buffer, ' ') != 0) {
                printf("Please ensure there are no spaces included.\n");
            }
            else {
                snt_msg.msg_size = msg_size;
                snt_msg.operation = 2;
                strcpy(snt_msg.msg_txt, buffer);
                ready = 1;
            }         
        }

        /* If User of User enters 'end' */
        if (strncmp(buffer, "end", 3) == 0){
            snt_msg.operation = 0;
            ready = 1;
            stop_running = 1;
        }



        else {
            printf("Please enter a compatable keyword\n");
        }

        if(ready){
            if (msgsnd(snt_msgid, (void *)&snt_msg, 35, 0) == -1){
                fprintf(stderr, "msgsnd failed\n");
                exit(EXIT_FAILURE);
            }
        }

        if (stop_running) running = 0;
    }

    exit(EXIT_SUCCESS);
}