# hy345_Assignment_1

Author : Giorgos Mathioudakis  - csd4674
Course : HY345

How to build :
make 

How to run : 
./hy345sh

Project structure : 
hy345sh.c is the main file and includes shell.h
shell.h is the header and includes all four main functions and libraries needed
shellc executes the four main functions and has assisting functions as well 

The way the shell works:
1. Main loop starts
2. Update prompt
3. Read input from stdin and store to user_input
4. Get input from user and make commands out of it 
5. Execute these commands 
6. Back to 2. until quit is executed