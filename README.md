# client-server-project
An implementation of a client and server text-based application that uses synchronous TCP stream sockets.

## User Guide

This is a one-page user guide that provides a step-by-step process to guide a user in running the program.

To efficiently run the program, the following instructions will be helpful to the user:

1.	Open a terminal
2.	Change the directory to the server 
3.	Enter “make” on the command line
4.	Enter “./server” on the command line
5.	Open another terminal
6.	Change the directory to the client
7.	Enter “make” on the command line
8.	Enter “./client” on the command line
9.	Ensure the client is connected to the server
10.	If the client is not connected, wait for 30 seconds and try step 8 again
11.	A menu will be displayed on the screen and follow the instruction to get expected results
12.	Use Ctrl+C to quit the server thread at anytime

The main features of the program are:

1.	Display IP Address and Student Info
2.	Display 5 random numbers
3.	Display System information
4.	Display File Names in the upload directory
5.	Exit the program

What does not work:

The method that displays file names is not working and will be included in a later iteration.
The program is also not enhanced to handle any error that may occur when a user tries to display file names.
