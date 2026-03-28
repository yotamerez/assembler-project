; some error tests for first transition

; syntax errors
,nonononono: stop
stop,
data ,,,,
,,,,,,,
P P P P PLEASE:



; label errors
0SomeIllegalLabel: 					
hmmmmmmm : 						
amaterasu:.data	1					
LetsWriteSomeLongButIMeanAVeryLongLabelNamehmm:		
r1: mov							
mov: stop						
.string:						


; data errors
iguees: .DATA 						
amaterasu: 	.data 1,2?				
.data 1,2,						
.data 	+1+,1						
.data 	 --1						
.data    11++,3,4,5					
.data		  	999999,1			
.data							 


; string errors
whythis: .string “abcdef”				
.string ""						
sharingan: .string LMAO"				
Rasengan: .string OP"					


; operations

; mov  syntax: first_op = number,label,reg  second_op = label,register
ZAWADRUUU: mov	 					
ItsMeDio:  	mov r1					
HmmmmmmmIC: mov r3 ,r3 ,r3				
hmmmmmmmICCCNowIthink: mov r5,#1			

; cmp  syntax: first_op = number,label,reg  second_op = number,label,reg
StarPlatinum: cmp 					
cmp narutokun						
cmp sakurachan, sasukekun, pleaseno			

; add syntax: first_op = number,label,reg  second_op = label,reg
CRINGE: add						
SHHHHHH: add #-200					
OmegaLULIGuess: add gg,wp,****				
Shishishishi: add gg,#777				

; sub syntax: first_op = number,label,reg  second_op = label,reg			
CRINGE: sub						
SHHHHHH: sub MonkaW					
OmegaLULIGuess: sub gg,wp,really?			
Shishishishi: sub MonkaS,#666	

; not syntax second_op = label,reg
CRINGE: not						
SHHHHHH: not #-9					
OmegaLULIGuess: not gg,wp,really?			
Shishishishi: not MonkaS,#666

; clr syntax second_op = label,reg
clr #666
clr r0,r7
clr	

; lea syntax first_op = label  second_op = label,reg
lea r1
lea #-777
lea r1,r2,r3
lea

; inc syntax second_op = label,reg
inc #-666
inc
hmmmmmmmmmm: inc wp,wp
IC: inc wp,wp,ggggg

; dec,red syntax second_op = label,reg
dec #-666
red
hmmmmmmmmmm: dec wp,wp
bb: red wp,wp,ggggg

; jmp,bne,jsr syntax second_op_parameters
jmp #-666
jsr r5
hmmmmmmmmmm: bne rasengan( spaces,unlcuky)
IC: jmp sharingan(lol)
jsr sha(ringan
bne nnn,nnn)

; prn syntax second_op = number,label,reg
ok: prn
prn 1,2,3
prn A,B,C

; rts,stop no operands
rts A
stop B,b,b,b,bb 


		


