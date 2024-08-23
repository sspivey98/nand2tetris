class VMWriter():
    def __init__(self, outf):
        self._output = open(outf, 'w')
    
    def close(self): #destructor
        self._output.close()
    
    #push or pop and segment and index
    def writePush(self, segment, index):
        self._output.write("push %s %d\n" % (segment.lower(), index))
    def writePop(self, segment, index):
        self._output.write("pop %s %d\n" % (segment.lower(), index))

    #checks operator and writes to the file the correct command sequence
    def writeArithmetic(self, command):
        if command == '+':
            self._output.write("add")
        elif command == '-':
            self._output.write("sub")
        elif command == '*':
            self._output.write("call Math.multiply 2")
        elif command == '/':
            self._output.write("call Math.divide 2")
        elif command == '&':
            self._output.write("and")
        elif command == '|':
            self._output.write("or")
        elif command == '<':
            self._output.write("lt")
        elif command == '>':
            self._output.write("gt")
        elif command == '=':
            self._output.write("eq")
        elif command == '~':
            self._output.write("not")
        elif command == 'neg':
            self._output.write("neg")
        
        self._output.write("\n")

    def writeLabel(self, label):
        self._output.write("label %s\n" % label)
    def writeGoto(self, label) :
        self._output.write("goto %s\n" % label)
    def writeIf(self, label):
        self._output.write("if-goto %s\n" % label)


    def writeCall(self, label, args):
        self._output.write("call %s %d\n" % (label, args))
    def writeFunction(self, label, local):
        self._output.write("function %s %d\n" % (label, local))
    def writeReturn(self):
        self._output.write("return\n")
    
    #allocates memory of 'size' size
    def writeAlloc(self, size):
        self.writePush("CONSTANT", size)
        self._output.write("Call Memory.alloc 1\n")

    #writes a string by using the definition of a string as an array of char
    def writeString(self, string):
        self.writePush("CONSTANT", len(string))
        self.writeCall("String.new", 1)
        for char in string:
            unicodeVal = ord(char)
            self.writePush("CONSTANT", unicodeVal)
            self.writeCall("String.appendChar", 2)
