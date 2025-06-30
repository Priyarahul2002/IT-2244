//Area calculation

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void calculate_area(char shape, float a, float b, float *area) {
    switch (shape) {
        case 'C': *area = 3.1416 * a * a; break;  // Circle (radius)
        case 'T': *area = 0.5 * a * b; break;     // Triangle (base, height)
        case 'R': *area = a * b; break;           // Rectangle (length, breadth)
        case 'S': *area = a * a; break;           // Square (side)
        default: *area = -1;
    }
}

int main() {
    int fd1[2], fd2[2]; // Pipe file descriptors
    pid_t pid;

    pipe(fd1); // Pipe from Parent -> Child
    pipe(fd2); // Pipe from Child -> Parent

    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }

    if (pid > 0) { // Parent process
        close(fd1[0]);  // Close read end of first pipe
        close(fd2[1]);  // Close write end of second pipe

        char shape;
        float dim1, dim2;

        printf("Enter shape (C-Circle, T-Triangle, R-Rectangle, S-Square): ");
        scanf(" %c", &shape);

        if (shape == 'C' || shape == 'S') {
            printf("Enter one dimension (radius or side): ");
            scanf("%f", &dim1);
            dim2 = 0;
        } else {
            printf("Enter two dimensions (base & height for triangle, length & breadth for rectangle): ");
            scanf("%f %f", &dim1, &dim2);
        }

        write(fd1[1], &shape, sizeof(shape)); // Send shape
        write(fd1[1], &dim1, sizeof(dim1)); // Send dimension 1
        write(fd1[1], &dim2, sizeof(dim2)); // Send dimension 2
        close(fd1[1]); // Close write end

        wait(NULL); // Wait for child

        float area;
        read(fd2[0], &area, sizeof(area)); // Read calculated area
        close(fd2[0]); // Close read end

        if (area >= 0) {
            printf("Calculated Area: %.2f\n", area);
        } else {
            printf("Invalid shape input!\n");
        }
    } else { // Child process
        close(fd1[1]); // Close write end of first pipe
        close(fd2[0]); // Close read end of second pipe

        char shape;
        float dim1, dim2, area;

        read(fd1[0], &shape, sizeof(shape)); // Read shape
        read(fd1[0], &dim1, sizeof(dim1)); // Read dimension 1
        read(fd1[0], &dim2, sizeof(dim2)); // Read dimension 2
        close(fd1[0]); // Close read end

        calculate_area(shape, dim1, dim2, &area);

        write(fd2[1], &area, sizeof(area)); // Send result to parent
        close(fd2[1]); // Close write end
    }

    return 0;
}


output=>
Enter shape (C-Circle, T-Triangle, R-Rectangle, S-Square): T
Enter two dimensions (base & height for triangle, length & breadth for rectangle): 10 5
Calculated Area: 25.00
