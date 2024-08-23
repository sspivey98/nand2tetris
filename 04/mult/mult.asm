// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.

// PSUEDO CODE:

// r2 = 0;
// 
// while(r1 > 0)
// {
//   r1 = r1 - 1;
//   r2 = r2 + r0;
// }

@R2
M=0

@R0
D=M
@END
D;JEQ //make sure R0 isn't 0

@R1
D=M
@END
D;JEQ //same for R1


(LOOP)
    // Check loop condition. Quit if R1 <= 0. 
    @R1
    D=M
    @END
    D;JLE

    @R0
    D=M

    // Update loop condition.
    @R1
    M=M-1

    // Do the repeated addition.
    @R2
    M=M+D 

    @LOOP
    0;JMP

(END)   
    @END
    0;JMP
