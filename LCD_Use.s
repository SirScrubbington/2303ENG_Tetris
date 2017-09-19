	   INCLUDE Registers.s
       AREA    DATA, ALIGN=2
	   
LCDmat SPACE   1056
       AREA    LCDCODE, CODE, READONLY, ALIGN=2
       EXPORT  LCD_Init
	   EXPORT  LCD_Cmd_Out
	   EXPORT  LCD_Dat_Out
	   EXPORT  LCD_Contrast
	   EXPORT  LCD_Pixel
	   EXPORT LCD_Refresh
	   EXPORT LCD_Blank
	   EXPORT DelayMs_R0
		 	 

CSbit  EQU 0x80
RSTbit EQU 0x40
A0bit  EQU 0x20
CLKbit EQU 0x20
DATbit EQU 0x10
CSport EQU  0x400043FC
RSTport EQU 0x400043FC
A0port EQU  0x400043FC
CLKport EQU 0x400243FC
DATport EQU 0x400243FC

DelayMs_R0
		PUSH {LR,R0-R4}
		; R3 = Delay Time
		
OuterR0	SUB R0,#1
		MOV R1,#4000
InnerR0	
		SUB R1,#1
		CMP R1,#0
		BGT InnerR0
		CMP R0,#0
		BGT OuterR0

		POP {LR,R0-R4}
		BX LR

DelayMs
		PUSH {LR,R0-R4}
		; R3 = Delay Time
		
Outer 	SUB R3,#1
		MOV R0,#4000
Inner	
		SUB R0,#1
		CMP R0,#0
		BGT Inner
		CMP R3,#0
		BGT Outer

		POP {LR,R0-R4}
		BX LR

LCD_Init  
; assuming CSwrite pin0, RSTwrite pin1 
	   PUSH {LR, R0-R5}
	   LDR R1, =CSport
	   LDR R2, [R1]  
	   BIC R2, #(CSbit)        ; GPIO_DATA &= ~1   CSwrite(0)
	   STR R2, [R1]
	   LDR R1, =RSTport
	   LDR R2, [R1] 
	   BIC R2, #(RSTbit)        ; GPIO_DATA &= ~2   RSTwrite(0)
	   STR R2, [R1]      
	   MOV R3, #70
	   BL DelayMs        ; DelayMs(50ms)
	   LDR R2, [R1]
	   ORR R2, #(RSTbit)
	   STR R2, [R1]      ; GPIO_DATA |= 2    RSTwrite(1)
	   MOV R3, #70
	   BL DelayMs

	   MOV R0, #0xA2     ; 1/9th bias
	   BL  LCD_Cmd_Out
	   MOV R0, #0xA0
	   BL  LCD_Cmd_Out
	   MOV R0, #0xC0
	   BL  LCD_Cmd_Out
	   MOV R0, #0xC8
	   BL  LCD_Cmd_Out
	   MOV R0, #(0x20 :OR: 0x3)
	   BL  LCD_Cmd_Out
	   MOV R0, #(0x28 :OR: 0x4)
	   BL  LCD_Cmd_Out
	   MOV R3, #70
	   BL  DelayMs
	   MOV R0, #(0x28 :OR: 0x6)
	   BL  LCD_Cmd_Out
	   MOV R3, #70
	   BL  DelayMs
	   MOV R0, #(0x28 :OR: 0x7)
	   BL  LCD_Cmd_Out
	   MOV R3, #70
	   BL  DelayMs
	   MOV R0, #0x26
	   BL  LCD_Cmd_Out
	   MOV R0, #0x81
	   BL  LCD_Cmd_Out
	   MOV R0, #31
	   BL  LCD_Cmd_Out
	   MOV R0, #0x40
	   BL  LCD_Cmd_Out
	   MOV R0, #0xAF
	   BL  LCD_Cmd_Out
	   LDR R1, =CSport
	   LDR R2, [R1]
	   ORR R2, #(CSbit)      ; CSwrite(1)
	   STR R2, [R1]
		 
		 MOV R0, #5       ; set contrast
	   MOV R1, #50
	   BL LCD_Contrast
		 MOV R0, #0xF0
	   BL  LCD_Cmd_Out

	   POP {LR, R0-R5}
	   BX LR

	   
LCD_Cmd_Out	   
       ; char in R0
	   PUSH {LR, R0-R5}
	   AND R0, #0xFF
	   LDR R1, =A0port
	   LDR R2, [R1]  
	   BIC R2, #(A0bit)			; A0write(0)
	   STR R2, [R1]
	      
	   BL LCD_Char_Out
	   
	   POP {LR, R0-R5}
	   BX LR
	   
LCD_Dat_Out	   
	   PUSH {LR, R0-R5}
	   AND R0, #0xFF
	   LDR R1, =A0port
	   LDR R2, [R1]  
	   ORR R2, #(A0bit)			; A0write(1)
	   STR R2, [R1]
	      
	   BL LCD_Char_Out
	   
	   POP {LR, R0-R5}
	   BX LR
	   
LCD_Char_Out
	   PUSH {LR, R0-R5}
	   LDR R1, =CSport       ; port containing CS bit
	   LDR R2, [R1]
	   BIC R2, #(CSbit)           ; CSwrite(0);
	   STR R2, [R1] 
	   
	   MOV R3, #0           ;r3 is loop counter
COutLp 
	   AND R5, R0, #0x80
	   LDR R1, =DATport      ; port containing data bit
	   LDR R2, [R1]
	   CMP R5, #0
	   BEQ elsejp
       ORR R2, #(DATbit)	   ;GPIO_Data ORR 0x80
	   B  nextjp               ; or
elsejp BIC R2, #(DATbit)	   ;GPIO_Data BIC 0x80
nextjp STR R2, [R1]
 
       LDR R1, =CLKport       ; port containing CLK bit
       LDR R2, [R1]              
	   ORR R2, #(CLKbit)              ;pulse clock
	   STR R2, [R1]
	   LDR R2, [R1]
	   BIC R2, #(CLKbit)
	   STR R2, [R1]
	   
	   LSL R0, #1           ; data << 1
	   ADD R3, #1
	   CMP R3, #8
	   BLO COutLp
	   
	   LDR R1, =CSport 
	   LDR R2, [R1]
	   ORR R2, #(CSbit)           ; CSwrite(1);
	   STR R2, [R1]
	   POP {LR, R0-R5}
	   BX LR

LCD_Contrast
	   PUSH {LR, R0-R5}
	   ; R4 = res_ratio (<=7)  R5= contrast (<=63)
	   CMP R0, #7
	   BLS CtNt1
	   MOV R0, #7
CtNt1  CMP R1, #63
	   BLS CtNt2
	   MOV R1, #63
CtNt2  ORR R0, #0x20
	   BL LCD_Cmd_Out
	   MOV R0, #0x81
	   BL LCD_Cmd_Out
	   MOV R0, R1
	   BL LCD_Cmd_Out
	   POP {LR, R0-R5}
	   BX LR

LCD_Pixel
       ; R0 = x = 1-128, R1 = y= 1-64  R2= colour
	   PUSH {LR, R0-R7}
	   SUB R0, #1  ; x
	   CMP R1, #33
	   BLO top
	   SUB R1, #32
	   B pixnxt
top    ADD R1, #32
pixnxt 	   SUB R1, #1  ; y  
       MOV R3, #8
	   UDIV R4, R1, R3      ; y/8
	   MLS R5, R4, R3, R1   ;remainder of y/8
	   MOV R3, #128
	   MLA R6, R4, R3, R0    ; MATPOS = R6 = y/8 * 128 + x
	   LDR R4, =LCDmat
	   LDRB R7, [R4,R6]
	   MOV R3, #1
	   LSL R3, R5
	   CMP R2, #1
	   BNE blkpx
	   ORR R7, R3            ; lcd_mat[R6] orr
	   B pxend
blkpx  BIC  R7, R3           ; lcd_mat[R6] bic
pxend  STRB R7, [R4,R6]                   
       POP {LR, R0-R7}
	   BX LR

LCD_Refresh
	   PUSH {LR, R0-R7}
	   MOV R6, #0        ; y=R6, x=R5
yAgn                        ; y loop
       ORR R0, R6, #0xB0     ; R4=R6 | B0
	   BL LCD_Cmd_Out
       MOV R0, #0
       BL LCD_Cmd_Out
	   MOV R0, #0x10
	   BL LCD_Cmd_Out
	   
	   MOV R5, #0          
xAgn   LDR R1, =LCDmat     ;x loop
       MOV R0, #128
	   MLA R3, R6, R0, R5   ; R3=y*128 +x
	   LDRB R0, [R1,R3]
       BL LCD_Dat_Out
	   ADD R5, #1          ; inc x
	   CMP R5, #128
	   BLO xAgn
	   
	   ADD R6, #1          ; inc 6y
	   CMP R6, #8
	   BLO yAgn 
	   POP {LR, R0-R7}
	   BX LR

LCD_Blank
	   PUSH {LR, R0-R6}
	   LDR R1, =LCDmat
	   MOV R2, #0
	   MOV R0, #0        ;offset
blagn  STRB R2, [R1, R0]
	   ADD R0, #1
	   CMP R0, #1024
	   BLO blagn
	   
	   BL LCD_Refresh
	   
	   POP {LR, R0-R6}
	   BX LR

       ALIGN      
       END  