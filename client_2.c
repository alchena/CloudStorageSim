// ICS53 - Assignment 5 Exercises
// #include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
// #include <sys/socket.h>
#include <unistd.h>
// #define PORT 9999

// // Sending and receiving a simple message.
// int client_example_1(client_socket){
//     ssize_t sent_size;
//     char buffer[1024];  // Receiving the message into the buffer.
//     int received_size;
//     char message[] = "Hello from client";  // Message to send (17-characters/bytes).

//     // Since client_socket is a File Descriptor, both "write" and "send" should work.
//     // However, "send" is dedicated for sockets which gives you a fourth arguments to set some socket options.
// //    sent_size = write(client_socket, message, strlen(message));
//     sent_size = send(client_socket, message, strlen(message), 0);
//     printf("Client: message sent with size %d bytes. Original message size = %d bytes.\n", sent_size, strlen(message));

//     // Since client_socket is a File Descriptor, both "read" and "recv" should work.
//     // However, "recv" is dedicated for sockets which gives you a fourth arguments to set some socket options.
// //    received_size = read(client_socket, buffer, 1024);
//     received_size = recv(client_socket, buffer, 1024, 0);
//     printf("Client received '%s' with size = %i\n", buffer, received_size);

//     // Make sure to close the socket after you close the application.
//     close(client_socket);
//     return 0;
// }

// // Sending and receiving multiple messages message.
// int client_example_2(client_socket){

//     char message1[] = "aaa";
//     send(client_socket, message1, strlen(message1), 0);
//     sleep(1);

//     char message2[] = "bbbb";
//     send(client_socket, message2, strlen(message2), 0);
//     sleep(1);

//     char message3[] = "ccccc";
//     send(client_socket, message3, strlen(message3), 0);

//     // Make sure to close the socket after you close the application.
//     close(client_socket);
//     return 0;
// }

// // Sending and receiving an entire file.
// int client_example_3(client_socket){
//     int received_size;
//     char destination_path[] = "/Local Directory/received_file.txt";  // Note how we don't have the original file name.
//     int chunk_size = 1000;
//     char file_chunk[chunk_size];
// //    int chunk_counter = 0;

//     FILE *fptr;

//     // Opening a new file in write-binary mode to write the received file bytes into the disk using fptr.
//     fptr = fopen(destination_path,"wb");

//     // Keep receiving bytes until we receive the whole file.
//     while (1){
//         bzero(file_chunk, chunk_size);
// //        memset(&file_chunk, 0, chunk_size);

//         // Receiving bytes from the socket.
//         received_size = recv(client_socket, file_chunk, chunk_size, 0);
//         printf("Client: received %i bytes from server.\n", received_size);

//         // The server has closed the connection.
//         // Note: the server will only close the connection when the application terminates.
//         if (received_size == 0){
//             close(client_socket);
//             fclose(fptr);
//             break;
//         }
//         // Writing the received bytes into disk.
//         fwrite(&file_chunk, sizeof(char), received_size, fptr);
// //        printf("Client: file_chunk data is:\n%s\n\n", file_chunk);
//     }
// }


// client_example_4:
// sending and receiving multiple files along with their file names.
// You have to do this yourself.
// Hint: you may want to attach a header to each file you send.

//Helper Functions======================================================================================
void tokenize(char* input, char** args){
    const char* delim = " \n\r\t";
    char* command = strtok(input, delim);
    int i = 0;
    while(command != NULL){
        args[i] = command;
        ++i;
        command = strtok(NULL, delim);
    }
}

void getFirst(char* input, char* first){
    int i = 0;
    while(input[i] != ' ' && input[i] != '\n'){
        first[i] = input[i];
        ++i;
    }
    first[i] = '\0';
}

void appendLoop(FILE* fp){
    char input[500];
    char* args[10];
    char first_word[500];
    //copy fp stream content to input
    while (fgets(input, sizeof(input), fp)){
        printf("Appending> %s", input);
        getFirst(input, first_word);
        if(strcmp(first_word, "close") == 0){
            break;
        }else if(strcmp(first_word, "pause") == 0){
            tokenize(input, args);
            sleep(atoi(args[1]));
            
        }else{
            
        }
    }
}
void execute(char** args, FILE* fp){
    if(strcmp(args[0], "pause") == 0){
        sleep(atoi(args[1]));
    }
    else if(strcmp(args[0], "append") == 0){
        //check if file exists on remote directory 
        char* filename = args[1];
        
        //append goes into a while loop that breaks iff close is called
        appendLoop(fp);
        
    } else {
        printf("Command [%s] is not recognized.\n", args[0]);
    }
}

int main(int argc, char **argv)
{
    //connecting to server --> clientfd
    // int clientfd = connect_client(argv[2]);
    //Opening Command Line File
    char input[100];
    char* args[10];
    //read input file
    FILE* fp = fopen(argv[1], "r");
    if(fp == NULL){
        printf("File cant be opened");
    }
    printf("Welcome to ICS53 Online Cloud Storage.\n");

    //Iterating through Command Line File
    while (fgets(input, sizeof(input), fp))
    {
        printf("> %s", input);
        tokenize(input, args);
        if (strcmp(args[0], "quit") == 0){
            break;
        }
        execute(args, fp);
    }

    fclose(fp);
    return 0;
    
}





