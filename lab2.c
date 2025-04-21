#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <locale.h>
#include <unistd.h> 

// Structure for passing arguments to the thread
typedef struct thread_args {
    int id;                // Thread identifier
    int sleep_time;        // Thread sleep time
    int priority;          // Thread priority        
    const char* msg;       // Message to display
} thread_args;

// Function executed by the thread
void *thread_func(void *args) {    
    thread_args* cur_args = (thread_args*)args; 

    // Display information about the start of thread execution
    printf("Thread %d: started execution. Priority = %d\n", cur_args->id, cur_args->priority);
    printf("Thread %d: %s\n", cur_args->id, cur_args->msg);

    // Simulate thread work
    sleep(cur_args->sleep_time); // Delay for the specified time

    // Display information about the end of thread execution
    printf("Thread %d: finished execution\n", cur_args->id);

    pthread_exit(NULL);
}

// Function to create a thread with specified parameters
void create_thread(pthread_t* tid, int policy, int prio, void*(func)(void*), void* args) {
    pthread_attr_t attr;
    struct sched_param param;

    // Initialize thread attributes
    pthread_attr_init(&attr);

    // Set priority and scheduling policy
    param.sched_priority = prio;
    pthread_attr_setschedparam(&attr, &param);
    pthread_attr_setschedpolicy(&attr, policy);

    // Set explicit scheduling inheritance
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

    // Set thread state (joinable)
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    // Create the thread
    if (pthread_create(tid, &attr, func, args) != 0) {
        perror("Error creating thread");
        exit(EXIT_FAILURE);
    }

    // Destroy thread attributes
    pthread_attr_destroy(&attr);
}

int main() {
    // Thread identifiers
    pthread_t thread_id1, thread_id2;

    // Arguments for threads
    thread_args args1 = {1, 5, 6, "Message from the first thread"};
    thread_args args2 = {2, 10, 9, "Message from the second thread"};

    // Create threads
    create_thread(&thread_id1, SCHED_RR, args1.priority, thread_func, (void*)&args1);
    create_thread(&thread_id2, SCHED_FIFO, args2.priority, thread_func, (void*)&args2);

    // Wait for threads to finish
    if (pthread_join(thread_id1, NULL) != 0) {
        perror("Error waiting for the first thread to finish");
        exit(EXIT_FAILURE);
    }
    if (pthread_join(thread_id2, NULL) != 0) {
        perror("Error waiting for the second thread to finish");
        exit(EXIT_FAILURE);
    }

    printf("All threads have finished.\n");
    printf("Sergeev Vladislav Vladimirovich i914B.\n");
    return 0;
}