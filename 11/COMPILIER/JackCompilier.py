import sys
import os
import CompilierEngine

def main():
    if len(sys.argv) < 2:
        print("No input file specified.")
        return -1

    if not os.path.exists(sys.argv[1]):
        print(sys.argv[1], "does not exist - maybe you need elevated permissions?")
        return -1
    
    inf = sys.argv[1]
    if os.path.isdir(sys.argv[1]): #do we have .jack extension or not
        compileFiles(inf)
    else:
        compileFile(inf)

def compileFile(file):
    print(file, "Compiling...")
    fileExt = file.replace(".jack", ".vm")
    CompilierEngine.CompilierEngine(file, fileExt)

def compileFiles(file):
    for ext in os.listdir(file):
        if ext.find('jack') != 1:
            compileFile(file + ext)

if __name__ == '__main__':
    main()
