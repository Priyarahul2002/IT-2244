vi shared.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

int main(){
	size_t size = 1096;
	char *shared_mem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	
	if (shared_mem == MAP_FAILED){
		perror("mmap failed");
		exit(1);
	}
	
	pid_t pid = fork();
	if (pid == 0){
		// child process
		sprintf(shared_mem, "Hello from child!");
		printf("Child wrote: %s\n", shared_mem);
		exit(0);
	} else if (pid > 0){
		// parent process
		wait(NULL); // wait for child to finish
		printf("Parent read: %s\n", shared_mem);
		munmap(shared_mem, size);
	} else {
		perror("fork failed");
		exit(1);
	}
return 0;
}



gcc shared.c -o shared

./shared

/*
Output:-

Child wrote: Hello from child!
Parent read: Hello from child!


*/