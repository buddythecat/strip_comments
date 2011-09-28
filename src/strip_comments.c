/*
 ============================================================================
 Name        : strip_comments.c
 Author      : Rich "Dances With Caterpillars" Tufano
 Headers	 : STDIO, STDBOOL, STRING
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
URL			: This source code is can be found on git
				https://github.com/buddythecat/strip_comments
Copyright 	: 2011, Rich "Dances With Caterpillars" Tufano (not that anyone cares)
 ============================================================================
 */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#define BUFFERSIZE 50			/* definition for  the size of the output buffer */


char outputBuffer[BUFFERSIZE];	/* the output for the program, as a string */
int bufferPos;					/* the current position in output, 0-based */
int current;					/* current keeps track of the current character being parsed. */
int previous = 0; 				/* previous keeps track of the last character being parsed (if needed). */
bool write = true; 				/* write flags whether or not the parser should write out the current characters */
bool inLineComment = false;		/* write flags for whether or not the parser is in a line-style C++ comment */
void checkChars(); 				/* function prototype for checkChars(), defined below */
void processChar(); 				/* function prototype for processChar(), defined below */
void toOutputBuffer(char toAdd);/* function prototype for toOutputBuffer(char toAdd), defined below */
void writeBufferOut();				/* function prototype for writeBufferOut(), defined below.

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
	writeBufferOut();
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
 * 	the function will call the processChar() method.
 * 	- EXTRA CREDIT: I've added catching for the single-line, C++ style comments
 ============================================================================
 */
void checkChars(){
	/*	this is the case for the beginning of a block comment */
	if (previous == '/' && current == '*'){
		/* Entered into a comment, flag write as false. Clear previous */
		write = false;
		previous = 0;
	}
	/*	this is the case for the end of a block comment */
	else if (previous == '*' && current == '/'){
		/*Exiting a comment, flag write as true. Clear previous */
		write = true;
		previous = 0;
	}
	/*	this is the special case for the C++ line comment */
	else if (previous == '/' && current == '/'){
		/* 	activate the inlineComment flag, to disable writing */
		inLineComment = true;
	}
	/*	this is a watch condition.  the next character may be the beginning of a case. */
	else if ( (current == '*' || current == '/') && previous == 0){
		/* This is a watched character.  Set it to previous and check the next character(as long as previous is null) */
		previous = current;
	}
	/* 	None of the watched characters were found.  Call the write function */
	else{
		/*
		 * This if statement is to unflag the inLineComment if we've reached the end of a commented line.
		 * 0x0A is the Hexadecimal ASCII code for the newline code (the end of the line).  Therefore,
		 * if the parser encounters this the inLineComment flag should be set to false despite it's
		 * initial state.
		 */
		if(current == 0x0A)
			inLineComment = false;
		// send the character to be written
		processChar();
	}
}
/*
 ============================================================================
 * processChar()
 * Description	: this method is used to send characters to the output buffer.
 *  The condition required to write out is that the 'write' flag must be true,
 *  and the 'inLineComment' flag must be false.  (Don't write if we're in a block
 *  comment or a line comment). If the condition is met, it will send previous
 * 	(if previous isn't null), and it will the current character to the string
 * 	buffer. If 'write' is false or 'inLineComment' is true, the method
 * 	will clear previous for good measure and complete.
 ============================================================================
 */
void processChar(){
	if(write && !inLineComment){
		/* the loop is currently not in a comment -- write out */
		if(previous){
			/* previous is not null, write out previous and clear previous */
			toOutputBuffer((char)previous);
			previous = 0;
		}
		/* write out current */
		toOutputBuffer((char)current);
	}
	else{
		/*
		 * clear previous for good measure, in case we could not write, but there was a flagged
		 * character in previous, but none in current.
		 */
		previous = 0;
	}
}
/*
 ============================================================================
 * toOutputBuffer(char toAdd) -
 * Description	: this method adds a character to the output buffer.  The
 * 	output buffer is emptied every 50 characters by printing the contents out.
 * 	when the buffer becomes full, printf() is called with the char array
 * 	(note: this is not a null terminated terminated string, but just an array of
 * 	characters.  This is because I'd rather the output not contain null characters,
 * 	because the original input didn't contain these null chars.) passed to it.
 * 	after the buffer is printed out, the bufferSize variable is reset to zero,
 * 	and the char waiting to be added to the buffer is added to the front
 * 	position. If the buffer isn't full, the char is added to the buffer at the rear.
 * Parameters:
 * 	[char] toAdd - the character being added to the buffer.
 ============================================================================
 */
void toOutputBuffer(char toAdd){
	//check to make sure the buffer has room for us to write to.
	if(bufferPos<BUFFERSIZE){
		//if it does, write toAdd to the next position in the buffer, and increment the buffer position
		outputBuffer[bufferPos] = toAdd;
		bufferPos++;
	}
	else{
		//if there is no more space in the buffer, write the output to clear out the buffer.
		writeBufferOut();
		//loop through the current buffer and clear it out (set each byte to null).
		int i;
		for(i=0;i<BUFFERSIZE; i++){
			outputBuffer[i] = 0;
		}
		//reset the buffer position counter.
		bufferPos=0;
		//add the character to the first position in the buffer and...
		outputBuffer[bufferPos]=toAdd;
		//increment the buffer position
		bufferPos++;
	}
}
/**
  ============================================================================
 * writeBufferOut() -
 * Description	: The writeBufferOut() function takes the program's buffer, and
 * 	pipes it out, whether it's to the console, or to a file.  It achieves this
 * 	by using the printf() function, formatted for a string, passing the
 * 	outputBuffer character array.
  ============================================================================
 */
void writeBufferOut(){
	//use printf() to output the contents of the outputBuffer as a string.
	printf("%s",outputBuffer);
}
