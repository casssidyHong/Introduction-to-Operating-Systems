#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void show(int fork_id) {
    printf("Fork %d. I'm the child %d, my parent is %d.\n", fork_id, getpid(), getppid());
}

void fork_3_4() {

    pid_t pid;
    pid = fork(); //fork 3
    if (pid == 0)
        show(3);
    else
        wait(NULL);

    pid = fork(); //fork 4
    if (pid == 0)
        show(4);
    else
        wait(NULL);
}

int main() {
    printf("Main Process ID: %d\n\n", getpid());
    
    pid_t pid;
    pid = fork(); // Fork 1
    if (pid == 0) { 
        show(1);
    } else if (pid > 0) {
        wait(NULL);
    }

    pid = fork(); //fork 2
    if (pid == 0){
        show(2);
    }
    else {
        wait(NULL);
        fork_3_4();
    }

    return 0;
}
