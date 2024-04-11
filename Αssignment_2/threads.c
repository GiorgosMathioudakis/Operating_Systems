/*
Giorgos Mathioudakis
csd4674
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define MAX_IN_ROOM 8
#define MAX_STUDENTS 41
#define MIN_STUDENTS 20


sem_t binary_sem; 
sem_t sem_q[MAX_STUDENTS]; 
int queue[MAX_STUDENTS];
int spots=8; //spots for students inside the room
int flag=0;

//in_hall acts like a queue , I use a queue since I know the max number of students waiting
int in_room[MAX_STUDENTS] , in_hall[MAX_STUDENTS]; //in_room is array representing students in the studying room and in_hall are the students waiting

int front = -1;
int rear = -1; 

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t prio_mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int thread_id; //student AM
    int studying_time; //students random studying time
} thread_vars;

// Function to check if the queue is empty
int isEmpty() {
    return front == -1;
}

// Function to check if the queue is full
int isFull() {
    return (rear == MAX_STUDENTS - 1) || (front == 0 && rear == MAX_STUDENTS - 1);
}

// Function to add an element to the rear of the queue
void enqueue(int item) {
    if (isFull()) {
        printf("Queue is full. Cannot enqueue.\n");
        return;
    }

    if (front == -1) {
        front = 0;
    }

    rear = (rear + 1) % MAX_STUDENTS;
    queue[rear] = item;
}

// Function to remove and return an element from the front of the queue
int dequeue() {
    if (isEmpty()) {
        //printf("Queue is empty. Cannot dequeue.\n");
        return 0; // Error code
    }

    int item = queue[front];
    if (front == rear) {
        front = rear = -1; // Queue becomes empty
    } else {
        front = (front + 1) % MAX_STUDENTS;
    }
    return item;
}

void initialize_arrays(void){
    for(int i=0;i<MAX_STUDENTS;i++){
        in_hall[i] = 0 ; // index is id of student , 0 means student is not in that array ,1 means they are
        in_room[i] = 0 ;
    }
}

void print_students(void ){
    printf("\nStudy Room:");
    for(int i=0;i<MAX_STUDENTS;i++){
        if(in_room[i]==1){ // if value is 1 , i is the id of the student there 
            printf(" %d |" , i);
        }
    }
    printf("\nWaiting Room: ");

    for (int i = front; i <= rear; i++) {
        if(front!=-1){
            printf( "%d |", queue[i]);
        }
    }
    printf("\n\n");
}

void join_room(thread_vars *student){

    printf("Student %d joined the room \n",student->thread_id);
    spots--;
    in_room[student->thread_id] = 1; 
    in_hall[student->thread_id-1] = 0; //in case the student was in hall
    print_students();
}

void leave_room(thread_vars *student){
    sem_wait(&binary_sem);
    printf("Student %d left the room after studying for %d seconds\n" , student->thread_id , student->studying_time);
    in_room[student->thread_id] = 0; // student left studying room
    print_students();
    spots++;
    if(spots==MAX_IN_ROOM){
        //everyone left again
        int id;
        for(int i=0;i<MAX_IN_ROOM;i++){
            if(!isEmpty()){ //an den einai o teleutaios student
                pthread_mutex_lock(&prio_mutex);
                int id = dequeue();
                sem_post(&sem_q[id-1]); 
                //sleep(1);   
            }
        }    
    }
    sem_post(&binary_sem);
}


void* thread_function1(void *transferdata){
    pthread_mutex_lock(&mutex);
    thread_vars *input_data = (thread_vars*) transferdata;
    printf("Student %d was born\n" , input_data->thread_id);
    if(spots==0){
        printf("Student %d cannot enter the study hall.It is full.\n" , input_data->thread_id);
        in_hall[input_data->thread_id-1] = 1;
        enqueue(input_data->thread_id); // add to queue for when its time to join studying room
        print_students();
        flag = 1;
        pthread_mutex_unlock(&mutex);
    }else{
        sem_post(&sem_q[input_data->thread_id-1]);  // if a student can join then he should post for himself to join 
        flag = 0;
    }

    sem_wait(&sem_q[input_data->thread_id-1]);
    join_room(input_data);
    if(flag==1){
        pthread_mutex_unlock(&prio_mutex);
    }else{
        pthread_mutex_unlock(&mutex);
    }
    sleep(input_data->studying_time);
    leave_room(input_data);
    return 0;
}



int main(){
    //initialize 
    sem_init(&binary_sem, 0, 1);
    for(int i=0;i<MAX_STUDENTS;i++){
        sem_init(&sem_q[i] , 0 , 0);
    }
    initialize_arrays();
    int maxthreads; //number of students
    do{
        printf("Number of students(20-40): ");
        scanf("%d", &maxthreads);
    }while(maxthreads<MIN_STUDENTS || maxthreads>MAX_STUDENTS);
    //int maxthreads=15 ;
    
    pthread_t *threads = malloc(sizeof(thread_vars)*maxthreads);
    thread_vars *input_data = (thread_vars*) malloc(sizeof(thread_vars)*maxthreads);



    for(int i=0;i < maxthreads; i++){
        input_data[i].thread_id = i+1;
        input_data[i].studying_time = (rand() % 11) + 5;
        if (pthread_create(&(threads[i]), NULL, thread_function1, (void*)&input_data[i]) != 0) {
                fprintf(stderr, "Error creating thread %d\n", i);
                exit(-1);
            }
    }
    
    for(int i =0;i < maxthreads; i++){
        pthread_join(threads[i],NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&binary_sem);
    for(int i=0;i<MAX_STUDENTS;i++){
        sem_destroy(&sem_q[i]);
    }

    printf("Looks like everyone left..\n");
    
    return 0;
}