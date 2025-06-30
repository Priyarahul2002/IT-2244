/*Q1) Write a C program that creates two threads.
Each thread should print a message indicating it is running. 
The main thread should wait for both threads to finish before exiting.*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Function to be executed by each thread
void* threadFunction(void* arg) {
    int threadNum = *(int*)arg;
    printf("Thread %d is running.\n", threadNum);
    pthread_exit(NULL);
}

int main() {
    pthread_t thread1, thread2;
    int t1 = 1, t2 = 2;

    // Create two threads
    if (pthread_create(&thread1, NULL, threadFunction, &t1)) {
        fprintf(stderr, "Error creating thread 1\n");
        return 1;
    }

    if (pthread_create(&thread2, NULL, threadFunction, &t2)) {
        fprintf(stderr, "Error creating thread 2\n");
        return 1;
    }

    // Wait for both threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Both threads have finished. Main thread exiting.\n");
    return 0;
}

[2021ict89@fedora ~]$ vi thread.cvi thread.c
2 files to edit
[2021ict89@fedora ~]$ vi thread.c
[2021ict89@fedora ~]$ gcc thread.c -o thread
[2021ict89@fedora ~]$ ./thread
Thread 1 is running.
Thread 2 is running.
Both threads have finished. Main thread exiting.

=========================================================================================================================================================================================

/*Q2)Write a program to create 5 threads.
Each thread should print its thread ID and the exit.
the main thread should join all threads and print a completion message.*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Function that each thread will run
void* threadFunction(void* arg) {
    pthread_t tid = pthread_self();  // Get thread ID
    printf("Thread ID: %lu is running.\n", (unsigned long)tid);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[5];

    // Create 5 threads
    for (int i = 0; i < 5; ++i) {
        if (pthread_create(&threads[i], NULL, threadFunction, NULL)) {
            fprintf(stderr, "Error creating thread %d\n", i);
            return 1;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < 5; ++i) {
        pthread_join(threads[i], NULL);
    }

    printf("All threads have finished. Main thread exiting.\n");
    return 0;
}

[2021ict89@fedora ~]$ vi thread.c
[2021ict89@fedora ~]$ gcc thread.c -o thread
[2021ict89@fedora ~]$ ./thread
Thread ID: 139957010994752 is running.
Thread ID: 139957002602048 is running.
Thread ID: 139956994209344 is running.
Thread ID: 139956985816640 is running.
Thread ID: 139956977423936 is running.
All threads have finished. Main thread exiting.

=========================================================================================================================================================================================

/*Q3)Write a program that creates a thread which prints numbers from 1 to 10 with a 1-second delay between each number.
the main thread should wait for this thread to finish*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>  // for sleep()

void* printNumbers(void* arg) {
    for (int i = 1; i <= 10; ++i) {
        printf("Number: %d\n", i);
        sleep(1);  // wait for 1 second
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t thread;

    // Create the thread
    if (pthread_create(&thread, NULL, printNumbers, NULL)) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    // Wait for the thread to finish
    pthread_join(thread, NULL);

    printf("Thread finished execution. Main thread exiting.\n");
    return 0;
}

[2021ict89@fedora ~]$ vi thread.c
[2021ict89@fedora ~]$ gcc thread.c -o thread
[2021ict89@fedora ~]$ ./thread
Number: 1
Number: 2
Number: 3
Number: 4
Number: 5
Number: 6
Number: 7
Number: 8
Number: 9
Number: 10
Thread finished execution. Main thread exiting.

=========================================================================================================================================================================================

/*Q4)Create two threads where one thread increments global counter 100 times and the other thread decrements the same counter 100 times.
print the final value of the counter after both threads finish.*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int counter = 0;                 // Global counter
pthread_mutex_t lock;           // Mutex for thread synchronization

void* increment(void* arg) {
    for (int i = 0; i < 100; ++i) {
        pthread_mutex_lock(&lock);
        ++counter;
        pthread_mutex_unlock(&lock);
    }
    pthread_exit(NULL);
}

void* decrement(void* arg) {
    for (int i = 0; i < 100; ++i) {
        pthread_mutex_lock(&lock);
        --counter;
        pthread_mutex_unlock(&lock);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t t1, t2;

    // Initialize the mutex
    if (pthread_mutex_init(&lock, NULL)) {
        fprintf(stderr, "Mutex initialization failed\n");
        return 1;
    }

    // Create the threads
    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, decrement, NULL);

    // Wait for threads to complete
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Destroy the mutex
    pthread_mutex_destroy(&lock);

    // Final counter value
    printf("Final counter value: %d\n", counter);

    return 0;
}

[2021ict89@fedora ~]$ vi thread.c
[2021ict89@fedora ~]$ gcc thread.c -o thread
[2021ict89@fedora ~]$ ./thread
Final counter value: 0

This is because the increment and decrement operations perfectly balance each other out, and the mutex ensures there are no race conditions or data corruption along the way.
However, if you were to remove the mutex locking, the output might not always be zero. Without synchronization, the threads could interleave in unpredictable ways, 
leading to missed updates—a classic example of a data race

=========================================================================================================================================================================================

/*Q5)Implement a thread function that takes an integer arguments, squares it, and returns the result to the main thread using pthread_exit and pthread_join*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Thread function that receives an integer, squares it, and returns result
void* squareNumber(void* arg) {
    int number = *(int*)arg;
    int* result = malloc(sizeof(int));  // allocate memory for result
    *result = number * number;
    pthread_exit(result);
}

int main() {
    pthread_t thread;
    int input = 7;  // example input value
    int* result;

    // Create the thread, passing &input as the argument
    if (pthread_create(&thread, NULL, squareNumber, &input)) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    // Join the thread and collect the result
    if (pthread_join(thread, (void**)&result)) {
        fprintf(stderr, "Error joining thread\n");
        return 2;
    }

    printf("Square of %d is %d\n", input, *result);

    free(result);  // free allocated memory
    return 0;
}

[2021ict89@fedora ~]$ vi thread.c
[2021ict89@fedora ~]$ gcc thread.c -o thread
[2021ict89@fedora ~]$ ./thread
Square of 7 is 49