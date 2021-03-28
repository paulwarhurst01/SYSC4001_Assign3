#define MICRO_SEC_IN_SEC 1000000

long int avg_time(long int time_array[4][11], int operation){
    long int total_time = 0;
    int i = operation - 1;
    int limit = time_array[i][0];
    for(int j = 1; j <= time_array[i][0]; j++){
        total_time = total_time + time_array[i][j];
    }
    long int average_time;
    average_time =  total_time / limit;
    //printf("Average time: %d", average_time)
    return average_time;
}

int add_to_array(long int time_array[4][11], struct timeval start, struct timeval end, int operation){
    if (time_array[operation - 1][0] <= 10){
        time_array[operation - 1][0] += 1;
    }
    else{
        time_array[operation - 1][0] = 1;
    }
    long int time_taken;
    int i = operation - 1;
    int j = time_array[i][0];
    time_taken = (end.tv_sec * MICRO_SEC_IN_SEC + end.tv_usec) - (start.tv_sec * MICRO_SEC_IN_SEC + start.tv_usec);
    time_array[i][j] = time_taken;
    printf("Time taken: %d microsec\n", time_array[i][j]);
    return 1;
}