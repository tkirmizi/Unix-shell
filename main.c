#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

// Signal handler function
void handle_signal(int sig) {
    if (sig == SIGINT) {
        // Handle Ctrl-C
        printf("\n");
        rl_on_new_line();
        rl_redisplay();
    } else if (sig == SIGQUIT) {
        // Handle Ctrl-\
        // Do nothing
    }
}

// Function to setup signal handlers
void setup_signals() {
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    // Set handler for SIGINT (Ctrl-C)
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // Set handler for SIGQUIT (Ctrl-\)
    if (sigaction(SIGQUIT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
}

int main() {
    char *input;

    setup_signals(); // Setup signal handlers

    while (1) {
        // Read input from the user
        input = readline("my_shell> ");
        
        if (input == NULL) {
            // Handle EOF (Ctrl-D)
            printf("\nExiting shell.\n");
            break;
        }

        // Add input to history
        add_history(input);

        // Process the input (not implemented in this example)
        printf("You entered: %s\n", input);

        // Free the allocated memory
        free(input);
    }

    return 0;
}
