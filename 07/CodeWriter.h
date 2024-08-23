#ifndef _CODEWRITER_
#define _CODEWRITER_

#include <iostream>
#include <string>
#include <fstream>
#include "Parser.h"
#include <assert.h> 

 using namespace std;
/*
    #MAIN#
	setFileName -> informs the code writer that the translation of a new VM file started
	writeArithmetic -> writes the assembly code that is the translation of the given arithmetic
	writePushPop -> writes assembly code that is the translation of the given push or pop command

    PJ8
	close -> closes output file
	writeInit -> writes preset asm code for eq, lt, and gt jumps and where to initially start the code
	writeLabel -> assembly code that effects the label command
	writeGoto -> assembly code that effects the goto command
	writeIf -> assembly code that effects the if-goto command
	writeCall -> assembly code that effects the call command
	writeReturn -> assembly code effects the return command
	writeFunction -> assembly code effects the function command
	
	#HELPER#
	popD -> Pop top of stack to register D
	popGPR -> Pop the value stored in register D to GPR, RAM 13-15
	decSP -> Decrements stack pointer and points register A to top of stack value
	incSP -> Increments stack pointer and points register A to top of stack
	setReturn -> Sets location to return to after a jump, stores it in GPR regNum
	getReturn -> Jumps back to location stored in GPR
	push -> pushes value of *(location+index) to top of stack
	pop -> pops top of stack value to location+index address
	setOutputFileName -> sets the output file name, as the directory
	
*/

 class CodeWriter {
     public:
        CodeWriter();
        ~CodeWriter(); //CPP destructor synthax

        void setFileName(string filename);
        void writeArithmetic(string command);
        void writePushPop(VMcommand pushOrPop, string segment, int index);

        //Project 8
        void writeLabel(string label);
        void writeGoto(string label);
        void writeIf(string label);
        void writeCall(string functionName, int numArgs);
        void writeReturn(void);
        void writeFunction(string functionName, int numLocals);
        void writeInit(void);

        //helper methods
        void incSP(void);
        void decSP(void);
        void push(string loc, int index);
        void pop(string loc, int index);
        void setReturn(int regNum); //set jump location, use R15
        void getReturn(int regNum); //jump back, use R15

        //somewhat redudant helper methods?
        void popD(void);
        void popGPR(int reg);

        //Project 8 helper
        void setOutputFileName(string outname); 

    private:
        string filename_;
        int start_;
        int labelnumber_;
        ofstream outf;
        string functionName_;
};

#endif