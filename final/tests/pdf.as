mcr m1
MAIN: mov	r3 ,LENGTH

LOOP: jmp	L1(#-1,r6)
endmcr

		mcr				m2

prn	#-5
bne	W(r4,r5)
sub	r1,r4
bne	L3		
endmcr

mcr		m3
L1: inc	K

.entry LOOP

bne	LOOP(K,W)
END: stop
STR: .string	"abcdef"

	endmcr


.entry LENGTH
.extern W
m1
m2
m3

LENGTH: .data	6,-9,15
K: .data	22

.extern L3
