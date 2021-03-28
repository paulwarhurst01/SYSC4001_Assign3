@author: Paul Warhurst
@student_number: 101 087 869

############### Assign 3 - Message Server and User ###############

Goals of Design:
-> Message server that stores one ever increaseing string containing 
   sentences separated by periods.
-> User program should check length of string input is < 35 characters - Notify user Otherwise
-> Check senence to appended or removed ends in a period.
-> Send this length as part of message to use by functions in Text-Manager
-> Decode instruction on user side and send integer as message member
    -> Function 0 is reserved as default to end Text-Manager
    -> Append (sentence) - Function 1 - use strcat()
    -> Delete (W) - Function 2 - Delete Every word matching - Use memmove in while loop
    -> Remove (target-sentence) - Function 3 - memmove, check for period before
    -> Search (A) - Function 4 - use two while loops to find ending and starting periods

Calculating Average:
Average will be found from latest 10 runs of operation.
Create 2D array to hold 'time taken' to execute instruction.
- Time Taken defined as when user presses enter to input command to when a msg response is recieved
- 4 rows, 1 for each operation
- 11 Columns, first to hold number of columns filled, rest for time taken values
- If operation run more than 10 times, loop back and replace oldest value first.

Test Cases: 
NB: Cases were entered successively

Case I: 
    append Hello there.
Response/Resulting print out:
    Time taken: 109 microsec
    Sentence appended.
    Avg time to complete operation: 109 microsec

Case II: 
    append Please stop this testing.
Response/Resulting print out:
    Time taken: 37 microsec
    Sentence appended.
    Avg time to complete operation: 73 microsec

Case III: 
    append This sentence is way longer than the 37 character limit.
Response/Resulting print out:
    Your sentence was 56 characters long
    Please enter a sentence less than 35 char long

Case IV: 
    append no period included
Response/Resulting print out:
    Final char was 'd', please ensure final character is a period.

Case V:
    remove Hello there.
Response/Resulting print out:
    Time taken: 41 microsec
    Sentence Found and deleted.
    Avg time to complete operation: 41 microsec

