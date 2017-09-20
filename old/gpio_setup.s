
       AREA    MYCODE, CODE, READONLY, ALIGN=2
       IMPORT  DrawBlock
	   IMPORT  DrawShape
       IMPORT  DelayMs
       IMPORT  Write_Char
       IMPORT  Read_Char
       IMPORT  Setup_GPIOA
       IMPORT  Setup_GPIOE
	   IMPORT  LCD_Blank
	   IMPORT  LCD_Refresh
       INCLUDE Registers.s	
       EXPORT  GPIO_Setup_A             
       EXPORT  GPIO_Setup_E             
	   EXPORT  Read_and_Move
             
GPIO_Setup_A
		PUSH {LR, R0-R3}
		;BL Setup_GPIOA   ; replace this with your function to setup port A
		LDR R0, =SYSCTL_RCGCGPIO
		LDR R1, [R0]
		ORR R1, #0x1
		STR R1, [R0]
		LDR R0, = SYSCTL_PRGPIO
ClkWaitA 
		LDR R1, [R0]
		AND R1, #0x1
		CMP R1, #0x1
		BNE ClkWaitA
		MOV R3, #10
		BL DelayMs
		
		
		LDR R0, =GPIO_A_BASE
		LDR R1, [R0,#GPIO_DIR]
		ORR R1, #0xE0
		STR R1, [R0,#GPIO_DIR]
		
		LDR R1, [R0, #GPIO_AFSEL]
		BIC R1, #0xE0
		STR R1, [R0, #GPIO_AFSEL]

		LDR R1, [R0, #GPIO_DEN]
		ORR R1, #0xE0
		STR R1, [R0,#GPIO_DEN]
	
		
		POP  {LR, R0-R3}
		BX LR
      
GPIO_Setup_E
		PUSH {LR, R0-R3}
		;BL Setup_GPIOE   ; replace this with your function to setup port E
		LDR R0, =SYSCTL_RCGCGPIO
		LDR R1, [R0]
		ORR R1, #0x10
		STR R1, [R0]
		LDR R0, = SYSCTL_PRGPIO
ClkWaitE
		LDR R1, [R0]
		AND R1, #0x10
		CMP R1, #0x10
		BNE ClkWaitE
		MOV R3, #10
		BL DelayMs
		
		LDR R0, =GPIO_E_BASE
		LDR R1, [R0,#GPIO_DIR]
		ORR R1, #0x38
		STR R1, [R0,#GPIO_DIR]
		
		LDR R1, [R0, #GPIO_AFSEL]
		BIC R1, #0x38
		STR R1, [R0, #GPIO_AFSEL]
		
		LDR R1, [R0, #GPIO_DEN]
		ORR R1, #0x38
		STR R1, [R0,#GPIO_DEN]
		
		
		POP  {LR, R0-R3}
		BX LR
      
Read_and_Move
    PUSH  {LR, R0-R3}
    ; write instructions to complete parts 3-6
	BL Read_Char
	CMP R4, #'s'
	BEQ AddY
			  
	CMP R4, #'w'
	BEQ SubY
			  
	CMP R4, #'a'
	BEQ SubX
			  
	CMP R4, #'d'
	BEQ AddX
			  
	CMP R4, #'b'
	BEQ Clear
			  
Clear

	BL LCD_Blank
	B ClearExit
	  
AddX
	;BL LCD_Blank
	CMP R5, #110
	BGT Exit
	ADD R5,#5
	B Exit
			
AddY
	;BL LCD_Blank
	CMP R6, #45
	BGT Exit
	ADD R6,#5
	B Exit
			
SubX
	;BL LCD_Blank
	CMP R5, #15
	BLT Exit
	SUB R5,#5
	B Exit
			
SubY
	;BL LCD_Blank
	CMP R6, #15
	BLT Exit
	SUB R6,#5
	B Exit
	
Exit
  BL LCD_Blank
  BL DrawShape
  
			  
ClearExit

      POP  {LR, R0-R3}
      BX LR
      
      
       ALIGN      
       END  
