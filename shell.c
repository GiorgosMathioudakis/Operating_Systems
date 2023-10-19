#include "shell.h"


pid_t child_pid;

int pid = 0; 


char* update_user(){
    char* username = getpwuid(getuid())->pw_name;
    if(!username)
    {
        perror("ERROR: User not found.");
        exit(-1);
    }
}


char* update_path(){
    buffer_t cd ; //current directory
    char* prompt_cd; //prompt current directory

    cd.size = pathconf(".", _PC_PATH_MAX);
    if ((cd.input = (char*)malloc(cd.size * sizeof(char))))
        prompt_cd = getcwd(cd.input, cd.size);
}

void command_prompt(){

    int AM = 4674; 
    char* username = update_user();
    char* prompt_cd = update_path();
    
    if (username != NULL) {
        printf("\e[32m%d-hy345\e[0m\e[93m@%s:\e[94m%s\e[0m ", AM , username,prompt_cd);
        // Other user-related information can also be accessed
    } else {
        perror("Error getting user information");
    }
}

void read_input(buffer_t* user_input){

    user_input->input= NULL;
    user_input->size= 0;

    ssize_t characters_read = getline(&user_input->input, &user_input->size, stdin);

    if(characters_read==-1){ //getline() returned -1 / something went wrong
        perror("Error: Cant get user input.\n");
        exit(-1);
    }

    int input_size = strlen(user_input->input);
    if (input_size == 1) {
        free(user_input->input);
        user_input->input = NULL;
    }
    else user_input->input[input_size - 1] = '\0';

}

//FUNCTIONS TO USE IN input_to_command
//Create command with default attributes
command_t* create_command() {
    command_t* command= (command_t*)malloc(sizeof(command_t));
    command->command = NULL;
    command->argument_size = 0;
    command->pipe = 0;
    command->redirection =0;
    command->in_red =0;
    command->out_red =0;
    for(int i=0;i<MAX_ARG;i++){
        command->arguments[i] = malloc(sizeof(char*));
    }
    return command;
}

void print_command(command_t command){
    printf("command is : %s \n" , command.command );
    printf("command arguments are(%d):" , command.argument_size);
    for(int i=0;i<command.argument_size;i++){
        printf("%s," , command.arguments[i]);
    }
    printf("\n");
    if(command.pipe==1){
        printf("Command part of pipe \n");
    }else if(command.redirection==1){
        printf("redirection command \n");
        if(command.in_red){
            printf("input redirection from file %s \n" , command.input_file);
        }
        if(command.in_red){
            printf("output redirection from file %s \n" , command.output_file);
        }
    }else{
        printf("simple command\n");
    }
}


void push_arg(command_t* command , char* argument ,int arg_size){
    command->arguments[arg_size] = argument;
}

void input_to_command(buffer_t user_input,commands_t* commands){


    char **tokens;  //array to store all tokens from input
    tokens  = (char**)malloc(250* sizeof(char*)); // tokens max size is now 250 which should be enough for every input 
    if (tokens == NULL) { //checking if malloc worked
        perror("malloc");
        exit(1);
    }

    int size=0;
    // Tokenize the input
    char *token = strtok(user_input.input, " \t\n"); // Tokenize using spaces, tabs, and newline as delimiters
    int i=0;
    while (token != NULL) {
        // Process each token (word)
        //printf("Token from strtok: %s\n", token);  //comment out if you want to see token value
        tokens[i] = (char*)malloc(sizeof(char*)+1);  //size of char* +1 for null termninator 
        tokens[i] = token;
        i++;
        token = strtok(NULL, " \t\n"); // Get the next token         
    }
    size=i;
    

    if(size==0){
        return; //size 0 = no tokens
    }



    //initialize commands
    commands->commands_size = 0;

    //create command to store tokens and push that to commands
    command_t* command = create_command(); 


    // for(int i=0;i<size;i++){printf("tokens[%d] = %s\n" , i , tokens[i] );} //print out to see tokens array


    int start = 0 ; //start of command
    int commands_size = 0 ; //number of commands
    int argument_size = 0; //count the arguments

    for(int i=0; i<size; i++){    
        if(strcmp(tokens[i],"|")==0){
            //repetable proccess of pushing command
            command->command = command->arguments[0];   //command is the first argument
            command->pipe = 1; //pipe because of |
            command->arguments[argument_size] = NULL;  //we add null as last argument
            argument_size++; //arguments++ since NULL was added
            command->argument_size = argument_size;  //push argument size
            commands->commands[commands_size] = *command; //push command
            command = create_command(); //new command
            argument_size = 0; //reset
            commands_size++; //command size ++ since command was added
        }else if(strcmp(tokens[i],";")==0){
            command->command = command->arguments[0];
            command->pipe = 0; 
            command->arguments[argument_size] = NULL;
            argument_size++;
            command->argument_size = argument_size;
            commands->commands[commands_size] = *command;
            command = create_command(); //new command
            argument_size = 0;
            commands_size++;
        }else if(strcmp(tokens[i],"<")==0){
            //push redirection info
            command->redirection = 1;
            command->in_red = 1;
        }else if(strcmp(tokens[i],">")==0){
            //push redirection info 
            command->redirection = 1;
            command->out_red = 1;
        }else{
            command->arguments[argument_size] = tokens[i];
            argument_size++;
        }
    }
    command->command = command->arguments[0];
    command->pipe = 0;
    command->arguments[argument_size] = NULL;
    argument_size++;
    command->argument_size = argument_size;
    commands->commands[commands_size] = *command;
    commands->commands_size = commands_size + 1 ;


    //check which files have redirections
    for(int i=0;i<commands->commands_size;i++){
        for(int j=1;j<commands->commands[i].argument_size-1;j++){
            if(strcmp(commands->commands[i].arguments[j],"<")==0){
                //command has input redirection 
                commands->commands[i].in_red = 1;
                commands->commands[i].redirection = 1;
                commands->commands[i].input_file = commands->commands[i].arguments[j+1];
            }
            if(strcmp(commands->commands[i].arguments[j],">")==0){
                printf("wtf\n\n");
                //command has input redirection 
                commands->commands[i].out_red = 1;
                commands->commands[i].redirection = 1;
                commands->commands[i].output_file = commands->commands[i].arguments[j+1];
            }
        }
    }

    for(int i=0;i<commands->commands_size;i++){  //commenout out to print commands
        print_command(commands->commands[i]);
    }
    
}

//fork for commands
void simple_fork(command_t command){
    int status;

    pid = fork();
    if (pid == 0)
    { // Child process
        execvp(command.command, command.arguments);
        perror("ERROR: Can't execute command.\n");
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    { // Error
        perror("ERROR: Forking went wroing.\n");
        exit(EXIT_FAILURE);
    }
    else
    { // Parent process
        waitpid(pid, &status, WUNTRACED);
    }
}

void exec_cd(command_t command){
    if(command.argument_size!=3){
        // cd , null are 2 arguments so I need a 3rd for directory 
        perror("ERROR: too many arguments.");
    }else{
        if (chdir(command.arguments[1]) == 0) {
            printf("Changed directory to: %s\n", command.arguments[1]);
        } else {
            perror("chdir");
        }
    }

    
}

void exec_exit(){
    exit(0);
}

//execute non-pipe processes
void exec_normal_process(command_t command){
    if(strcmp(command.command,"chdir")==0){
        exec_cd(command);
    }else if(strcmp(command.command,"quit")==0){
        exec_exit();
    }else{
        simple_fork(command);
    }
}  


void exec_redirection_process(command_t command){

    int status;
    pid = fork();
    if (pid == 0) { // Child process
        // input redirection
        if (command.in_red) {
            int input_fd = open(command.input_file, O_RDONLY);
            if (input_fd < 0) {
                perror("ERROR: Input redirection failed");
                exit(EXIT_FAILURE);
            }
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }

        // output redirection
        if (command.out_red) {
            int output_fd = open(command.output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (output_fd < 0) {
                perror("ERROR: Output redirection failed");
                exit(EXIT_FAILURE);
            }
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }

        execvp(command.arguments[0], command.arguments);
        perror("ERROR: Can't execute command.\n");
        exit(EXIT_FAILURE);
    }
}

//execute pipes processes
void exec_pipe_process(command_t command){

}

void execute_command(commands_t commands){
    command_t command; //temp


    //traverse commands
    for(int i=0;i<commands.commands_size;i++){
        command = commands.commands[i];

        if(command.command == NULL){
            if (command.pipe==1) {   
                perror("ERROR: Expected expression before pipe.");
                break;
            }
            else continue;
        }

        if(command.redirection==1){
           exec_redirection_process(command);
        }else if(command.pipe==1){
            exec_pipe_process(command);
        }else{
            exec_normal_process(command);
        }
    }
}


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

