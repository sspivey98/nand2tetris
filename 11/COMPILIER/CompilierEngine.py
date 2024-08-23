import JackTokenizer
import VMWriter
import SymbolTable

class CompilierEngine():
    def __init__(self, inf, outf):
        #creating input file and ouput file objects
        self._tokenizer = JackTokenizer.JackTokenizer(inf)
        self._VMWriter = VMWriter.VMWriter(outf)

        #creating symboltable structure
        self._symbolTable = SymbolTable.SymbolTable()

        #context counter vars
        self._label_index = 0
        self._if_index = 0
        self._while_index = 0

        #start compiling
        self.compileClass()
        self._VMWriter.close()

    def compileClass(self):
        #initialize class object, class name 
        self._getKeyword()
        self._class_name = self._getIdentifier

        self._getSymbol() #get first '{'

        #class vars 
        if (self._tokenizer.keyWord() in ["STATIC", "FIELD"]):
            self.compileClassVarDec()
        
        #class subroutines
        while (self._tokenizer.keyWord() in ["CONSTRUCTOR", "FUNCTION", "METHOD", "VOID"]):
            self.compileSubroutine()
    
        self._getSymbol() #gets last '}' 

    def compileClassVarDec(self):
        while (self._tokenxKW("STATIC") or self._tokenxKW("FIELD")):
            varKind = self._getKeyword()
            varType = self._getType()
            varName = self._getIdentifier()

            self.defineVar(varName, varType, varKind)

            #might be multiple var declerations on the same line. However, they will be the same type and kind.
            while self._tokenizer.symbol() == ',':
                self._getSymbol() #grab the comma first
                varName = self._getIdentifier()
                self._defineVar(varName, varType, varKind)
            
            self._getSymbol() #grab ';'
    
    def compileSubroutine(self):
        self._symbolTable.startSubroutine() #intialize and reset values
        
        subType = self._getKeyword()
        returnType = self._getType()
        subName = self._getIdentifier()

        if subType == "METHOD": #reserving a pointer to the first for the first arg
            self._defineVar("this_ptr", "INT", "ARG")
        
        self.compileParameters() #paramterlist may be empty

        self._getSymbol() # '{'

        local_counter = 0
        if self._tokenizer.keyWord() == "VAR":
            local_counter = self.compileClassVarDec()
        
        callName = self._callName + "." + subName
        self._VMWriter.writeFunction(callName, local_counter)

        if subType == "METHOD": #argument 0 is 'this' - 'this' points to register
            self._VMWriter.writePush("ARGUMENT", 0)
            self._VMWriter.writePop("POINTER", 0)
        elif subType == "CONSTRUCTOR": #for constructor invoke OS functions for space allocation
            self._VMWriter.writeAlloc(self._symbolTable.varCount("FIELD"))
            self._VMWriter.writePop("POINTER", 0)
        
        if not self._tokenxSymbol('}'): #check for let; return, do, if, while conditional statements
            self.compileStatements()
        self._getSymbol() # '}'
        #end of subroutine

        if returnType == "VOID":
            self._VMWriter.writePush("CONSTANT", 0)
        self._VMWriter.writeReturn()


    def compileParameters(self):
        self._getSymbol() # '('

        while self._tokenizer.symbol() != ')': #we have stuff in the ()
            varType = self._getKeyword()
            varName = self._getIdentifier()
            self._defineVar(varName, varType, "ARG")

            #we potentially could have more parameters
            if self._tokenizer.symbol() == ',':
                self._getSymbol()
            
        self._getSymbol() # ')'
    
    #while there are lines declaring vars, add them to the local dictionary. could be multiple var decelrations on one line
    def compileVarDec(self):
        varCounter = 0
        while self._tokenxKW("VAR"):
            self._getKeyword() # 'VAR'
            varType = self._getType()
            varName = self._getIdentifier()

            self._defineVar(varName, varType, "LOCAL")
            varCounter += 1

            while self._tokenizer.symbol() == ',': #multiple declarations?
                self._getSymbol()
                varName = self._getIdentifier()
                self._defineVar(varName, varType, "LOCAL")
                varCounter += 1
            
        self._getSymbol() # ';'
        return varCounter
        
    #if branching method for keyword identifying
    def compileStatements(self):
        while not self._tokenxSymbol('}'):
            if self._tokenizer.keyWord() == "LET":
                self.compileLet()
            
            elif self._tokenizer.keyWord() == "RETURN":
                self.compileReturn()
            elif self._tokenizer.keyWord() == "DO":
                self.compileDo()
            elif self._tokenizer.keyWord() == "IF":
                self.compileIf()
            elif self._tokenizer.keyWord() == "WHILE":
                self.compileWhile()

    def compileDo(self):
        self._getKeyword()
        name = self._getIdentifier()
        self._writeSubroutineCall(name, returnsvoid = True)
        self._getSymbol() # ';'

    def compileLet(self):
        self._getKeyword() #let
        varName = self._getIdentifier()
        isArray = False

        if self._tokenizer.symbol() == '[':
            isArray = True

            self._writeVarPush(varName)

            self._getSymbol() # '['
            self.compileExpression()
            self._getSymbol() # ']'

            self._VMWriter.writeArithmetic('+')
            self._VMWriter.writePop("POINTER", 1)
        
        self._getSymbol() # '='
        self.compileExpression()

        if isArray: #push to 'that' pointer
            self._VMWriter.writePop("THAT", 0)
        else:
            self._writeVarPop(varName)
        
        self._getSymbol() # ';'

    def compileWhile(self):
        self._while_index += 1

        whileBeginLabel = "W%d" % self._while_index
        whileEndLabel = "Wend%d" % self._while_index

        self._VMWriter.writeLabel(whileBeginLabel)

        self._getKeyword() # 'WHILE'
        self._getSymbol() # '('
        self.compileExpression()
        self._getSymbol() # ')'

        #negate and making a goto in case of false
        self._VMWriter.writeArithmetic("~")
        self._VMWriter.writeIf(whileEndLabel)

        self._getSymbol() # '('
        self.compileStatements()
        self._getSymbol() # ')'

        self._VMWriter.writeGoto(whileBeginLabel)
        self._VMWriter.writeLabel(whileEndLabel)
    
    def compileReturn(self):
        self._getKeyword() # 'RETURN'

        #if our next char is not a ';' we have an expression
        if not self._tokenxSymbol(';'): 
            self.compileExpression()
        
        self._getSymbol # ';'
    
    def compileIf(self):
        self._if_index += 1

        self._getKeyword() # 'IF'
        self._getSymbol() # '('
        self.compileExpression()
        self._getSymbol() # ')'
        self._VMWriter.writeArithmetic("~")

        falseLabel = "ifF%d" % self._if_index
        trueLabel = "ifT%d" % self._if_index
        endIfLabel = "ifEnd%d" % self._if_index
    
        self._VMWriter.writeIf(falseLabel)
        
        self._getSymbol() # '{'
        self._VMWriter.writeLabel(trueLabel)
        self.compileStatements()
        self._VMWriter.writeGoto(endIfLabel)
        self._getSymbol() # '}'

        self._VMWriter.writeLabel(falseLabel)
        if self._tokenxKW("ELSE"):
            self._getKeyword() # 'ELSE'
            self._getSymbol() # '{'
            self.compileStatements()
            self._getSymbol() # '}'

        self._VMWriter.writeLabel(endIfLabel)

    def compileExpression(self):
        self.compileTerm() 

        while self._tokenIsOperator():
            command - self._getSymbol()
            self.compileTerm()
            self._VMWriter.writeArithmetic(command)

    def compileExpressionList(self):
        self._getSymbol() # '('
        expressionCounter = 0

        while not self._tokenxSymbol(')'):
            exp = self.compileExpression()
            expressionCounter += 1

            if self._tokenxSymbol(','): #if we find multiple declerations, just skip the comma
                self._getSymbol()
        self._getSymbol() # ')'
        return expressionCounter
    
    def compileTerm(self):
        tokenType = self._tokenizer.tokenType()

        if self._tokenIsKeywordConstant():
            kw = self._getKeyword()
            
            if kw == "FALSE" or kw == "NULL":
                self._VMWriter.writePush("constant", 0)
            elif kw == "TRUE":
                self._VMWriter.writePush("constant", 0)
                self._VMWriter.writeArithmetic("~")
            elif kw == "THIS":
                self._VMWriter.writePush("pointer", 0)
        elif tokenType == "INT_CONST":
            self._VMWriter.writePush("constant", self._getIntVal)
        
        elif tokenType == "STRING_CONST":
            tempstring = self._getStringVal()
            self._VMWriter.writeString(tempstring)
        
        elif tokenType == "IDENTIFIER":
            varName = self._getIdentifier()
            varKind = self._symbolTable.kindOf(varName)
            self._writeVarPush(varName)

            if self._tokenxSymbol('['): #we have an array
                self._getSymbol() #'['
                self._VMWriter.writePush("POINTER", 1)
                self._VMWriter.writePop("TEMP", 0)
                self.compileExpression()

                self._VMWriter.writeArithmetic('+') #we may need to set the memory location
                self._VMWriter.writePop("POINTER", 1)
                self._VMWriter.writePush("TEMP", 1)

                self._VMWriter.writePush("TEMP", 0)
                self._VMWriter.writePop("POINTER", 1)

                self._VMWriter.writePush("TEMP", 1)
                self._getSymbol() # ']'
            
            elif self._tokenxSymbol('('):
                self._writeSubroutineCall(varName)
            elif self._tokenxSymbol('.'):
                self._writeSubroutineCall(varName)

        elif self._tokenxSymbol('('):
            self._getSymbol() # '('
            expression = self.compileExpression()
            self._getSymbol() # ')'

        elif self._tokenIsUnaryOperator():
            symbol = self._getSymbol() # 'OP'
            self.compileTerm()
            if symbol == '-':
                symbol = 'neg'
            
            self._VMWriter.writeArithmetic(symbol)



    #######################PRIVATE__HELPER_METHODS##############################
    #pipelines between jackTokenizer class
    def _getKeyword(self):
        keyword = self._tokenizer.keyWord()
        self._tokenizer.advance()
        return keyword
    def _getIdentifier(self):
        identifier = self._tokenizer.identifier()
        self._tokenizer.advance()
        return identifier
    def _getSymbol(self):
        symbol = self._tokenizer.symbol()
        self._tokenizer.advance()
        return symbol
    #type could be a primitive or composite
    def _getType(self):
        if self._tokenIsPrimitive():
            return self._getKeyword()
        else:
            return self._getIdentifier()   
    def _getIntVal(self):
        intVal = self._tokenizer.intVal()
        self._tokenizer.advance()
        return intVal
    def _getStringVal(self):
        stringVal = self._tokenizer.stringVal()
        self._tokenizer.advance()
        return stringVal

    def _tokenIsPrimitive():
        return (self._tokenIsKeyword() and self._tokenizer.keyWord() in ["INT", "CHAR", "BOOLEAN", "VOID"])

    #matches token to keyword, keyword must be "KEYWORD" and match tokenizer
    def _tokenxKW(self, kw):
        return (self._tokenIsKeyword() and self._tokenizer.keyWord() == kw) 
    def _tokenIsKeyword(self):
        return self._tokenizer.tokenType() == "KEYWORD"

    #matches token to symbol, keyword must be "SYMBOL" and match tokenizer
    def _tokenxSymbol(self, symbol):
        return (self._tokenIsSymbol() and self._tokenizer.symbol() == symbol)
    def _tokenIsSymbol(self):
        return (self._tokenizer.tokenType == "SYMBOL")

    #define a var in the symbol table - if we look on the table and find nothing add it
    def _defineVar(self, name, jtype, kind):
        if self._symbolTable.kindOf(name) == "NONE":
            self._symbolTable.define(name, jtype, kind)
    
    def _tokenIsOperator(self):
        return (self._tokenIsSymbol() and self._tokenizer.symbol() in ['+', '-', '*', '/', '&', '|', '<', '>', '='])
    def _tokenIsUnaryOperator(self):
        return (self._tokenIsSymbol() and self._tokenizer.symbol in ['-', '~'])
    def _tokenIsKeywordConstant(self):
        return (self._tokenIsKeyword() and self._tokenizer.keyWord in ["TRUE", "FALSE", "NULL", "THIS"])

    def _writeVarPush(self, varName):
        # Writes the operations to push a variable depending on its kind
		varKind = self._symbolTable.kindOf(var_name)
		if varKind == "NONE":
			return

		varIndex = self._symbolTable.indexOf(var_name)
		if varKind == "FIELD":
			self.VMWriter.writePush("THIS", varIndex)
		elif varKind == "STATIC":
			self.VMWriter.writePush("STATIC", varIndex)
		elif varKind == "LOCAL":
			self.VMWriter.writePush("LOCAL", varIndex)
		elif varKind == "ARG":
			self.VMWriter.writePush("ARGUMENT", varIndex)

    def _writeVarPop(self, varName):
        # Writes the operations to pop a variable depending on its kind
		varKind = self._symbolTable.kindOf(var_name)
		if varKind == "NONE":
			return

		varIndex = self._symbolTable.indexOf(var_name)
		if varKind == "FIELD":
			self.VMWriter.writePop("THIS", varIndex)
		elif varKind == "STATIC":
			self.VMWriter.writePop("STATIC", varIndex)
		elif varKind == "LOCAL":
			self.VMWriter.writePop("LOCAL", varIndex)
		elif varKind == "ARG":
			self.VMWriter.writePop("ARGUMENT", varIndex)
    
    def _writeSubroutineCall(self, name, returnsvoid = False):
        callName = ""
        methodName = ""
        pushPointer = False

        if self._tokenxSymbol('.'):
            self._getSymbol() # '.'
            methodName = self._getIdentifier()
        
        if methodName == "":
            pushPointer = True
            self._VMWriter.writePush("POINTER", 0)
            callName = "%s.%s" % (self._class_name, name)
        else:
            kind = self._symbolTable.kindOf(name)
            if kind == "NONE":
                callName = "%s.%s" % (name, methodName)
            else:
                temp = self._symbolTable.typeOf(name)
                callName = "%s.%s" % (temp, methodName)
                pushPointer = True
                self._writeVarPush(name)

        paramcounter = self.compileExpressionList()

        if pushPointer:
            paramcounter += 1
        
        self._VMWriter.writeCall(callName, paramcounter)

        if returnsvoid:
            self._VMWriter.writePop("TEMP", 0)

            