#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <pwd.h>
#include <sys/wait.h>   
#include <fcntl.h>      
#include <string.h>     

#define MAX_ARG 50

#ifndef SHELL_H
#define SHELL_H

typedef struct{
    char* input;
    size_t size;
} buffer_t;


typedef struct{
    char* command;  //command
    char* arguments[MAX_ARG];  //arguments of the command (may be more than 1)
    int argument_size;  //num of args
    int pipe; //1 == part of pipe , 0 == single command 
    int in_red; //1==input redirection
    int out_red; //1==out redirection
    int redirection; //1==redirection
    char* input_file; 
    char* output_file;
} command_t;

typedef struct{
    command_t commands[100];  //all commands 
    int commands_size; //num of all commands 
} commands_t;

 
void command_prompt();   //this function requires 2 more functions , update_user and update_path 
void read_input(buffer_t* user_input); //this function gets the input from the user 
void input_to_command(buffer_t user_input,commands_t* commands); //this function gets input_user and makes commands out of it 
void execute_command(commands_t commands); 

#endif