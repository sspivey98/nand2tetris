#include "CodeWriter.h"
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <assert.h> 

using namespace std;

CodeWriter::CodeWriter() {
    labelnumber_ = 0;
    start_ = 0;
}

CodeWriter::~CodeWriter() {
    if (outf.is_open()) {
        outf.close();
    }
}

void CodeWriter::close() {
    outf.close();
}

void CodeWriter::setFileName(string filename) {
	filename_ = filename;
    filename = filename + ".asm";
	outf.open(filename.c_str());
}

std::string CodeWriter::normalizeSegment(string s) {
	if (s == "local")
		return "LCL";
	if (s == "argument")
		return "ARG";
	if (s == "this")
		return "THIS";
	if (s == "that")
		return "THAT";

	return s;
}

void CodeWriter::writeArithmetic(string command) {
    //All 9 commands as if Blocks
    outf << "//" << command << endl;

    outf << "@SP" << endl;
	outf << "A=M-1" << endl;		// A -> y

    if (command.substr(0,3) == "neg") {
        outf << "M=-M" << endl;
        return;
    }
    if (command.substr(0,3) == "not") {
        outf << "M=!M" << endl;
        return;
    }

    outf << "D=M" << endl;
    outf << "A=A-1" << endl;
  
    if (command.substr(0,3) == "add") {
       outf << "M=M+D" << endl;
    }
    else if (command.substr(0,3) == "sub") {
       outf << "M=M-D" << endl;
    }
    else if (command.substr(0,3) == "and") {
       outf << "M=D&M" << endl;
    }
    else if (command.substr(0,2) == "or") {
       outf << "M=D|M" << endl;  
    }
    //-------------------------------
    else if (command.substr(0,2) == "eq") {
		outf << "D=M-D" << endl;	// D = x-y

		outf << "@eq_equals" << labelnumber_ << endl;
		outf << "D;JEQ" << endl;
		outf << "D=0"	<< endl;	// D = false
		outf << "@eq_done" << labelnumber_ << endl;
		outf << "0;JMP" << endl;

		outf << "(eq_equals" << labelnumber_ << ")" << endl;
		outf << "D=-1" << endl;		// D = true

		outf << "(eq_done" << labelnumber_ << ")" << endl;
		outf << "@SP" << endl;
		outf << "A=M-1" << endl;	// A = [SP]-1
		outf << "A=A-1" << endl; 	// A--
		outf << "M=D" << endl;		// [SP]-2 <- result
    }
    else if (command.substr(0,2) == "gt") {
		outf << "D=M-D" << endl;	// D = x-y

		outf << "@gt_true" << labelnumber_ << endl;
		outf << "D;JGT" << endl;    // x-y > 0 => x > y => true
		outf << "D=0"	<< endl;	// D = false
		outf << "@gt_done" << labelnumber_<<  endl;
		outf << "0;JMP" << endl;

		outf << "(gt_true" << labelnumber_ << ")" << endl;
		outf << "D=-1" << endl;		// D = true

		outf << "(gt_done" << labelnumber_ << ")" << endl;
		outf << "@SP" << endl;
		outf << "A=M-1" << endl;
		outf << "A=A-1" << endl; 	// A = [SP] - 2
		outf << "M=D" << endl;		// [SP]-2 <- result
    }
    else if (command.substr(0,2) == "lt") {
		outf << "D=M-D" << endl;	// D = x-y

		outf << "@lt_true" << labelnumber_ << endl;
		outf << "D;JLT" << endl;    // x-y < 0 => x < y => true
		outf << "D=0"	<< endl;	// D = false
		outf << "@lt_done" << labelnumber_<<  endl;
		outf << "0;JMP" << endl;

		outf << "(lt_true" << labelnumber_ << ")" << endl;
		outf << "D=-1" << endl;		// D = true

		outf << "(lt_done" << labelnumber_ << ")" << endl;
		outf << "@SP" << endl;
		outf << "A=M-1" << endl;
		outf << "A=A-1" << endl; 	// A = [SP]-2
		outf << "M=D" << endl;		// [SP]-2 <- result
    }

    outf << "@SP" << endl;
	outf << "M=M-1" << endl;	

	labelnumber_++;

}

void CodeWriter::writePushPop(VMcommand pushOrPop, string segment, int index) {
    //local, argument, this, that, pointer, static(don't push, own stuff), temp, constant(don't push) 
    segment = normalizeSegment(segment); 
    if (pushOrPop == C_PUSH) {
        outf << "//Push " << index << endl;

        if (segment.substr(0,8) == "constant") {
            outf << "@" << index << endl;
            outf << "D=A" << endl;			// D = constant.
	    }
        // TEMP segment: indexed access to the pointer.
        else if (segment.substr(0,4) == "temp") {
            outf << "@R5" << endl;	
            outf << "D=A" << endl;
            outf << "@" << index << endl; 	// A = offset
            outf << "A=D+A" << endl;		// A = base+offset
            outf << "D=M" << endl;			// D = element to push			
        }
        // POINTER segment selects between THIS and THAT (no dereferences).
        else if (segment.substr(0,7) == "pointer") {
            if (index == 0)
                outf << "@THIS" << endl;
            else
                outf << "@THAT" << endl;

            outf << "D=M" << endl;
        }
        // Static variables.
        else if (segment.substr(0,6) == "static") {
            outf << "@" << filename_ << "." << index << endl; // xxx.i
            outf << "D=M" << endl;
        }
        else {
            //Make helper method for segment to uppercase (argument should be ARG)
            outf << "@" << segment << endl;	
            outf << "D=M" << endl;			// D = segment base
            outf << "@" << index << endl; 	// A = offset
            outf << "A=D+A" << endl;		// A = base+offset
            outf << "D=M" << endl;			// D = element to push
        }
        // D = constant/element to push
        outf << "@SP" << endl;				// A -> top of stack
        outf << "A=M" << endl;			
        outf << "M=D" << endl;				// Writing the element
        outf << "@SP" << endl;
        outf << "M=M+1" << endl;			// SP++

    }
    else if (pushOrPop == C_POP) {
        outf << "//Pop " << index << endl;
        outf << "@SP" << endl;
        outf << "A=M-1" << endl;		// A -> element
        outf << "D=M" << endl;			// Extracting element to D
        outf << "@R13"	<< endl;		// R13: temp. register
        outf << "M=D" << endl;			// R13 = element

        // TEMP segment: fixed base at 0x5.
        if (segment.substr(0,4) == "temp") {
            outf << "@R5" << endl;			// A = 0x5
            outf << "D=A" << endl;
            outf << "@" << index << endl; 	// A = offset
            outf << "D=D+A" << endl;		// D = base+offset		
        }
        // POINTER segment selects between THIS and THAT (no dereferences).
        else if (segment.substr(0,7) == "pointer") {
            if (index == 0)
                outf << "@THIS" << endl;
            else
                outf << "@THAT" << endl;
            
            outf << "D=A" << endl;
        }
        else if (segment.substr(0,6) == "static") {
            outf << "@" << filename_ << "." << index << endl; // xxx.i
            outf << "D=A" << endl;
        }
        // All other segments have variable bases, so we need to dereference them.
        else {
            //Make helper method for segment to uppercase (argument should be ARG)
            outf << "@" << segment << endl; // A -> segment base
            outf << "D=M" << endl;			// D = segment base
            outf << "@" << index << endl;	// A = offset
            outf << "D=D+A" << endl;		// D = base+offset
        }
        
        outf << "@R14" << endl;
        outf << "M=D" << endl;			// R14 = base+offset

        outf << "@R13" << endl;
        outf << "D=M" << endl;			// D = element
        outf << "@R14" << endl;
        outf << "A=M" << endl;			// A -> base + index
        outf << "M=D" << endl;

        outf << "@SP" << endl;			// SP--
        outf << "M=M-1" << endl;
    }
}

void CodeWriter::writeInit(void) {
    // Set SP to 256
    outf << "//System Init" << endl;
    outf << "@256" << endl << "D=A" << endl;
    outf << "@SP" << endl << "M=D" << endl;
	writeCall("Sys.init", 0);
}

void CodeWriter::writeLabel(string label) {
    label.erase(std::remove(label.begin(), label.end(), '\r'), label.end()); //remove carriage return char
    outf << "(" << functionName_ << label << ")" << endl;
}

void CodeWriter::writeGoto(string label) {
	outf << "@" << label << endl;
    outf << "0;JMP" << endl;
}

void CodeWriter::writeIf(string label) {
    outf << "@SP" << endl;		// Pop element to D
	outf << "M=M-1" << endl;	// SP--
	outf << "A=M" << endl;		// A -> if guard
	outf << "D=M" << endl;		// D = if guard
	outf << "@" << label << endl;	// A -> destination
	outf << "D;JNE" << endl;	// Jump to s if guard != 0
}

void CodeWriter::writeCall(string functionName, int numArgs) {
    /*
    Push return value from label declared below onto stack
    Push LCL of calling function onto stack
    Push ARG of calling function onto stack
    Push THIS of calling function onto stack
    Push THAT of calling function onto stack
    reposition ARG
    reposition LCL
    transfer control
    declare label for return-address
    */

	int labelId = labelnumber_++;
	string retAddressLabel("retPoint");
	retAddressLabel.append(functionName);
	retAddressLabel.append(to_string(labelId));

	outf << "@" << retAddressLabel << endl;
	outf << "D=A" << endl;		// D = return address
	outf << "@SP" << endl;
	outf << "A=M" << endl;		// A -> top of stack
	outf << "M=D" << endl;		// Pushing element
	outf << "@SP" << endl;		// SP++
	outf << "M=M+1" << endl;

    //---------------------
    const std::string registers[4] = {"LCL", "ARG", "THIS", "THAT"};
    for (unsigned int i = 0; i < 4; i++) {
        outf << "@" << registers[i] << endl;
        outf << "D=M" << endl;		// D = value to push
        outf << "@SP" << endl;
        outf << "A=M" << endl;		// A -> top of stack
        outf << "M=D" << endl;		// Pushing element
        outf << "@SP" << endl;		// SP++
        outf << "M=M+1" << endl;
    }
    //---------------------

	// ARG = SP-args-5
	outf << "@SP" << endl;
	outf << "D=M" << endl;			// D = SP
	outf << "@" << numArgs << endl;	// A = args
	outf << "D=D-A" << endl;		// D = SP-args
	outf << "@5" << endl;
	outf << "D=D-A" << endl;		// D = SP-args-5
	outf << "@ARG" << endl;
	outf << "M=D" << endl;			// ARG = SP-args-5

	// LCL = SP
	outf << "@SP" << endl;
	outf << "D=M" << endl;			// D = SP
	outf << "@LCL" << endl;
	outf << "M=D" << endl;			// LCL = SP

	writeGoto(functionName);
	writeLabel(retAddressLabel);
}
void CodeWriter::writeReturn(void) {
    /*
    FRAME is a temporary variable
    Put the return-address in a temp variable
    Reposition the return value for the caller; pop return value to register D
    Restore SP of the caller
    Restore THAT of the caller
    Restore THIS of the caller
    Restore ARG of the caller
    Restore LCL of the caller
    go to return-address
    */
	outf << "@LCL" << endl;
	outf << "D=M" << endl;		// D = LCL
	outf << "@R15" << endl;		// Temp. var. FRAME
	outf << "M=D" << endl;		// FRAME = LCL

	outf << "@5" << endl;
	outf << "A=D-A" << endl;	// A -> FRAME-5 
	outf << "D=M" << endl;		// D = [FRAME-5]
	outf << "@R16" << endl;		// Temp. var. RET
	outf << "M=D" << endl;		// RET = FRAME-5

	writePushPop(C_POP, "ARG", 0);	// Result into arg[0] for the caller.

	outf << "@ARG" << endl;
	outf << "D=M+1" << endl;	// D = ARG+1
	outf << "@SP" << endl;
	outf << "M=D" << endl;		// SP = ARG+1 (SP restored)

	// Restoring registers:
	const std::string registers[4] = {"THAT", "THIS", "ARG", "LCL"};
	for (unsigned int i = 0; i < 4; i++) {
		outf << "@R15" << endl;
		outf << "D=M" << endl;		// D = FRAME
		outf << "A=D-1" << endl;	// A-> frame-1
		outf << "D=M" << endl;		// D = [frame-1]
		outf << "@" << registers[i] << endl;
		outf << "M=D" << endl;		// THAT = [frame-1]
		outf << "@R15" << endl;		// FRAME--
		outf << "M=M-1" << endl;
	}

	// Goto RET.
	outf << "@R16" << endl;
	outf << "A=M" << endl;			// A -> RET address
	outf << "0;JMP" << endl;
}

void CodeWriter::writeFunction(string functionName, int numLocals) {
	writeLabel(functionName);
	while (numLocals > 0) {
		writePushPop(C_PUSH, "constant", 0);
		numLocals--;
	}
}


