#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/msg.h>
#include <sys/time.h>
#include <sys/types.h>

#include "message_struct.h"

#define MICRO_SEC_IN_SEC 1000000

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

   // Operation Instructions 
    printf("Enter the command you wish to perform, followed by a space and then the associated string.\n");
    printf("Possible commands: \n");
    printf("\tappend - Append sentence passed, sentence must end in a period '.'\n");
    printf("\tdelete - Delete every occurence of word passes\n");
    printf("\tremove - Remove first occurence of sentence passed\n");
    printf("\tsearch - returns first sentence containing word\n");
    printf("Example input: append Hello there.\n");

    while(running){
        printf("\nEnter command and parameter: ");
        fgets(buffer, BUFSIZ, stdin); 
        /* If User of User enters "Append" */
        msg_size = strlen(buffer) - 7 - 1;      // -7 to remove command
        if( msg_size > 34 ){
                printf("Your sentence was %d characters long\n", msg_size); // 7 characters reserved for command
                printf("Please enter a sentence less than 35 char long\n");
            }

        else if(strncmp(buffer, "append", 6) == 0) {
            // If user left full stop off
            if (buffer[msg_size + 6]!= '.'){
                printf("Final char was '%c', please ensure final character is a period.", buffer[msg_size + 6]);
            }
            else {
                snt_msg.operation = 1;
                ready = 1;
            }
        }

        /* If User of User enters "Delete" */
        else if(strncmp(buffer, "delete", 6) == 0) {
            if (strchr(buffer + 7, ' ') != 0) {
                printf("Please ensure there are no spaces included.\n");
            }
            // Check no spaces included
            else {
                snt_msg.operation = 2;
                ready = 1;
            }         
        }

        else if(strncmp(buffer, "remove", 6) == 0) {
            if (buffer[msg_size + 8]!= '.'){
                printf("Final char was '%c', please ensure final character is a period.", buffer[msg_size + 5]);
            }
            // Check no spaces included
            else {
                snt_msg.operation = 3;
                ready = 1;
            }         
        }

        else if(strncmp(buffer, "search", 6) == 0) {
            // Check no spaces included
            if (strchr(buffer + 7, ' ') != 0) {
                printf("Please ensure there are no spaces included.\n");
            }
            else {
                snt_msg.operation = 4;
                ready = 1;
            }
            printf("Found in stored sentence: ");        
        }

        /* If User of User enters 'end' */
        else if (strncmp(buffer, "end", 3) == 0){
            snt_msg.operation = 0;
            ready = 1;
            stop_running = 1;
        }

        else {
            printf("Please enter a compatable keyword\n");
        }

        if(ready){
            // Prepare message to send
            snt_msg.my_msg_type = 1;
            snt_msg.msg_size = msg_size;
            buffer[msg_size + 7] = '\0';    // Remove "new line" operator, + 6 to include command still in buffer
            strcpy(snt_msg.msg_txt, buffer + 7);
            //printf("Message to send with size %d: %s\n", snt_msg.msg_size, snt_msg.msg_txt);
            if (msgsnd(snt_msgid, (void *)&snt_msg, 35, 0) == -1){
                fprintf(stderr, "msgsnd failed\n");
                exit(EXIT_FAILURE);
            }
            if(msgrcv(rcv_msgid, (void *)&rcv_msg, BUFSIZ, 
                msg_to_receive, 0) == -1) {
                    fprintf(stderr, "msgrcv failed with error %d\n", errno);
                    exit(EXIT_FAILURE);
            }
            printf("%s\n", rcv_msg.msg_txt);
            ready = 0;
        }

        if (stop_running) running = 0;
    }

    exit(EXIT_SUCCESS);
}