# Assembler
	This program will translate assembly files to 14 bits binary coded words files.
	In order to achive this, the program is divided to three major parts:
	
	1. Pre-Assembler -
		will find and monitor any macro declarations and calls,
		and will produce a file without those declerations and calls but with the
		content of each macro it was called.
		
	2. First-Pass -
		will validate the syntax of the assembly code and will print
		an error message for each invalid line describing what exactly is wrong.
		
	3. Second-Pass -
		will produce the final output files:
		.obj: in which the code is translated to binary
		.ent: in which the usage of internal lables is documented.
  		.ext: in which the usage of external lables is documented.


## binary words format
	A word is composed of 14 bits as follows: |13-12|11-10|9-8-7-6|5-4|3-2|1-0|
	bits 1,0 - A,R,E:
		A = Absolute = 00, content isn't dependent on the machine code location (immidiate number, register, etc.).
		E = External = 01, content depends on an external lable.
		R = Relocatable = 10, content depends on the machine code location (internal lable).
		
	bits 3,2 - Destination operand's addressing mode:
		00 = immidiate number.
		01 = lable.
		11 = register.
		
	bits 5,4 - Source operand's addressing mode, same as destination operand's addressing mode.
	
	bits 9,8,7,6 - One of 16 operations code:
		0000 = mov	|	1000 = dec
		0001 = cmp	|	1001 = jmp
		0010 = add	|	1010 = bne
		0011 = sub	|	1011 = red
		0100 = not	|	1100 = prn
		0101 = clr	|	1101 = jsr
		0110 = lea	|	1110 = rts
		0111 = inc	|	1111 = stop
		
	bits 11,10 - jmp's/bne's/jsr's destination parameter addressing mode, same as destination operand's addressing mode.
	
	bits 13,12 - jmp's/bne's/jsr's source parameter addressing mode, same as destination operand's addressing mode.
