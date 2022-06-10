/* Jan Ras
 * CSCI 4100
 * Assignment 1: kittycat
 * This program displays the contents of one or more text files given as arguments
 * If no argument is given, it displays text from the standard input stream
 */

// Header file for standard I/O functions and exit function
#include <stdio.h>
#include <stdlib.h>

// Function prototype for display_stream
void display_stream(FILE *stream);

// The main function; takes text files as command line arguments and displays them
int main(int argc, char *argv[])
{
	// Declare counter variable
	int i;
	
	// If there are no arguments given, then argc = 1
	// Pass input to display stream function
	if (argc < 2) {
		display_stream(stdin);
	}
		
	// For each argument provided
	// 1. attempt to open file
	// 2. pass file to display function
	else { 
		for (i = 1; i < argc; i++) {
		
			// Open file, readonly
			FILE *stream = fopen(argv[i], "r");
			
			// Throw error if attempt is unsuccessful
			if (stream == 0) {
				perror("error: file not found \n");
				exit(EXIT_SUCCESS);
			}
			
			//Pass file to display_stream function and close
			display_stream(stream);
			fclose(stream);
		}
	}
	return 0;
}


// This function prints characters until the EOF value is reached
void display_stream(FILE *stream)
{
	// Declare char variable
	char c = fgetc(stream);
	
	// Read and print every character from the file stream until the EOF is reached
	while (c != EOF) {
		putchar(c);
		c = fgetc(stream);
	}
}
