
mcr mov
endmcr
mcr cmp
endmcr
mcr add
endmcr
mcr sub
endmcr
mcr not
endmcr
mcr clr
endmcr
mcr lea
endmcr
mcr inc
endmcr
mcr dec
endmcr
mcr jmp
endmcr
mcr bne
endmcr
mcr red
endmcr
mcr prn
endmcr
mcr jsr
endmcr
mcr rts
endmcr
mcr stop
endmcr
mcr r0
endmcr
mcr r1
endmcr
mcr r2
endmcr
mcr r3
endmcr
mcr r4
endmcr
mcr r5
endmcr
mcr r6
endmcr
mcr r7
endmcr
mcr .entry
endmcr
mcr .extern
endmcr
;this is a good macro:
mcr M1
endmcr
;this is the same one:
mcr M1
endmcr

mcr this is not good
endmcr

mcr thisIsOK
	mov harta, barta
endmcr but this is not
;check to make sure macro was not created:
thisIsOK

mcr thisMacroHasNoEnd
