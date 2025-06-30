(01)
//buffer-over flows and temporary storage

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MSGSIZE 16

char* msg1 = "hello, world #1";
char* msg2 = "hello, world #2";
char* msg3 = "hello, world #3";


int main(){
	char inbuf[MSGSIZE];
	int p[2],i;
	
	if(pipe(p)<0)
		exit(1);
	
	/*continued
	write pipe*/
	
	write(p[1], msg1,MSGSIZE); //write(file descriptor ,message,message size)
	write(p[1], msg2,MSGSIZE);
	write(p[1], msg3,MSGSIZE);
	
	for(i=0;i<3;i++){
		//read pipe
		read(p[0],inbuf,MSGSIZE); //read(fildes,msg,msgsize)
		printf("%s\n",inbuf);
	}
	return 0;
}
[2021ict89@fedora ~]$ vi pipe.c
[2021ict89@fedora ~]$ chmod 777 pipe.c
[2021ict89@fedora ~]$ gcc pipe.c -o pipe
[2021ict89@fedora ~]$ ./pipe
hello, world #1
hello, world #2
hello, world #3

===================================================================================================================================================
 exercise (02)
1. get 2 inputs from users(//parent process)
name:
regno:
age:
2. print the output from the child(//child process)

name:tikku
regno:2021ict89
age:23

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    char name[50];
    char regno[20];
    int age;

    // Taking input in the parent process
    printf("Enter Name: ");
    scanf("%s", name);
    printf("Enter Reg No: ");
    scanf("%s", regno);
    printf("Enter Age: ");
    scanf("%d", &age);

    pid_t pid = fork();  // Creating the child process

    if (pid < 0) {
        printf("Fork failed!\n");
        return 1;
    }
    else if (pid == 0) { // Child process
        printf("\nChild Process Output:\n");
        printf("Name: %s\n", name);
        printf("Reg No: %s\n", regno);
        printf("Age: %d\n", age);
    }
    else { // Parent process
        wait(NULL); // Wait for child to finish
        printf("\nParent Process Completed!\n");
    }

    return 0;
}

[2021ict89@fedora ~]$ vi exer.c
[2021ict89@fedora ~]$ chmod 777 exer.c
[2021ict89@fedora ~]$ gcc exer.c -o exer
[2021ict89@fedora ~]$ ./exer
Enter Name: Tikku
Enter Reg No: 2021ict89
Enter Age: 23

Child Process Output:
Name: Tikku
Reg No: 2021ict89
Age: 23

Parent Process Completed!

----------------------------------------------------------------------------------------------
(pipe method)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MSGSIZE 100  // Define message size

int main() {
    int p[2];  // Pipe file descriptors
    char buffer[MSGSIZE];  // Buffer to store received message
    pid_t pid;

    // Creating a pipe
    if (pipe(p) < 0) {
        perror("Pipe creation failed");
        exit(1);
    }

    pid = fork();  // Creating child process

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    else if (pid > 0) { // Parent Process
        close(p[0]); // Close reading end of the pipe

        // Taking user input
        char name[50], regno[20], age[10];
        printf("Enter Name: ");
        scanf("%s", name);
        printf("Enter Reg No: ");
        scanf("%s", regno);
        printf("Enter Age: ");
        scanf("%s", age);

        // Formatting message and writing to pipe
        snprintf(buffer, MSGSIZE, "Name: %s\nReg No: %s\nAge: %s", name, regno, age);
        write(p[1], buffer, strlen(buffer) + 1);  // Sending formatted message to child
        
        close(p[1]); // Close writing end of the pipe
    }
    else { // Child Process
        close(p[1]); // Close writing end of the pipe

        // Reading data from parent
        read(p[0], buffer, MSGSIZE);
        printf("\nChild Process Output:\n%s\n", buffer);

        close(p[0]); // Close reading end of the pipe
    }

    return 0;
}

[2021ict89@fedora ~]$ vi exer.c
[2021ict89@fedora ~]$ chmod 777 exer.c
[2021ict89@fedora ~]$ gcc exer.c -o exer
[2021ict89@fedora ~]$ ./exer
Enter Name: Tikku
Enter Reg No: 2021ict89
Enter Age: 23

Child Process Output:
Name: Tikku
Reg No: 2021ict89
Age: 23

===================================================================================================================================================
exercise (03)
 step01: parent process get input from user
 step02: sending these inputs to child process
 step03: child process getting these inputs from parent and doing calculation
 step04: step the calculated values to parent
 step05: parent print the outputs
 
 (using fork call and pipe method)
 //2 pipes
 [circle,rectangle,triangle,square]
 
 Area calculation program
 Circle=>C (need radius)
 Triangle=>T (base and height)
 Square=>S (side)
 Rectangle=>R (length and width)
 //inside the int main
 need to read parameters
 
 //example
 enter your choice:C
enter the radius:20.9
area is: 1371.58

like this need to calculate the area of the given shapes



#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main() {
    int fd1[2], fd2[2]; // Pipes: fd1 for parent-to-child, fd2 for child-to-parent
    pid_t pid;
    char shape;
    float param1, param2, area;

    if (pipe(fd1) == -1 || pipe(fd2) == -1) {
        perror("Pipe failed");
        return 1;
    }

    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }

    if (pid > 0) { // Parent Process
        close(fd1[0]); // Close unused read end
        close(fd2[1]); // Close unused write end

        printf("Enter your choice (C, T, S, R): ");
        scanf(" %c", &shape);
        
        if (shape == 'C') {
            printf("Enter radius: ");
            scanf("%f", &param1);
        } else if (shape == 'T') {
            printf("Enter base and height: ");
            scanf("%f %f", &param1, &param2);
        } else if (shape == 'S') {
            printf("Enter side length: ");
            scanf("%f", &param1);
        } else if (shape == 'R') {
            printf("Enter length and width: ");
            scanf("%f %f", &param1, &param2);
        } else {
            printf("Invalid choice.\n");
            exit(1);
        }

        write(fd1[1], &shape, sizeof(shape));
        write(fd1[1], &param1, sizeof(param1));
        write(fd1[1], &param2, sizeof(param2));

        read(fd2[0], &area, sizeof(area));
        printf("Calculated area: %.2f\n", area);

        close(fd1[1]);
        close(fd2[0]);
    } else { // Child Process
        close(fd1[1]); // Close unused write end
        close(fd2[0]); // Close unused read end

        read(fd1[0], &shape, sizeof(shape));
        read(fd1[0], &param1, sizeof(param1));
        read(fd1[0], &param2, sizeof(param2));

        if (shape == 'C') {
            area = 3.1416 * param1 * param1;
        } else if (shape == 'T') {
            area = 0.5 * param1 * param2;
        } else if (shape == 'S') {
            area = param1 * param1;
        } else if (shape == 'R') {
            area = param1 * param2;
        }

        write(fd2[1], &area, sizeof(area));

        close(fd1[0]);
        close(fd2[1]);
        exit(0);
    }

    return 0;
}
[2021ict89@fedora ~]$ vi exerP2.c
[2021ict89@fedora ~]$ gcc exerP2.c -o exerP2
[2021ict89@fedora ~]$ ./exerP2
Enter your choice (C, T, S, R): C
Enter radius: 14
Calculated area: 615.75
[2021ict89@fedora ~]$ ./exerP2
Enter your choice (C, T, S, R): T
Enter base and height: 12 8
Calculated area: 48.00
[2021ict89@fedora ~]$ ./exerP2
Enter your choice (C, T, S, R): S
Enter side length: 5
Calculated area: 25.00
[2021ict89@fedora ~]$ ./exerP2
Enter your choice (C, T, S, R): R
Enter length and width: 4 6
Calculated area: 24.00

===================================================================================================================================================
