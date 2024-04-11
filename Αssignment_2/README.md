# hy345_Assignment_2

Author : Giorgos Mathioudakis  - csd4674
Course : HY345

How to build :
make 

How to run : 
./ex2

The way the code works:
1.Every student is born and decided if he is going to join the room or wait (that is in locked code).This decision is based on counter spots representing how many spots are left in the studying room.
2.If the student has to wait , he joins queue and prints rooms
3.If the sutdent can join he posts his semaphore to join and prints rooms
4.The last student exiting studying room has to update the next 8 students based on queue and post their semaphores for them to join 
5.Back to 4. until all students join room, study and leave.