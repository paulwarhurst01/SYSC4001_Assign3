struct database_struct{
    char string_array[1015]; // 1015 < 1024 and a multiple of 35, the max message size
    int length_array[29];   // The max number of full size strings
}