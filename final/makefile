assembly: assembly.o preAssembler.o first.o second.o  
		gcc -ansi -Wall -pedantic -g assembly.o preAssembler.o first.o second.o coding.o macros.o registers.o commands.o lables.o inputAnalysis.o stringModifications.o -o assembly
		
assembly.o: assembly.c header.h macros.h assembly.h
		gcc -c -ansi -Wall -pedantic -g assembly.c -o assembly.o 
		
preAssembler.o: preAssembler.c stringModifications.o macros.o header.h macros.h assembly.h preAssembler.h stringModifications.h
		gcc -c -ansi -Wall -pedantic -g preAssembler.c -o preAssembler.o
		
first.o: first.c registers.o commands.o lables.o inputAnalysis.o stringModifications.o header.h macros.h registers.h commands.h lables.h inputAnalysis.h stringModifications.h assembly.h
		gcc -c -ansi -Wall -pedantic -g first.c -o first.o
		
second.o: second.c coding.o macros.o commands.o registers.o lables.o inputAnalysis.o stringModifications.o header.h second.h coding.h macros.h commands.h registers.h lables.h inputAnalysis.h stringModifications.h 
		gcc -c -ansi -Wall -pedantic -g second.c -o second.o
		
coding.o: coding.c macros.o lables.o commands.o registers.o stringModifications.o header.h coding.h macros.h lables.h commands.h registers.h stringModifications.h
		gcc -c -ansi -Wall -pedantic -g coding.c -o coding.o
		
inputAnalysis.o: inputAnalysis.c stringModifications.o registers.o header.h macros.h inputAnalysis.h stringModifications.h registers.h assembly.h
		gcc -c -ansi -Wall -pedantic -g inputAnalysis.c -o inputAnalysis.o
		
commands.o: commands.c registers.o header.h registers.h commands.h assembly.h
		gcc -c -ansi -Wall -pedantic -g commands.c -o commands.o
		
registers.o: registers.c header.h registers.h assembly.h
		gcc -c -ansi -Wall -pedantic -g registers.c -o registers.o
	
lables.o: lables.c macros.o registers.o header.h macros.h registers.h lables.h assembly.h
		gcc -c -ansi -Wall -pedantic -g lables.c -o lables.o

stringModifications.o: stringModifications.c stringModifications.h
		gcc -c -ansi -Wall -pedantic -g stringModifications.c -o stringModifications.o
	
macros.o: macros.c macros.h header.h
		gcc -c -ansi -Wall -pedantic -g macros.c -o macros.o
