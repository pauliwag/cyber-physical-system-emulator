/**
 * Main CPS program.
 * Creates two child processes for ECG and image data transmissions and handles the signals.
 *
 * Author: Paul Roode
 */
#include "cps.h"

/**
 * Handles the signals sent by child processes indicating the status of data transmission.
 *
 * @param signo The received signal number.
 * @param info Contains signal metadata, including the sender's PID.
 * @param context Not used here but can provide context when the signal was received.
 */
void handle_signal(int signo, siginfo_t *info, void *context)
{
    if (signo == SIGUSR1)
    {
        printf("Successful transmission received from child process %d\n", info->si_pid);
    }
    else if (signo == SIGALRM)
    {
        printf("Unsuccessful transmission received from child process %d\n", info->si_pid);
    }
}

int main()
{
    pid_t ecg_pid, image_pid;

    // Setting up signal handling
    struct sigaction sa;
    sa.sa_sigaction = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO; // Use SA_SIGINFO to get signal metadata
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGALRM, &sa, NULL);

    // Forking to create the ECG child process
    ecg_pid = fork();
    if (ecg_pid == 0)
    { // ECG child
        transmit_data(ECG_FIFO_NAME, ECG_CLIENT_FIFO_NAME);
        exit(EXIT_SUCCESS);
    }
    else if (ecg_pid < 0)
    { // Forking error
        perror("ECG fork failed");
        exit(EXIT_FAILURE);
    }

    // Delay to ensure proper sequencing of server-client interactions
    sleep(1);

    // Forking to create the Image child process
    image_pid = fork();
    if (image_pid == 0)
    { // Image child
        transmit_data(IMAGE_FIFO_NAME, IMAGE_CLIENT_FIFO_NAME);
        exit(EXIT_SUCCESS);
    }
    else if (image_pid < 0)
    { // Forking error
        perror("Image fork failed");
        exit(EXIT_FAILURE);
    }

    // Parent process waits for both child processes to complete their tasks
    wait(NULL);
    wait(NULL);

    return 0;
}
