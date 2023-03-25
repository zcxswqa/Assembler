# Assembler
	This program will translate the assembly files to 14 bits binary coded words files.
	In order to achive this, the program is divided to three major parts:
	
	1. Pre-Assembler - will find and monitor any macro declarations and calls,
		and will produce a file without those declerations and calls but with the
		content of each macro it was called.
		
	2. First-Pass - will validate the syntax of the assembly code and will print
		an error message for each invalid line describing what exactly is wrong.
		
	3. Second-Pass - will produce the final output files:
    .obj: in which the code is translated to binary
    .ent: in which the usage of internal lables is documented.
    .ext: in which the usage of external lables is documented.
	
	The program is split across many files each with it's own documentation.
