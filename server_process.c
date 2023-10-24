/**
 * Server functionality.
 * Provides the main server processing functionality, handling data receipt and acknowledgment.
 * This code is common for both the ECG and image servers.
 *
 * Author: Paul Roode
 */
#include "cps.h"

/**
 * Processes data received from a client through FIFOs.
 *
 * @param fifo_name Name of the server FIFO.
 * @param client_fifo_template Template string to generate client FIFO name using the client's PID.
 */
void server_function(const char *fifo_name, const char *client_fifo_template)
{
    int server_fifo_fd, client_fifo_fd;
    data_st data;
    int bytes_read = 0, res;
    char client_fifo[256];
    int total_bytes_to_receive;

    // Create server FIFO if it doesn't exist
    if (access(fifo_name, F_OK) == -1)
    {
        mkfifo(fifo_name, 0777);
    }

    // Open the server FIFO for reading
    server_fifo_fd = open(fifo_name, O_RDONLY);
    if (server_fifo_fd == -1)
    {
        perror("Open server FIFO failed");
        exit(EXIT_FAILURE);
    }

    // First, read the total byte count from the client to know how much data to expect
    read(server_fifo_fd, &total_bytes_to_receive, sizeof(int));

    // Read the first chunk of data to identify which client we are receiving data from
    res = read(server_fifo_fd, &data, sizeof(data));
    if (res > 0)
        bytes_read += res;

    // Display the client data that the server is expecting and starting to process
    printf("Server expecting a total of %d bytes from client with PID: %d\n", total_bytes_to_receive, data.client_pid);
    printf("Server processing data from client with PID: %d\n", data.client_pid);

    // Continue reading data in chunks until we receive the entire expected byte count
    while (bytes_read < total_bytes_to_receive && (res = read(server_fifo_fd, &data, sizeof(data))) > 0)
    {
        bytes_read += res;
        printf("Server processing data from client with PID: %d\n", data.client_pid);
    }
    printf("Server successfully received all %d bytes from client with PID: %d\n", bytes_read, data.client_pid);

    // Once data receipt is complete, send an acknowledgment back to the client
    sprintf(client_fifo, client_fifo_template, data.client_pid);
    client_fifo_fd = open(client_fifo, O_WRONLY);
    if (client_fifo_fd != -1)
    {
        write(client_fifo_fd, &bytes_read, sizeof(int));
        close(client_fifo_fd);
    }

    // Clean up: Close the server FIFO and remove it
    close(server_fifo_fd);
    unlink(fifo_name);

    // Display a message indicating completion of processing for a client
    printf("Server completed processing for child process (PID: %d) of a cps_main instance and is now waiting for the next.\n", data.client_pid);
}
