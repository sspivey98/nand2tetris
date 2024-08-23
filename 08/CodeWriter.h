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
	
	writeInit -> writes preset asm code for eq, lt, and gt jumps and where to initially start the code
	writeLabel -> assembly code that effects the label command
	writeGoto -> assembly code that effects the goto command
	writeIf -> assembly code that effects the if-goto command
	writeCall -> assembly code that effects the call command
	writeReturn -> assembly code effects the return command
	writeFunction -> assembly code effects the function command
	
	#HELPER#
	push -> pushes value of *(location+index) to top of stack
	pop -> pops top of stack value to location+index address
	setFileName -> sets the output file name, as the directory
    close -> closes output file
    normalizeSegment -> captialize arguments for normalization
	
*/

 class CodeWriter {
     public:
        CodeWriter();
        ~CodeWriter(); //CPP destructor synthax

        void close(void);

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
        string normalizeSegment(string s);

    private:
        string filename_;
        int start_;
        int labelnumber_;
        ofstream outf;
        string functionName_;
};

#endif