MAIN: mov r3 ,LENGTH
LOOP: jmp L1(#-1,r6)
mcr m1
sub r1, r4
bne END
endmcr
prn #-5
bne LOOP(r4,r5) 
m1
L1: inc K
bne LOOP(K,STR)
END: stop
STR: .string "abcdef"
LENGTH: .data 6,-9,15
K: .data 22
.extern L2
sub r1, L2
sub r1, L2

