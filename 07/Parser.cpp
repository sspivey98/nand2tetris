#include "Parser.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

Parser::Parser() {
    filename_="";
}

Parser::~Parser() {
    if (inf.is_open()) {
        inf.close();
    }
}

bool Parser::hasMoreCommands(void) {
    return !inf.eof();
}
void Parser::advance(void) {
	if (hasMoreCommands()) {
		getline(inf, currentCommand_);
		while (((currentCommand_.substr(0, 2) == "//") || (currentCommand_.substr(0, 1) == "")) && hasMoreCommands())
			getline(inf, currentCommand_);
	}
}

VMcommand Parser::commandType(void) {
    if ((currentCommand_.substr(0, 3) == "add") || (currentCommand_.substr(0, 3) == "sub") || (currentCommand_.substr(0, 3) == "neg") || (currentCommand_.substr(0, 3) == "and") || (currentCommand_.substr(0, 3) == "not") || (currentCommand_.substr(0, 2) == "eq") || (currentCommand_.substr(0, 2) == "gt") || (currentCommand_.substr(0, 2) == "lt") || (currentCommand_.substr(0, 2) == "or")) {
		return C_ARITHMETIC;    
    }
    else if (currentCommand_.substr(0,4) == "push") {
        return C_PUSH;
    }
    else if (currentCommand_.substr(0,3) == "pop") {
        return C_POP;
    }
    else if (currentCommand_.substr(0,5) == "label") {
        return C_LABEL;        
    }
    else if (currentCommand_.substr(0,4) == "goto") {
        return C_GOTO;
    }
    else if (currentCommand_.substr(0,2) == "if") {
        return C_IF;
    }
    else if (currentCommand_.substr(0,8) == "function") { //project 8 stuff
        return C_FUNCTION;
    }
    else if (currentCommand_.substr(0,4) == "call") {
        return C_CALL;
    }
    else if (currentCommand_.substr(0,6) == "return") {
        return C_RETURN;
    }
}
        
string Parser::arg1(void) {
    VMcommand ct = commandType();
    if (ct == C_ARITHMETIC) {
        int op1 = currentCommand_.find(" ", 0);
        string arg = currentCommand_.substr(0, op1);
        return arg;
    }
    else if (ct != C_RETURN) {
        int op1 = currentCommand_.find(" ", 0);
        int op2 = currentCommand_.find(" ", op1 + 1);
        string arg = currentCommand_.substr(op1 + 1, op2 - op1 - 1); //effectively splicing the string at the space to next space
        return arg;
    }
}
int Parser::arg2(void) {
    VMcommand ct = commandType();
    if (ct == C_PUSH || ct == C_POP || ct == C_FUNCTION || ct == C_CALL) {
        int op1 = currentCommand_.find(" ", 0);
        int op2 = currentCommand_.find(" ", op1 + 1);
        int op3 = currentCommand_.find(" ", op2 + 1);
        string arg = currentCommand_.substr(op2 + 1, op3 - op2 - 1);
        int arg2i = stoi(arg);
        return(arg2i);
    }
}

void Parser::setFileName(string filename) {
    filename_= filename;
    filename = filename + ".vm";
    
    if(inf.is_open()) {
        inf.close();
    }
	inf.open(filename.c_str());
	if (!inf.is_open()) {
		cerr << "Error: Could not open " << filename << ".\n";
    }
}
