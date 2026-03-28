; testing without errors i guess
mcr m1
hmm: mov hmm hmm 
.extern okthen
.entry hmm
jmp hmm(okthen,QWWWW)
endmcr
A: .data 1
B: stop
C: cmp #1, 	#-100
D: add r6, A
E: sub r1,r2
F: not E
G: clr G 
H: lea A,r7

mcr IGUESS
I: .string "Shurururururu"
QQ: .data 100,200,300,400,500
TT: dec BB
.extern BB
endmcr
m1
ShiShi: jmp BB(BB,BB)
.entry QQ
;ZZZZZZZZZZZZZZZZZZZZZZZZZZ

ff15please: bne H
IGUESS
jsr A(F,TT)
stop
HM: rts 
prn QWWWW
.extern QWWWW


