#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100

void execute_command(char *command) {
    pid_t pid;
    int status;

    // Create a child process
    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        // Child process
        char *args[MAX_COMMAND_LENGTH];
        int arg_count = 0;

        // Tokenize the command
        char *token = strtok(command, " \t\n");

        while (token != NULL) {
            args[arg_count++] = token;
            token = strtok(NULL, " \t\n");
        }

        args[arg_count] = NULL;

        // Execute the command
        execvp(args[0], args);

        exit(1);
    } else {
        // Parent process
        waitpid(pid, &status, 0);

        if (strcmp(command, "exit") == 0) {
            printf("Process end\n");
            exit(0);
        }

        if (strncmp(command, "cat ", 4) == 0) {
            // Add a newline after executing "cat" with a file
            printf("\n");
        }
    }
}

int main() {
    char command[MAX_COMMAND_LENGTH];

    while (1) {
        printf("osh> ");
        fflush(stdout);

        if (fgets(command, sizeof(command), stdin) == NULL) {
            perror("fgets");
            exit(1);
        }

        // Remove newline character from the end of the command
        size_t len = strlen(command);
        if (len > 0 && command[len - 1] == '\n') {
            command[len - 1] = '\0';
        }

        execute_command(command);
    }

    return 0;
}
