/* Author : Giorgos Mathioudakis csd4674 */

In this exercise to implement a new system call I went through the following steps : 
1. Defined 2 new system calls and increased number of system calls by 2 
2. Defined a function pointer pointing to the new system calls.
3. Defined the new functions signatures 
4. Added 2 extra fields in task_struct as requested 
5. Created a header d_params.h with the parameters
6. Implemented the new functions for the system calls
7. Added obj-y += <function_name>.o to Makefile for the 2 system calls
8. Compiled the new bzImage and opened the GuestOs on Qemu
9. Created tests and moved d_params header in GuestOs to test the system calls.
