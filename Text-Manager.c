#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/msg.h>
#include <sys/time.h>
#include <sys/types.h>

#include "message_struct.h" // Shared message data struct

void main(){
    int running = 1;
    // Initiate stored_str with a preiod as a marker to use for search and remove
    char stored_str[512] = ".";
    char * str_loc;                          // used by strstr to locate substring
    int rcv_msgid, snt_msgid;
    struct message_struct rcv_msg, snt_msg;  // Received msg struct
    long int msg_to_receive = 0;
    int stored_strlen, deleted;                      // Length of stored string

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
                break;
            case 1: 
                // Append a sentence
                strcat(stored_str, rcv_msg.msg_txt);
                printf("Stored: %s\n", stored_str);
                strcpy(snt_msg.msg_txt, "Sentence appended.");
                snt_msg.operation = 1;
                break;
            case 2:
                // Delete a word every time it appears
                deleted = 0;
                stored_strlen = strlen(stored_str);
                while(str_loc = strstr(stored_str, rcv_msg.msg_txt)){
                    printf("Substring: %s\n", str_loc);
                    stored_strlen = strlen(stored_str);
                    memmove(str_loc, str_loc + rcv_msg.msg_size, (stored_strlen - rcv_msg.msg_size));
                    printf("Stored: %s\n", stored_str);
                    deleted++;
                }
                char temp[35];
                sprintf(temp, "Word deleted %d times.", deleted);
                strcpy(snt_msg.msg_txt, temp);
                snt_msg.operation = 2;
                break;
            case 3:
                // Remove a message
                if( (str_loc = strstr(stored_str, rcv_msg.msg_txt)) == NULL){
                    // Notify user sentence does not exist
                    strcpy(snt_msg.msg_txt, "Sentence not found.");
                    break;
                }
                stored_strlen = strlen(stored_str);
                if((str_loc - 1)[0] == '.'){
                    // Ensures sentence is full sentence not just a sub string
                    // Otherwise would essentially be repeat of 'delete' function
                    memmove(str_loc, str_loc + rcv_msg.msg_size, (stored_strlen - rcv_msg.msg_size));
                    strcpy(snt_msg.msg_txt, "Sentence Found and deleted.");
                }
                else {
                    strcpy(snt_msg.msg_txt, "Sentence not found.");
                }
                printf("Stored: %s\n", stored_str);
                snt_msg.operation = 3;
                break;
            case 4:
                // search for word and return sentence with word
                if( (str_loc = strstr(stored_str, rcv_msg.msg_txt)) == NULL){
                    strcpy(snt_msg.msg_txt, "String not found.");
                    break;
                }
                else {
                    int str_end = 0, done1 = 0, done2 = 0;

                    // Decrement str_loc until beginning of sentence with word is found
                    while(strncmp(str_loc - 1, ".", 1) != 0 ){
                        str_loc--;
                    }

                    // Increment str_end until terminating period found
                    while(strncmp(str_loc + str_end, ".", 1) != 0){
                        str_end++; // Increment the length of the string
                    }

                    // Copy found sentence in msg to send
                    memset(snt_msg.msg_txt, '\0', 35); // Reset sending message
                    strncpy(snt_msg.msg_txt, str_loc, str_end + 1);
                }
                snt_msg.operation = 4;
                break;
            default:
                break;
        }
        // Notify User of changes
        snt_msg.my_msg_type = 1;
            if (msgsnd(snt_msgid, (void *)&snt_msg, 35, 0) == -1){
                    fprintf(stderr, "msgsnd failed\n");
                    exit(EXIT_FAILURE);
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