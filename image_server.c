/**
 * Image server.
 * Continuously listens for image data transmissions from the cps_main instances.
 *
 * Author: Paul Roode
 */
#include "cps.h"

int main()
{
    printf("Image Server started with PID: %d\n", getpid());
    while (1)
    { // Infinite loop to keep the server running
        server_function(IMAGE_FIFO_NAME, IMAGE_CLIENT_FIFO_NAME);
    }
    return 0;
}
