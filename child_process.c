/**
 * Child process functionality.
 * Contains functions that are common for both the ECG and image child processes.
 *
 * Author: Paul Roode
 */
#include "cps.h"

/**
 * Transmits data to the server through FIFOs.
 *
 * @param fifo_name Name of the server FIFO.
 * @param client_fifo_template Template string to generate client FIFO name using the client's PID.
 */
void transmit_data(const char *fifo_name, const char *client_fifo_template)
{
    int server_fifo_fd, client_fifo_fd;
    data_st data;
    int bytes_sent = 0, res;
    char client_fifo[256];

    // Arbitrary data length, based on the size of the data structure and number of chunks to be sent
    int total_bytes = sizeof(data) * 10;

    // Open server FIFO for writing
    server_fifo_fd = open(fifo_name, O_WRONLY);
    if (server_fifo_fd == -1)
    {
        perror("Open server FIFO failed");
        exit(EXIT_FAILURE);
    }

    // Get the current process' PID and create a client FIFO using this PID
    data.client_pid = getpid();
    sprintf(client_fifo, client_fifo_template, data.client_pid);
    if (mkfifo(client_fifo, 0777) == -1)
    {
        perror("Create client FIFO failed");
        exit(EXIT_FAILURE);
    }

    // Send the total byte count to the server first
    write(server_fifo_fd, &total_bytes, sizeof(int));

    // Send the actual data in chunks to the server
    for (int i = 0; i < 10; i++)
    {
        sprintf(data.data, "Data chunk %d from client %d", i, data.client_pid);
        write(server_fifo_fd, &data, sizeof(data));
        bytes_sent += sizeof(data);
    }

    // Open the client FIFO for reading to receive acknowledgment from the server
    client_fifo_fd = open(client_fifo, O_RDONLY);
    if (client_fifo_fd != -1)
    {
        int bytes_read = 0;
        read(client_fifo_fd, &bytes_read, sizeof(int));

        // If there's a mismatch in the number of bytes sent and acknowledged, send SIGALRM to parent
        if (bytes_sent != bytes_read)
        {
            kill(getppid(), SIGALRM);
        }
        else // If everything went well, send SIGUSR1 to parent
        {
            kill(getppid(), SIGUSR1);
        }
        close(client_fifo_fd);
    }

    // Cleanup: Close the server FIFO and remove the client FIFO
    close(server_fifo_fd);
    unlink(client_fifo);
}
