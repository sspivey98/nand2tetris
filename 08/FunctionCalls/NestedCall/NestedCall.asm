(Sys.init)
//Push 4000
@4000
D=A
@SP
A=M
M=D
@SP
M=M+1
//Pop 0
@SP
A=M-1
D=M
@R13
M=D
@THIS
D=A
@R14
M=D
@R13
D=M
@R14
A=M
M=D
@SP
M=M-1
//Push 5000
@5000
D=A
@SP
A=M
M=D
@SP
M=M+1
//Pop 1
@SP
A=M-1
D=M
@R13
M=D
@THAT
D=A
@R14
M=D
@R13
D=M
@R14
A=M
M=D
@SP
M=M-1
@retPointSys.main0
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@0
D=D-A
@5
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Sys.main
0;JMP
(retPointSys.main0)
//Pop 1
@SP
A=M-1
D=M
@R13
M=D
@R5
D=A
@1
D=D+A
@R14
M=D
@R13
D=M
@R14
A=M
M=D
@SP
M=M-1
(LOOP)
@LOOP
0;JMP
(Sys.main)
//Push 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
//Push 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
//Push 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
//Push 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
//Push 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
//Push 4001
@4001
D=A
@SP
A=M
M=D
@SP
M=M+1
//Pop 0
@SP
A=M-1
D=M
@R13
M=D
@THIS
D=A
@R14
M=D
@R13
D=M
@R14
A=M
M=D
@SP
M=M-1
//Push 5001
@5001
D=A
@SP
A=M
M=D
@SP
M=M+1
//Pop 1
@SP
A=M-1
D=M
@R13
M=D
@THAT
D=A
@R14
M=D
@R13
D=M
@R14
A=M
M=D
@SP
M=M-1
//Push 200
@200
D=A
@SP
A=M
M=D
@SP
M=M+1
//Pop 1
@SP
A=M-1
D=M
@R13
M=D
@LCL
D=M
@1
D=D+A
@R14
M=D
@R13
D=M
@R14
A=M
M=D
@SP
M=M-1
//Push 40
@40
D=A
@SP
A=M
M=D
@SP
M=M+1
//Pop 2
@SP
A=M-1
D=M
@R13
M=D
@LCL
D=M
@2
D=D+A
@R14
M=D
@R13
D=M
@R14
A=M
M=D
@SP
M=M-1
//Push 6
@6
D=A
@SP
A=M
M=D
@SP
M=M+1
//Pop 3
@SP
A=M-1
D=M
@R13
M=D
@LCL
D=M
@3
D=D+A
@R14
M=D
@R13
D=M
@R14
A=M
M=D
@SP
M=M-1
//Push 123
@123
D=A
@SP
A=M
M=D
@SP
M=M+1
@retPointSys.add121
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@1
D=D-A
@5
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Sys.add12
0;JMP
(retPointSys.add121)
//Pop 0
@SP
A=M-1
D=M
@R13
M=D
@R5
D=A
@0
D=D+A
@R14
M=D
@R13
D=M
@R14
A=M
M=D
@SP
M=M-1
//Push 0
@LCL
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
//Push 1
@LCL
D=M
@1
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
//Push 2
@LCL
D=M
@2
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
//Push 3
@LCL
D=M
@3
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
//Push 4
@LCL
D=M
@4
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
//add
@SP
A=M-1
D=M
A=A-1
M=M+D
@SP
M=M-1
//add
@SP
A=M-1
D=M
A=A-1
M=M+D
@SP
M=M-1
//add
@SP
A=M-1
D=M
A=A-1
M=M+D
@SP
M=M-1
//add
@SP
A=M-1
D=M
A=A-1
M=M+D
@SP
M=M-1
@LCL
D=M
@R15
M=D
@5
A=D-A
D=M
@R16
M=D
//Pop 0
@SP
A=M-1
D=M
@R13
M=D
@ARG
D=M
@0
D=D+A
@R14
M=D
@R13
D=M
@R14
A=M
M=D
@SP
M=M-1
@ARG
D=M+1
@SP
M=D
@R15
D=M
A=D-1
D=M
@THAT
M=D
@R15
M=M-1
@R15
D=M
A=D-1
D=M
@THIS
M=D
@R15
M=M-1
@R15
D=M
A=D-1
D=M
@ARG
M=D
@R15
M=M-1
@R15
D=M
A=D-1
D=M
@LCL
M=D
@R15
M=M-1
@R16
A=M
0;JMP
(Sys.add12)
//Push 4002
@4002
D=A
@SP
A=M
M=D
@SP
M=M+1
//Pop 0
@SP
A=M-1
D=M
@R13
M=D
@THIS
D=A
@R14
M=D
@R13
D=M
@R14
A=M
M=D
@SP
M=M-1
//Push 5002
@5002
D=A
@SP
A=M
M=D
@SP
M=M+1
//Pop 1
@SP
A=M-1
D=M
@R13
M=D
@THAT
D=A
@R14
M=D
@R13
D=M
@R14
A=M
M=D
@SP
M=M-1
//Push 0
@ARG
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
//Push 12
@12
D=A
@SP
A=M
M=D
@SP
M=M+1
//add
@SP
A=M-1
D=M
A=A-1
M=M+D
@SP
M=M-1
@LCL
D=M
@R15
M=D
@5
A=D-A
D=M
@R16
M=D
//Pop 0
@SP
A=M-1
D=M
@R13
M=D
@ARG
D=M
@0
D=D+A
@R14
M=D
@R13
D=M
@R14
A=M
M=D
@SP
M=M-1
@ARG
D=M+1
@SP
M=D
@R15
D=M
A=D-1
D=M
@THAT
M=D
@R15
M=M-1
@R15
D=M
A=D-1
D=M
@THIS
M=D
@R15
M=M-1
@R15
D=M
A=D-1
D=M
@ARG
M=D
@R15
M=M-1
@R15
D=M
A=D-1
D=M
@LCL
M=D
@R15
M=M-1
@R16
A=M
0;JMP
