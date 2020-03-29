CODES   SEGMENT  
start:  
        MOV AX, CODES  
        MOV ES, AX  

        MOV AX, OFFSET msg  
        MOV BP, AX          
  
        MOV CX, OFFSET strend                
        MOV DX, OFFSET msg  
        SUB CX, DX         
        MOV len, CX  
  
next:  
        MOV AX, 1301H        
        MOV BH, 00H    
        MOV BL, color      
        MOV CX, len  
        MOV DX, 0815H     
        
        INT 10H      
        mov ah,4ch
        int 21h   
		
        color   DB 0FH  
        len     DW 0000H   
        msg   DB 	"Name: HanZhuo",0dh,0ah,
		"		     Number: 201785095",0dh,0ah,
		"		     Phone: 17741192582",0dh,0ah,
		"		     Email: hanzhuomail@qq.com"
        strend  DB '$'  
        ORG 07C00H+200H-2H             
        DW 0AA55H             
CODES   ENDS      
        END start  