assume cs:code 

msg  segment
       DB 	'Name: HanZhuo',0,
	'Number: 201785095',0,
	'Phone: 17741192582',0,
	'Email: hanzhuomail@qq.com',0,0
msg ends

code segment
start:    
	MOV AX, msg
           	MOV DS, AX
           	MOV SI, 0
           	CALL site
           	MOV AX, 0B800h
           	MOV ES, AX
           	MOV BP, 0
          	MOV AX, 0
s1:
	MOV AL, DS:[BP]
	CMP AL, 0
	JE s2
	MOV ES:[BX][SI], AL
	ADD SI, 2
	INC BP
	JMP s1     
site:
	MOV BX, 1600
	MOV SI, 50      ;行起始位置调整
	MOV DI,SI
	RET
s2:
	INC BP
	MOV AL, DS:[BP]
	CMP AL, 0
	JE exit
	ADD BX, 160
	MOV SI, DI
	JMP s1
exit:
	MOV AX, 4C00H
	INT 21H

code ends
end start
