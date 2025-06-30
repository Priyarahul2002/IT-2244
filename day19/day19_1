vi shared1.c


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

typedef struct {
    int n;
    int r;
    int fact_n; //unsigned means only represent non-negative numbers, i.e., zero and positive numbers.
    int fact_r;
    int fact_n_r; // factorial of (n-r)
} shared_data_t;

int factorial(int x) {
    if (x == 0 || x == 1) return 1;
    int fact = 1;
    for (int i = 2; i <= x; i++) {
        fact *= i;
    }
    return fact;
}

int main() {
    size_t size = sizeof(shared_data_t);
    shared_data_t *shared = mmap(NULL, size, PROT_READ | PROT_WRITE,
                                MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shared == MAP_FAILED) {
        perror("mmap failed");
        exit(1);
    }

    // Parent reads n and r
    printf("Enter n: ");
    scanf("%d", &shared->n);
    printf("Enter r: ");
    scanf("%d", &shared->r);

    pid_t pid = fork();
    if (pid == 0) {
        // Child process: calculate factorials
        shared->fact_n = factorial(shared->n);
        shared->fact_r = factorial(shared->r);
        shared->fact_n_r = factorial(shared->n - shared->r);

        // Child prints factorials (optional)
        printf("Child computed factorials:\n");
        printf("fact(%d) = %llu\n", shared->n, shared->fact_n);
        printf("fact(%d) = %llu\n", shared->r, shared->fact_r);
        printf("fact(%d) = %llu\n", shared->n - shared->r, shared->fact_n_r);

        exit(0);
    } else if (pid > 0) {
        // Parent waits for child to finish
        wait(NULL);

        // Calculate nCr and nPr using factorials from shared memory
        int nCr = shared->fact_n / (shared->fact_r * shared->fact_n_r);
        int nPr = shared->fact_n / shared->fact_n_r;

        printf("Parent reads factorials and calculates:\n");
        printf("nCr = %llu\n", nCr);
        printf("nPr = %llu\n", nPr);

        munmap(shared, size);
    } else {
        perror("fork failed");
        exit(1);
    }

    return 0;
}


gcc shared1.c -o shared1

./shared1

/*
Output:-

Enter n: 5
Enter r: 2
Child computed factorials:
fact(5) = 120
fact(2) = 2
fact(3) = 6
Parent reads factorials and calculates:
nCr = 10
nPr = 20



*/