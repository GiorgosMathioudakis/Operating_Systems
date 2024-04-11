/*test no*/

#include <stdio.h>

#define set_task_params(a,b) syscall(341, a, b)
#define get_task_params(a) syscall(342, a)

struct d_params {
	char group_name;
	int member_id;
};

int main(void) {
	struct d_params params;
	char group_name = 'A';
	int member_id = 1;
	
	printf("Setting params of current task_struct: group_name=%c, member_id=%d\n", group_name, member_id);
	set_task_params(group_name , member_id);
	get_task_params(&params);
	printf("Requested params of current task_struct: group_name=%c, member_id=%d\n", params.group_name, params.member_id);
	
	return 0;
}


//member_id wrong usage
#include <stdio.h>

#define set_task_params(a,b) syscall(341, a, b)
#define get_task_params(a) syscall(342, a)

struct d_params {
	char group_name;
	int member_id;
};

int main(void) {
	char group_name = 'A';
	int member_id = 0;
	
	struct d_params params;
	printf("Setting params of current task_struct: group_name=%c, member_id=%d\n", group_name, member_id);
	set_task_params(group_name , member_id);
	get_task_params(&params);
	printf("Requested params of current task_struct: group_name=%c, member_id=%d\n", params.group_name, params.member_id);
	
	return 0;
}


//wrong group_name usage 

/* Check if argument values are valid */

/*member id has to be > 0 */
if (member_id <=0) {
	printk("\nInvalid non positive value in member_id");
	return EINVAL;
}

/*group_name has to be english and upper case */
if( group_name < 'A' || group_name > 'Z'){
	printk("\nInvalid non english non upper case letter in group_name");
	return EINVAL;
}

//null params test

#include <stdio.h>

#define set_task_params(a,b) syscall(341, a, b)
#define get_task_params(a) syscall(342, a)

struct d_params {
	char group_name;
	int member_id;
};

int main(void){
	char group_name = 'F';
	int member_id = 20;

	struct d_params *params = NULL;

	printf("Setting current task_sturct with valid values!\n");
	set_task_params(group_name , member_id );
	get_task_params(params);
	printf("Getting current task_struct with null params pointer!\n");

	return 0;
}




/*
Author : Giorgos Mathioudakis csd4674

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
*/

