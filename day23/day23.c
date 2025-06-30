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