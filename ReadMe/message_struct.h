struct message_struct {
    long int my_msg_type;
    int operation;
    int msg_size; // number of characters in the msg
    char msg_txt[BUFSIZ];
};