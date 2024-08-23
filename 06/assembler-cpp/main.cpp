#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <bitset>
#include <cstring>
#include <stdlib.h>

#include "Parser.h"
#include "Coder.h"
#include "SymbolTable.h"

bool is_number(const std::string &str);
void checkBinString(std::string outputString);

int main(int argc, char *argv[])
{
  std::string outputString = "";
  Parser myParser(*(argv + 1));
  Coder myDecoder;
  SymbolTable mySymbolTable;

  //FIRST PASS
  while (myParser.hasMoreCommands())
  {
    if (myParser.commandType() == "L_COMMAND")
    {
      std::string currentCommand = myParser.getCurrentCommand();

      int start = currentCommand.find('(') + 1; // remove the () parantheses
      int end = currentCommand.find(')') - 1;
      currentCommand = currentCommand.substr(start, end);
      mySymbolTable.addLabel(currentCommand, myParser.getLine()); //add the label at the current line
      myParser.removeLCommand();                                  //remove the L command from the token list
    };
    myParser.advance();
  };

  //SECOND PASS
  myParser.setLine(0); //reset the parser to the top
  while (myParser.hasMoreCommands())
  {
    if (myParser.commandType() == "C_COMMAND") //handle C commands
    {
      std::string destBin = myDecoder.dest(myParser.dest());
      std::string compBin = myDecoder.comp(myParser.comp());
      std::string jmpBin = myDecoder.jmp(myParser.jmp()); //get the binary values for each component of the code string

      outputString = outputString + "111" + compBin + destBin + jmpBin + "\n"; //concatenate the c instruction binary
    }
    else if (myParser.commandType() == "A_COMMAND") //handle A commands
    {

      std::string currentCommand = myParser.getCurrentCommand();
      int end = currentCommand.find('\0');
      currentCommand = currentCommand.substr(1, end);

      int x;
      if (!is_number(currentCommand)) //if @xxx is a symbol
      {
        mySymbolTable.addVariable(currentCommand); //try to add the variable if first encounter
        x = mySymbolTable.getSymbol(currentCommand);
      }
      else //if @var is a number
      {
        x = stoi(currentCommand);
      };

      std::string binString = std::bitset<15>(x).to_string();
      outputString = outputString + "0" + binString + "\n"; //concatenate the a instruction binary
    };
    myParser.advance(); //take one step
  };

  //Check string and save file
  //TODO
  checkBinString(outputString);
  std::ofstream myfile;
  std::string outputFile = *(argv + 2);
  myfile.open(outputFile);
  myfile << outputString;
  myfile.close();

  return 0;
}

bool is_number(const std::string &str)
{
  return str.find_first_not_of("0123456789") == std::string::npos;
};

void checkBinString(std::string outputString)
{
  std::cout << outputString << std::endl;
};
