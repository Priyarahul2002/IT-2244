---------------------------------------------------------------------------

/* Exercise

IPC using Message Queue

read inputs from the parent process
Enter name:hi
Enter regNo:2314
Enter Age:24

Message sent successfully.

give output from the child process

Received Name:hi
Received RegNo:2314
Received Age:24
*/


vi sendParentReceiveChild.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define MAX 100

// Structure for message
struct message {
    long msg_type;
    char name[MAX];
    char regNo[MAX];
    int age;
} msg;

int main() {
    key_t key;
    int msgid;   

    // Generate unique key
    key = ftok("keyfile", 65);
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    // Create message queue
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid > 0) {
        //  Parent Process: Sender
        msg.msg_type = 1;
		printf("I'm parent process. My pid is : %d\t  and my ppid is :%d\n ", getpid(), getppid());

        printf("Enter name: ");
        fgets(msg.name, MAX, stdin);
        msg.name[strcspn(msg.name, "\n")] = '\0';

        printf("Enter regNo: ");
        fgets(msg.regNo, MAX, stdin);
        msg.regNo[strcspn(msg.regNo, "\n")] = '\0';

        printf("Enter Age: ");
        scanf("%d", &msg.age);

        // Send message
        if (msgsnd(msgid, &msg, sizeof(struct message) - sizeof(long), 0) == -1) {
            perror("msgsnd");
            exit(1);
        }

        printf("\nMessage sent successfully.\n");

    } else {
        // Child Process: Receiver
        sleep(1); // small delay to let parent send message
		
        if (msgrcv(msgid, &msg, sizeof(struct message) - sizeof(long), 1, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }
		printf("I'm child process. My pid is : %d\t  and my ppid is :%d\n ", getpid(), getppid());

        printf("\nReceived Name: %s\n", msg.name);
        printf("Received RegNo: %s\n", msg.regNo);
        printf("Received Age: %d\n", msg.age);

        // Clean up
        msgctl(msgid, IPC_RMID, NULL);
    }

    return 0;
}

touch keyfile

chmod 644 keyfile

gcc sendParentReceiveChild.c -o sendParentReceiveChild

./sendParentReceiveChild

Output:-

I'm parent process. My pid is : 16189     and my ppid is :2749
 Enter name: Hi
Enter regNo: 2314
Enter Age: 24

Message sent successfully.
I'm child process. My pid is : 16190      and my ppid is :16189

Received Name: Hi
Received RegNo: 2314
Received Age: 24

-------------------------------------------------------------------------------------