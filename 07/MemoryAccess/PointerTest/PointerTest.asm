//Push 3030
@3030
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
//Push 3040
@3040
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
//Push 32
@32
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
@THIS
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
//Push 46
@46
D=A
@SP
A=M
M=D
@SP
M=M+1
//Pop 6
@SP
A=M-1
D=M
@R13
M=D
@THAT
D=M
@6
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
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
//Push 1
@THAT
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
//Push 2
@THIS
D=M
@2
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
//sub
@SP
A=M-1
D=M
A=A-1
M=M-D
@SP
M=M-1
//Push 6
@THAT
D=M
@6
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
