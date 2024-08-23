#ifndef _PARSER_
#define _PARSER_

#include <string>
#include <fstream>
#include <iostream>
using namespace std;

enum VMcommand {C_ARITHMETIC, C_PUSH, C_POP, C_LABEL, C_GOTO, C_IF, C_FUNCTION, C_RETURN, C_CALL};

/*
    hasMoreCommands -> are there more commands in the input?
	advance -> reads next command from input, should only be called if hasMoreCommands() is true
	commandType -> returns the type of the current VM command
	arg1 -> returns the first argument of the current command
	arg2 -> returns the second argument of the current command
*/

class Parser {
    public:
        Parser();
        ~Parser();

        bool hasMoreCommands(void);
        void advance(void);
        VMcommand commandType(void);
        string arg1(void);
        int arg2(void);

        //Project 8
        void setFileName(string filename);

    private:
        string filename_;
        string currentCommand_;
        ifstream inf;
};

#endif