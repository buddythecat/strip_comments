/*
 ============================================================================
 Name        : strip_comments.c
 Author      : Rich Tufano
 Description : This program is designed to parse input and remove any C style
				 comments from the input itself.  It does this by reading the
				 input character by character.  When it encounters a / or *
				 character, it sets the previous character to to the current.
				 On the next step through, if the current character is a
				 * (following the /), the program stops writing the characters
				 out.  if the current character is / (following the *), the
				 program will then start writing out again.  The flagging of
				 whether to write out or not is handled by the boolean write.
				 The current character and previous character are held by the
				 variables 'current' and 'previous' respectively.
 ============================================================================
 */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

char output[1000];	/* the output for the program, as a string */
int outPos;			/* the current position in output, 0-based */
int current;		/* current keeps track of the current character being parsed. */
int previous = 0; 	/* previous keeps track of the last character being parsed (if needed). */
bool write = true; 	/* write flags weather or not the parser should write out the current characters */
void checkChars(); 	/* function prototype for checkChars(), defined below */
void writeChars(); 	/* function prototype for writeChars(), defined below */
void addToString(char toAdd);
void writeOutput();

/*
 ============================================================================
 * main()
 * 	Description	: the main function is the applications main loop.  This is what parses
 * 		the input.  The loop continues until the EOF (End of File) is read,
 * 		and each time a character is read, this function calls the checkChars()
 * 		function.
 * 	Returns		: 0 upon clean completion
 ============================================================================
 */
int main(){
	/* Get the character and start the main loop */
	while( ( current = getchar() ) != EOF){
		checkChars(&current, &previous);
	}
	writeOutput();
	printf("\n");
	/* return 0 on completion */
	return 0;
}

/*
 ============================================================================
 * checkChars()
 * Description	: this method is designed to process the characters being read,
 * 	and	to decide whether or not the parser is within a comment. If the function
 * 	detects the start or the end of a comment, it will flag the boolean 'write'
 * 	false or true respectively.  If no comment start- or end-block is found,
 * 	the function will call the writeChars() method.
 ============================================================================
 */
void checkChars(){
	if (previous == '/' && current == '*'){
		/* Entered into a comment, flag write as false. Clear previous */
		write = false;
		previous = 0;
	}
	else if (previous == '*' && current == '/'){
		/*Exiting a comment, flag write as true. Clear previous */
		write = true;
		previous = 0;
	}
	else if ( (current == '*' || current == '/') && previous == 0){
		/* This is a watched character.  Set it to previous and check the next character(as long as previous is null) */
		previous = current;
	}
	else{
		/* None of the watched characters were found.  Call the write function */
		writeChars();
	}
}
/*
 ============================================================================
 * writeChars()
 * Description	: this method is used to write characters out.  the method
 * 	first checks whether 'write' is true.  If it is, it will write out previous
 * 	(if previous isn't null), and it will write out current.
 * 	If 'write' is false, the method will clear previous for good measure and complete.
 ============================================================================
 */
void writeChars(){
	if(write){
		/* the loop is currently not in a comment -- write out */
		if(previous){
			/* previous is not null, write out previous and clear previous */
			addToString((char)previous);
			previous = 0;
		}
		/* write out current */
		addToString((char)current);
	}
	else{
		/*
		 * clear previous for good measure, in case we could not write, but there was a flagged
		 * character in previous, but none in current.
		 */
		previous = 0;
	}
}

void addToString(char toAdd){
	if(outPos<1000){
		output[outPos] = toAdd;
		outPos++;
	}
	else{
		writeOutput();
		int i=0;
		while(i<1000){
			output[i] = 0;
			i++;
		}
		outPos=0;
		output[outPos]=toAdd;
	}
}

void writeOutput(){
	printf("%s",output);
}
