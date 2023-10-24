/**
 * Shared header.
 * Contains shared constants, structures, and utility functions.
 *
 * Author: Paul Roode
 */
#ifndef CPS_H
#define CPS_H

#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// FIFO names
#define ECG_FIFO_NAME "/tmp/ecg_fifo"
#define IMAGE_FIFO_NAME "/tmp/image_fifo"
#define ECG_CLIENT_FIFO_NAME "/tmp/ecg_cli_%d_fifo"
#define IMAGE_CLIENT_FIFO_NAME "/tmp/image_cli_%d_fifo"

// Buffer size
#define BUFFER_SIZE PIPE_BUF

// Data structure for passing information
typedef struct
{
    pid_t client_pid;
    char data[BUFFER_SIZE - sizeof(pid_t)];
} data_st;

// Function prototypes
void transmit_data(const char *fifo_name, const char *client_fifo_template);
void server_function(const char *fifo_name, const char *client_fifo_template);

#endif // CPS_H
