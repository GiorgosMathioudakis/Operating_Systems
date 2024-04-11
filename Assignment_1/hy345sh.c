/*
Giorgos Mathioudakis
Csd4674
*/

#include "shell.h"


int main(){

    buffer_t user_input;
    commands_t commands;

    while(1){
        command_prompt();
        read_input(&user_input);
        input_to_command(user_input, &commands);
        execute_command(commands);
    }

    return 0;
}