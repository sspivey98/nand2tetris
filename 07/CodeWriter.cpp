#include "CodeWriter.h"
#include <string>
#include <iostream>
#include <fstream>
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

void CodeWriter::setFileName(string filename) {
	filename_ = filename;
	if(start_==0) {
		filename = filename + ".asm";
		outf.open(filename.c_str());
		writeInit();
	}
	if (!outf.is_open())
		cerr << "Could not open output file " << filename_ << ".\n";
}

void CodeWriter::writeArithmetic(string command) {
    //All 9 commands as if Blocks
    if (command == "add") {
       outf << "//Add" << endl;
       popD();
       decSP();
       outf << "M=D+M" << endl;
       incSP(); 
    }
    else if (command == "sub") {
       popD();
       decSP();
       outf << "M=M-D" << endl;
       incSP(); 
    }
    else if (command == "neg") {
        decSP();
        outf << "M=-M" << endl;
        incSP();
    }
    else if (command == "not") {
        decSP();
        outf << "M=!M" << endl;
        incSP();
    }
    else if (command == "and") {
       popD();
       decSP();
       outf << "M=D&M" << endl;
       incSP(); 
    }
    else if (command == "or") {
       popD();
       decSP();
       outf << "M=D|M" << endl;
       incSP(); 
    }
    //-------------------------------
    else if (command == "gt") {
        setReturn(15);
        popD();
        decSP();
        outf << "D=D-M" << endl << "M=0" << endl;
        incSP();
        outf << "@MAKETRUE" << endl << "D;JLT" << endl;
        outf << "(RETURN" << labelnumber_ << ")" << endl;
        labelnumber_++;
    }
    else if (command == "lt") {
        setReturn(15);
        popD();
        decSP();
        outf << "D=D-M" << endl << "M=0" << endl;
        incSP();
        outf << "@MAKETRUE" << endl << "D;JGT" << endl;
        outf << "(RETURN" << labelnumber_ << ")" << endl;
        labelnumber_++;
    }
    else if (command == "eq") {
        setReturn(15);
        popD();
        decSP();
        outf << "D=D-M" << endl << "M=0" << endl;
        incSP();
        outf << "@MAKETRUE" << endl << "D;JEQ" << endl;
        outf << "(RETURN" << labelnumber_ << ")" << endl;
        labelnumber_++;
    }

}

void CodeWriter::writePushPop(VMcommand pushOrPop, string segment, int index) {
    //local, argument, this, that, pointer, static(don't push, own stuff), temp, constant(don't push) 
    if (pushOrPop == C_PUSH) {
        outf << "//Push " << index << endl;
        if (segment == "local") {
            push("LCL", index);
        }
        if (segment == "this") {
            push("THIS", index);
        }
        if (segment == "that") {
            push("THAT", index);
        }
        if (segment == "argument") {
            push("ARG", index);
        }
        //-------------------------
        if (segment == "pointer") {
            push("3", index);
        }
        if (segment == "temp") {
            push("5", index);
        }
        //-------------------------
        if (segment == "constant") {
            outf << "@" << index << endl << "D=A" << endl;
            outf << "@SP" << endl << "A=M" << endl << "M=D" << endl;
            incSP();
        }
        if (segment == "static") {
            outf << "@" << filename_ << "." <<  index << endl << "D=M" << endl;
            outf << "@SP" << endl << "A=M" << endl << "M=D" << endl;
            outf << "@SP" << endl << "M=M+1" << endl;
        }

    }
    else if (pushOrPop == C_POP) {
        if (segment == "local") {
            pop("LCL", index);
        }
        if (segment == "this") {
            pop("THIS", index);
        }
        if (segment == "that") {
            pop("THAT", index);
        }
        if (segment == "argument") {
            pop("ARG", index);
        }
        //-------------------------
        if (segment == "pointer") {
            pop("3", index);
        }
        if (segment == "temp") {
            pop("5", index);
        }
        //-------------------------
        if (segment == "static") {
            outf << "@" << filename_ << "." <<  index << endl << "D=A" << endl;
            outf << "@R13" << endl << "M=D" << endl;
            outf << "@SP" << endl << "M=M-1" << endl << "A=M" << "D=M" << endl;
            outf << "@R13" << endl << "A=M" << endl << "M=D" << endl;
        }
    }
}
void CodeWriter::incSP() {
    outf << "@SP" << endl << "M=M+1" << endl; //not AM=M+1?
}

void CodeWriter::decSP() {
    outf << "@SP" << endl << "AM=M-1" << endl;
}

void CodeWriter::push(string loc, int index) {
    if (loc == "3" || loc == "5") {
        outf << "@" << loc << endl << "D=A" << endl;

    }
    else {
        outf << "@" << loc << endl << "D=M" << endl;
    }
    
    outf << "@" << index << endl << "A=D+A" << endl;
    outf << "D=M" << endl;
    outf << "@SP" << endl << "A=M" << endl << "M=D" << endl;

}
void CodeWriter::pop(string loc, int index) {
    //temp = ram[5] pointer = ram[3]
    if (loc == "3" || loc == "5") {
        outf << "@" << loc << endl << "D=A" << endl;
    }
    else {
        outf << "@" << loc << endl << "D=M" << endl; //maybe incorrect
    }

    outf << "@" << index << endl << "D=D+A" << endl;
    popGPR(13);
    decSP();
    outf << "D=M" << endl;
    outf << "@R13" << endl << "A=M" << endl << "M=D" << endl;
    
}
void CodeWriter::popD(void) {
    outf << "@SP" << endl << "AM=M-1" << "D=M" << endl << "M=0" << endl; 
}
void CodeWriter::popGPR(int reg) {
    assert(reg >= 13 && reg <= 15);
    outf << "@" << reg << endl << "M=D" << endl;
}

void CodeWriter::writeInit(void) {
    outf << "@256" << endl << "D=A" << endl;
    outf << "@SP" << endl << "M=D" << endl;

    outf << "@START" << start_ << endl << "0;JMP" << endl << "(MAKETRUE)" << endl;
    decSP();
    outf << "M=-1" << endl;
    incSP();
    getReturn(15);
    outf << "(START" << start_ << ")" << endl;
    writeCall("Sys.init", 0);
    start_++;
}

void CodeWriter::setReturn(int regNum) { //use R15
    assert(regNum >= 13 && regNum <= 15);
    outf << "@RETURN" << labelnumber_ << endl;
    outf << "D=A" << endl << "@R" << regNum << endl << "M=D" << endl;
}
void CodeWriter::getReturn(int regNum) {
    assert(regNum >= 13 && regNum <= 15);
    outf << "@R" << regNum << endl << "A=M" << endl << "0;JMP" << endl;
}

void CodeWriter::writeLabel(string label) {
    outf << "(" << functionName_ << "$" << label << ")" << endl;
}

void CodeWriter::writeGoto(string label) {
    if (label != functionName_) {
        outf << "@" << functionName_ << "$" << label << endl;
    }
    else {
        outf << "@" << label << endl;
    }
    outf << "0;JMP" << endl;
}

void CodeWriter::writeIf(string label) {
    popD();
    outf << "@" << functionName_ << "$" << label << endl;
    outf << "D;JNE" << endl;
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
	outf << "@RETURN" << labelnumber_ << endl << "D=A" << endl;
    outf << "@SP" << endl << "A=M" << endl << "M=D" << endl;
	incSP();
	
	outf << "@LCL" << endl << "D=M" << endl;
    outf << "@SP" << endl << "A=M" << endl << "M=D" << endl;
	incSP();
	
	outf << "@ARG" << endl << "D=M" << endl; 
    outf << "@SP" << endl << "A=M" << endl << "M=D" << endl;
	incSP();
	
	outf << "@THIS" << endl << "D=M" << endl; 
    outf << "@SP" << endl <<"A=M" << endl << "M=D" << endl;
	incSP();
	
	outf << "@THAT" << endl << "D=M" << endl;
    outf << "@SP" << endl <<"A=M" << endl << "M=D" << endl;
	incSP();
	
	outf << "@" << numArgs << endl << "D=A" << endl;
    outf << "@SP" << endl << "D=M-D" << endl;
    outf << "@5" << endl << "D=D-A" << endl;
    outf << "@ARG" << endl << "M=D" << endl;

	outf << "@SP" << endl << "D=M" << endl;
    outf << "@LCL" << endl << "M=D" << endl;

	outf << "@" << functionName << endl;
    outf << "0;JMP" << endl;

	outf << "(RETURN" << labelnumber_ << ")" << endl;
	labelnumber_++;
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
	outf << "@LCL" << endl << "D=M" << endl;
    outf << "@FRAME" << endl << "M=D" << endl;
	
	outf << "@5" << endl << "D=A" << endl;
    outf << "@FRAME" << endl << "A=M-D" << endl << "D=M" << endl;
    outf << "@RET" << endl << "M=D" << endl;
	popD(); 

	outf << "@ARG" << endl << "A=M" << endl << "M=D" << endl;
	
	outf << "@ARG" << endl << "D=M+1" << endl;
    outf << "@SP" << endl << "M=D" << endl;
	
	outf << "@FRAME" << endl << "A=M-1" << endl << "D=M" << endl;
    outf << "@THAT" << endl << "M=D" << endl;
	
	outf << "@2" << endl << "D=A" << endl;
    outf << "@FRAME" << endl << "A=M-D" << endl << "D=M" << endl;
    outf << "@THIS" << endl << "M=D" << endl;
	
	outf << "@3" << endl << "D=A" << endl;
    outf << "@FRAME" << endl << "A=M-D" << endl << "D=M" << endl;
    outf << "@ARG" << endl << "M=D" << endl;
	
	outf << "@4" << endl << "D=A" << endl;
    outf << "@FRAME" << endl << "A=M-D" << endl << "D=M" << endl;
    outf << "@LCL" << endl << "M=D" << endl;
	
	outf << "@RET" << endl << "A=M" << endl << "0;JMP" << endl;
}

void CodeWriter::writeFunction(string functionName, int numLocals) {
    functionName_ = functionName;
    outf << "(" << functionName << ")" << endl;
    for (int i = 0; i < numLocals; i++) {
        writePushPop(C_PUSH, "constant", 0);
    }
}

void CodeWriter::setOutputFileName(string outname) {
    filename_= outname;
	outname = outname+".asm";
	outf.open(outname.c_str());
	writeInit();
	start_++;
}

