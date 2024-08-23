#include "CodeWriter.h"
#include "CodeWriter.cpp"
#include "Parser.h"
#include "Parser.cpp"

int main(int argc, char *argv[]) {
    CodeWriter CW;
    Parser P;

    for(int i=1; i<argc; i++) {
        string filename = argv[i];
        int dotLoc = filename.find(".", 0);

        if (dotLoc == -1) {
            P.setFileName(filename);
        }
        else {
            filename = filename.substr(0, dotLoc);
            P.setFileName(filename);
        }

        if (i == 1) {
                cout << filename << endl;
                CW.setFileName(filename);
        }


        if (argc > 2 && i == 1) {
            CW.writeInit();
        }

        cout << "Starting Translation..." << endl;

        while (P.hasMoreCommands()) {
            P.advance();
            if (P.commandType() == C_ARITHMETIC) {
                CW.writeArithmetic(P.arg1());
            }
            else if (P.commandType() == C_POP) {
                CW.writePushPop(C_POP, P.arg1(), P.arg2());
            }
            else if (P.commandType() == C_PUSH) {
                CW.writePushPop(C_PUSH, P.arg1(), P.arg2());
            }
            else if (P.commandType() == C_LABEL) {
				CW.writeLabel(P.arg1());
			}
			else if(P.commandType() == C_GOTO) {
				CW.writeGoto(P.arg1());
			}
			else if(P.commandType() == C_IF) {
				CW.writeIf(P.arg1());
			}
			else if(P.commandType() == C_FUNCTION) {
				CW.writeFunction(P.arg1(), P.arg2());
			}
			else if(P.commandType() == C_RETURN) {
				CW.writeReturn();
			}
			else if(P.commandType() == C_CALL) {
				CW.writeCall(P.arg1(), P.arg2());
			}
        }
    P.close();
    }
    //Close and destructor
    CW.close();
    cout << "Finished" << endl;
}
