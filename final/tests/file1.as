.entry L1
mcr m1
L1: mov r2, r1
	cmp MOV	, r2
	add	r2 ,r1
endmcr

mcr m2
sub #-3, r4
not r4
		L2: 	clr		 r4
endmcr

mcr m3
lea		L2, r5
inc r7
dec 	r7		
endmcr

mcr m4

jmp L1(#5,r6)

bne L2

.extern MOV
red	r4

prn r4

	endmcr

mcr m5
STRING: .string "abc123-=()"%$|\/"^'#	anything Goes"
DATA: .data 5, 6, -2048, 2047, -1
endmcr


mcr m6
jsr L2(L1,r5)
jsr L2
rts
endmcr

			mcr m7
			stop
	endmcr
	
mcr m8
	.data 1,2,3
	.string "this sting has no lable\n"
endmcr

mcr thisMacroWasNotUsed
endmcr



m1
m2
m3
MAIN: clr r6
m4
rts
m5
m6
m7
m8

.entry L2
.extern ThisLableIsNotUsed
