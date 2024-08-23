import sys
#put list of arrays of string constants
TOKEN_TYPES = ["KEYWORD", "SYMBOL", "IDENTIFIER", "INT_CONST", "STRING_CONST"]

TOKEN_KEYWORDS = ["CLASS", "METHOD", "FUNCTION", "CONSTRUCTOR", "INT", \
					"BOOLEAN", "CHAR", "VOID", "VAR", "STATIC", "FIELD", \
					"LET", "DO", "IF", "ELSE", "WHILE", "RETURN", "TRUE", \
					"FALSE", "NULL", "THIS"]

SYMBOLS = ['(', ')', '[', ']', '{', '}', ',', ';', '=', '.', '+', '-', '*', \
		 '/', '&', '|', '~', '<', '>']

WHITE_SPACE = [' ', '\n', '\t']

class JackTokenizer():
    #Frozen set makes it immutable (i.e. final operator)
    _tokenTypes = frozenset(TOKEN_TYPES)
    _tokenKeywords = frozenset(TOKEN_KEYWORDS)
    _symbols = frozenset(SYMBOLS)
    _whiteSpace = frozenset(WHITE_SPACE)

    def __init__(self, filename):
        self.file = open(filename, 'r')
        self.currentToken = ""

        if self.hasMoreTokens():
            self.advance()
    
    def __del__(self):
        self.file.close()

    def hasMoreTokens(self):
        while 1:
            if self._peek() == "":
                return False

            #skip space & \n
            while self._peek() in self._whiteSpace:
                self._pop()

            #skip comments
            skco = self._peek(2) 
            while skco in ["//", "/*"]:
                if self._peek(2) == "//":
                    self._skipLine()
                else:
                    self._skipComment()
                
                skco = self._peek(2)
            
            #odd case where it COULD BE a token - it is not a comment or whitespace, most likely a /r
            if self._peek() not in self._whiteSpace:
                return True
    def advance(self):
        if self.hasMoreTokens():
            if self._peek() != '"':
                self.currentToken = self._peekWord()
                self._pop(len(self.currentToken))
            else:
                #first "
                tempstring = self._pop() 
                while self._peek() != '"':
                    tempstring = tempstring + self._pop()
                tempstring = tempstring + self._pop() #end "
                self.currentToken = tempstring
    def tokenType(self):
        token = self.currentToken.upper()
        if token in self._symbols:
            return "SYMBOL"
        if token in self._tokenKeywords:
            return "KEYWORD"
        if '"' in token:
            return "STRING_CONST"
        if token.isdigit():
            return "INT_CONST"

        return "IDENTIFIER"
    
    def keyWord(self):
        return self.currentToken.upper()
    def symbol(self):
        return self.currentToken[0]
    def identifier(self):
        return self.currentToken
    def intVal(self):
        return int(self.currentToken)
    def stringVal(self):
        return self.currentToken.replace('"', '')
    #######################PRIVATE__HELPER_METHODS##############################
    def _peek(self, n=1): #n=1 acts as a default constructor; you can set n or not, it auto defaults to 1
        genesisPos = self.file.tell()
        char = self.file.read(n)
        self.file.seek(genesisPos)
        return char

    def _pop(self, n=1):
        return self.file.read(n)
    def _skipLine(self):
        self.file.readline()
    def _skipComment(self):
        starFound = False
        while 1:
            char = self._pop()
            if char == '*':
                starFound = True
            else:
                if starFound == True and char == '/':
                    return
                if char != '\n':
                    starFound = False
    #debugging only
    def _rawtoken(self): 
        return self.currentToken
    
    #see the entier word 
    def _peekWord(self):
        genesisPos = self.file.tell()
        currChar = self._pop()
        result = ""
        if currChar in self._symbols:
            result = currChar
        else:
            while (currChar != "") and (currChar not in self._whiteSpace):
                if currChar in self._symbols:
                    break
                result = result + currChar
                currChar = self._pop()
        self.file.seek(genesisPos)
        return result   
#def main():