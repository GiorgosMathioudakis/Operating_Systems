// #include "shell.h"
// //fork


// int status;
// pid = fork();
// if (pid == 0) { // Child process
//     // input redirection
//     if (input_file) {
//         int input_fd = open(input_file, O_RDONLY);
//         if (input_fd < 0) {
//             perror("ERROR: Input redirection failed");
//             exit(EXIT_FAILURE);
//         }
//         dup2(input_fd, STDIN_FILENO);
//         close(input_fd);
//     }

//     // output redirection
//     if (output_file) {
//         int output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//         if (output_fd < 0) {
//             perror("ERROR: Output redirection failed");
//             exit(EXIT_FAILURE);
//         }
//         dup2(output_fd, STDOUT_FILENO);
//         close(output_fd);
//     }

//     execvp(command_args[0], command_args);
//     perror("ERROR: Can't execute command.\n");
//     exit(EXIT_FAILURE);