@author: Paul Warhurst
@student_number: 101 087 869

############### Assign 3 - Message Server and User ###############

Goals of Design:
-> Message server that stores one ever increaseing string containing 
   sentences separated by periods.
-> User should check length of string input is < 35 characters
-> send this length as part of message to avoid repitition
-> Store this length of string in an integer array
-> Limit this array to contain 100 sentences
-> If more than 100 strings, create a new array
-> Storing string length allows indexing of string using string functions
    - Possible to skip sentences if initial few characters don't match
-> Storing string legnth to decrease search time. 
    - Skip strings with differenct numbers of character for search/delete
-> Decode instruction on user side and send integer as message member
-> Append (sentence) - Function 0
-> Delete (W) - Function 1 - Delete Every word matching - Use string.erase
-> Remove (target-sentence) - Function 2 
-> Search (A) - Function 3 
-> Achieve all string operations using <string> library
-> Track time to complete operation -> send initial time from user
    - Use this to calculate time to complete
-> Store times for each operation in an array, use this to calculate 
   average time
    - Array limited to 100 operations before overwriting first (the earliest) 
      entries
-> Print average time