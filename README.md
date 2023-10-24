# Cyber-Physical System (CPS) Emulator

Emulates a healthcare monitoring system, facilitating ECG and image data transmissions from a local clinic to an edge-cloud server.

## How to Run

1. Compile and run the programs:

    ```
    sh compile_and_run.sh
    ```

    This script will compile the necessary programs and then automatically open three separate terminals:

    - one running the ECG server;
    - one running the Image server; and
    - one that has already executed the main CPS program.

2. To emulate multiple clinics, simply run the following command in the original terminal or any new terminal:

    ```
    ./cps_main
    ```

**Remember:**

-   Ensure `xterm` is installed on your system for the script to work as expected.
-   The servers will continuously listen for new data transmissions, allowing you to run multiple instances of `cps_main`.

## Author

Paul Roode
