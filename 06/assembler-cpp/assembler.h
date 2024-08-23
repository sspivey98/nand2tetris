#include<string>

//SymbolTable Methods
int contains(std::string);
void addLabel(std::string, int);
void addVariable(std::string);
int getSymbol(std::string);

//Parser Methods
void Parser(std::string);
int hasMoreCommands(void);
std::string advance(void);
std::string commandType(void);
std::string symbol(void);
std::string dest(void);
std::string comp(void);
std::string jmp(void);
std::string getCurrentCommand(void);
void removeLCommand(void);
int getLine(void);
void setLine(int);
void loadTokens(std::string);
std::string getTokenIdx(int);
int numberOfTokens();
std::string removeSpaces(std::string);

//Coder
void Code(void);
std::string dest(std::string);
std::string comp(std::string);
std::string jmp(std::string);

