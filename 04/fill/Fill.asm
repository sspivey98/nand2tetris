// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

//NOTES
	//The D register is a general purpose data register whose contents are always presented to one of the two ALU data ports --> meaning it can only come from the ALU, not a register.
	//'A' register holds the address anytime that memory is to be accessed, but when not used for memory it is a general purpose register.
	//Consequently, the ALU cannot perform operations on both the A and the M values at the same time.
	//The destination field of a C-type instruction can be any subset of the A, M, and D registers but they must appear in that order.

(INIT)
	// initially set the screen pointer to point to @SCREEN
	//basically, we are setting screen_ptr as the pointer to the address of var screen
	@SCREEN
	D=A 
	@screen_ptr
	M=D
	//EQV code:
	// int *screen_ptr; 

	// create a pointer 'SCREEN_END' that points to the last address
	@8192 
	D=D+A
	@SCREEN_END
	M=D

	// set the correct color depending on keyboard input
	@KBD
	D=M
	@WHITE
	D;JEQ

	@color
	M=-1 // -1 sets all the bits in the word to 1

	@DRAW
	0;JMP

(WHITE)
	@color
	M=0

(DRAW)
	// If the pointer is pointing to the last word in the screens memory map, go back to init
	@screen_ptr
	D=M
	@SCREEN_END
	D=D-M
	@INIT
	D;JEQ

	// change the memory pointed at by the screen pointer to the correct color
	@color
	D=M
	@screen_ptr
	A=M
	M=D

	// move the pointer to the next word in the screen's memory map.
	@screen_ptr
	M=M+1

	// exit loop --> I don't agree with this
	@KBD
	D=M
	@WHITE
	D;JEQ

	@DRAW
	0;JMP