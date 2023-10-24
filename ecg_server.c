/**
 * ECG server.
 * Continuously listens for ECG data transmissions from the cps_main instances.
 *
 * Author: Paul Roode
 */
#include "cps.h"

int main()
{
    printf("ECG Server started with PID: %d\n", getpid());
    while (1)
    { // Infinite loop to keep the server running
        server_function(ECG_FIFO_NAME, ECG_CLIENT_FIFO_NAME);
    }
    return 0;
}
