//01>vi exit.c
#include <stdio.h>
#include <stdlib.h>  //for exit()
#include <unistd.h>  //for sleep()

int main(){
	printf("Program started.\n");
	
	printf("Sleeping for 3 seconds...\n");
	sleep(3);  //pause for 3 seconds
	
	printf("Exiting the program\n");
	exit(0);  //clean exit
}

output:Program started.
Sleeping for 3 seconds...
Exiting the program

--------------------------------
//02> vi wait.c
#include <stdio.h>
#include <stdlib.h>  //for exit()
#include <unistd.h>  //for fork(),sleep()
#include <sys/wait.h> //for wait()

int main(){
	
	pid_t pid;
	printf("Parent process started. PID: %d\n", getpid());
	pid =fork();  //create a new child process
	
	if(pid<0)
	{
		perror("fork failed");
		exit(1);
	}
	
	if(pid==0)
	{
		//child process
		printf("Child process. PID: %d, sleeping for 2 seconds..\n", getpid());
		sleep(2);
		printf("Child process exiting.\n");
		exit(0);  //child exits with status 0
	}
	else
	{
		//parent process
		int status;
		printf("Parent waiting for child to finish..\n");
		wait(&status);  //wait for child to finish
		if(WIFEXITED(status))
		{
			printf("Child exited with status: %d\n",WEXITSTATUS(status)); 
		}
		else
		{
			printf("Child did not exit normally.\n");
		}
		printf("Parent process ending.\n");
	}
	return 0;
}

output:Parent process started. PID: 10093
Parent waiting for child to finish..
Child process. PID: 10094, sleeping for 2 seconds..
Child process exiting.
Child exited with status: 0
Parent process ending.

----------------------------------------

//03> vi day15_3.c
Exercise 01:
First child: Slept for 1 seconds.
Second child:Slept for 3 seconds.
Parent: Both children have finished.



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid1, pid2;

  
    pid1 = fork();
    if (pid1 < 0) {
        perror("fork failed");
        exit(1);	
		
    } else if (pid1 == 0) {
       
        sleep(1);
        printf("First child slept for 1 second\n");
        exit(0);
    }

    
    pid2 = fork();
    if (pid2 < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid2 == 0) {
        
        sleep(3);
        printf("Second child slept for 3 seconds\n");
        exit(0);
    }

   
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    printf("Parent: both children have finished\n");

    return 0;
}

output:First child slept for 1 second
Second child slept for 3 seconds
Parent: both children have finished
--------------------------------------

//04>vi day15_4.c
Exercise 02:
First child: Sleeps for 2 seconds,exits with status2.
Second child:Sleeps for 1 second, exits with status1.
Parent: waits twice, and prints which child (by PID or exit code)
finished first and second.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid1, pid2;
    int status1, status2;
    pid_t finished_pid1, finished_pid2;
    int exit_status1, exit_status2;

    printf("Parent process started. PID: %d\n", getpid());

    // First child
    pid1 = fork();
    if (pid1 < 0) {
        perror("fork failed");
        exit(1);
    }
    if (pid1 == 0) {
        // Child 1
        printf("First child: PID %d, sleeping for 2 seconds.\n", getpid());
        sleep(2);
        printf("First child: exiting with status 2.\n");
        exit(2);
    }

    // Second child
    pid2 = fork();
    if (pid2 < 0) {
        perror("fork failed");
        exit(1);
    }
    if (pid2 == 0) {
        // Child 2
        printf("Second child: PID %d, sleeping for 1 second.\n", getpid());
        sleep(1);
        printf("Second child: exiting with status 1.\n");
        exit(1);
    }

    // Parent process: wait for both children and record the order
    printf("Parent: waiting for children to finish...\n");

    // Wait for first finished child
    finished_pid1 = wait(&status1);
    if (finished_pid1 == -1) {
        perror("wait failed");
        exit(1);
    }
    exit_status1 = WIFEXITED(status1) ? WEXITSTATUS(status1) : -1;

    // Wait for second finished child
    finished_pid2 = wait(&status2);
    if (finished_pid2 == -1) {
        perror("wait failed");
        exit(1);
    }
    exit_status2 = WIFEXITED(status2) ? WEXITSTATUS(status2) : -1;

    // Print the order of finishing
    printf("Parent: Child with PID %d finished first with exit status %d.\n", finished_pid1, exit_status1);
    printf("Parent: Child with PID %d finished second with exit status %d.\n", finished_pid2, exit_status2);

    printf("Parent: Both children have finished.\n");

    return 0;
}

output:
Parent process started. PID: 14156
Parent: waiting for children to finish...
First child: PID 14157, sleeping for 2 seconds.
Second child: PID 14158, sleeping for 1 second.
Second child: exiting with status 1.
First child: exiting with status 2.
Parent: Child with PID 14158 finished first with exit status 1.
Parent: Child with PID 14157 finished second with exit status 2.
Parent: Both children have finished.



//05>
Exercise 03:
The parent creates a child process.
The child creates a grandchild process.
The grandchild sleeps 2 seconds and exits with status2.
The child waits for the grandchild prints its exit status,
then exits with status 55.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid_child, pid_grandchild;
    int status;

    printf("Parent started. PID: %d\n", getpid());

    pid_child = fork();
    if (pid_child < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid_child == 0) {
        // Child process
        printf("Child started. PID: %d\n", getpid());

        pid_grandchild = fork();
        if (pid_grandchild < 0) {
            perror("fork failed");
            exit(1);
        }

        if (pid_grandchild == 0) {
            // Grandchild process
            printf("Grandchild started. PID: %d, sleeping 2 seconds...\n", getpid());
            sleep(2);
            printf("Grandchild exiting with status 2.\n");
            exit(2);
        } else {
            // Child waits for grandchild
            waitpid(pid_grandchild, &status, 0);
            if (WIFEXITED(status)) {
                printf("Child: Grandchild exited with status %d.\n", WEXITSTATUS(status));
            } else {
                printf("Child: Grandchild did not exit normally.\n");
            }
            printf("Child exiting with status 55.\n");
            exit(55);
        }
    } else {
        // Parent process
        waitpid(pid_child, &status, 0);
        if (WIFEXITED(status)) {
            printf("Parent: Child exited with status %d.\n", WEXITSTATUS(status));
        } else {
            printf("Parent: Child did not exit normally.\n");
        }
        printf("Parent exiting.\n");
    }

    return 0;
}

output:
Parent started. PID: 14353
Child started. PID: 14354
Grandchild started. PID: 14355, sleeping 2 seconds...
Grandchild exiting with status 2.
Child: Grandchild exited with status 2.
Child exiting with status 55.
Parent: Child exited with status 55.
Parent exiting.
