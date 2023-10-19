#include "shell.h"


int main(void){
    
    while(1){  //repeat forever
        buffer_t user_input;


        command_prompt(); //display command for terminal
        read_input(&user_input);


    }

    return 0;
}