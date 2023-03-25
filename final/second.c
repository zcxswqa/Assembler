/* Yam Tiram 312127814 */

#include "header.h"
#include "commands.h"
#include "registers.h"
#include "macros.h"
#include "lables.h"
#include "inputAnalysis.h"
#include "stringModifications.h"
#include "coding.h"
#include "second.h"

/*
This function will get a pointer to an assembly source code file after macros
spread, a char pointer to the file name without extentions, an int pointer to
the instructions counter, an int pointer to the data counter, a pointer to
macros linke list root pointer and a pointer to lables linked list root pointer.

The function will create the following files:
".obj" file - will contain the binary coding of the assembly source code and
	the address of each memory word.
".ent" file - will contain all the ".entry" declared lables and their declaration
	address.
".ext" file - will contain all the ".extern" declared lables that were used in
	in the assembly source code.

The function will validate that each lable used as operand is declared in the
file, if not, will print an error message and remove the newly created files.
The function will as well check if every ".extern" declared lable was used,
if not, will print a notice message but will not delete any file.

*/
int second(FILE *amFile, char *filename, int *IC, int *DC, Macro **macroRoot, LableTable **lableRoot)
{
	char line[LINE_SIZE];														/* Buffer to save line in. */
	char lineTokenized[LINE_SIZE];												/* Copy of line to use in strtok function. */
	char *token;
	char *linePointer;															/* Pointer to line. */
	int lineNumber = 0;
	char *amFileName;
	char *objFileName;
	char *entryFileName;
	char *externFileName;
	int error = 0;																/* Decide whether to delete ".obj", ".ext" and ".ent" files. */
	DataWordsTable *DataWordRoot = NULL;
	FILE *objFile;
	
	if((amFileName = malloc(strlen(filename) + 4)) == NULL)
	{
		printf("MEMORY ALLOCATION ERROR: \"%s\" was not created\n", amFileName);	
		exit(1);
	}
	else if((objFileName = malloc(strlen(filename) + 5)) == NULL)
	{
		printf("MEMORY ALLOCATION ERROR: \"%s\" was not created\n", objFileName);	
		exit(1);
	}
	else if((entryFileName = malloc(strlen(filename)+5)) == NULL)
	{
		printf("MEMORY ALLOCATION ERROR: \"%s\" was not created\n", entryFileName);	
		exit(1);
	}
	else if((externFileName = malloc(strlen(filename)+5)) == NULL)
	{
		printf("MEMORY ALLOCATION ERROR: \"%s\" was not created\n", externFileName);
		exit(1);
	}
	
	strcpy(amFileName, filename);
	strcat(amFileName, ".am");	
	
	strcpy(objFileName, filename);
	strcat(objFileName, ".obj");
	
	strcpy(entryFileName, filename);
	strcat(entryFileName, ".ent");
	remove(entryFileName);														/* In case the program is run more than one time on the same source file, because the file is opened in append mode. */		
	
	strcpy(externFileName, filename);
	strcat(externFileName, ".ext");
	remove(externFileName);														/* In case the program is run more than one time on the same source file, because the file is opened in append mode. */		
	
	objFile = fopen(objFileName, "w+");
	if(objFile == NULL)
	{
		printf("FILE OPENING ERROR: \"%s\" was not opened\n", objFileName);
		exit(1);
	}
	
	fprintf(objFile, "\t\t%d %d", *IC - STARTING_ADDRESS, *DC);					/* Print the number of instruction word and data words in the first line of ".obj" file. */
	*IC = STARTING_ADDRESS;
	*DC = 0;
	rewind(amFile);																/* go back to the beginning of file. */
	
	while(fgets(line, LINE_SIZE, amFile) != NULL)								/* Get next line. */
	{
		lineNumber++;
		removeWhitespaces(line);												/* Remove white-spaces from beginning and end of line. */
		linePointer = line;
		
		strcpy(lineTokenized, line);
		token = strtok(lineTokenized, WHITESPACES);
		linePointer += strlen(token);
		removeWhitespaces(linePointer);											/* Remove white-spaces from beginning and end of line. */		

		if(whatAreYou(token) == comment)										/* Ignore commented lines. */
			continue;
		else if(whatAreYou(token) == lable)										/* Ignore lable at the beginning of line. */
		{
			token = strtok(NULL, WHITESPACES);
			linePointer += strlen(token);
			removeWhitespaces(linePointer);										/* Remove white-spaces from beginning and end of line. */		
		}
		if(whatAreYou(token) == command)
		{
			if(!handleCommand(linePointer, getCommandNumber(token), IC,
				lableRoot, objFile, externFileName, amFileName, lineNumber))	/* Make sure lables are declared and write code to ".obj" file. */
				error = 1;		
		}
		else if(whatAreYou(token) == directive)
			switch(whatDirective(token, amFileName, lineNumber))
			{
				case(data):
					handleData(linePointer, DC, &DataWordRoot);					/* Save addresses and code in a linked list for later. */
					break;
				case(string):
					handleString(linePointer, DC, &DataWordRoot);				/* Save addresses and code in a linked list for later. */
					break;
				case(entry):
					if(!handleEntry(linePointer, lableRoot, entryFileName,
						amFileName, lineNumber))								/* Make sure lable is declared in the source code file and write information to ".ent" file. */
						error = 1;
					break;
				case(ext):														/* Handled when encountered as an operand and checked in checkExternsUses function. */
					break;
				case(invalid):
					printf("MAJOR ERROR\n");
					break;
			}
		token = strtok(NULL, WHITESPACES);
	}
	checkExternsUses(lableRoot, amFileName);									/* Make sure all ".extern" declared lables are used. */
	if(error)
	{
		fclose(objFile);
		remove(objFileName);
		remove(entryFileName);
		remove(externFileName);
		free(amFileName);
		free(objFileName);
		free(entryFileName);
		free(externFileName);
		freeDataWords(&DataWordRoot);											/* Free memory allocations for data words linked list. */
		return 0;
	}
	else
	{
		free(amFileName);
		free(objFileName);
		free(entryFileName);
		free(externFileName);
		writeDataToFile(objFile, &DataWordRoot, *IC);							/* Write the data code to the ".obj" file. */
		fclose(objFile);
		return 1;
	}
}


/*
Handle instruction line, code and write command word and then if needed, any
operand to the ".obj" file.
All "write" functions used are writing the binary code to the ".obj" file.
All "code" functions used are using "write" functions to do the same.
Uses "lableWasDeclared" function to verify all lables given as operands
are declared in the source code file.
Returns 0 id not, and 1 otherwise.
*/
int handleCommand(char *justOperands, int commandNumber, int *IC, LableTable **lableRoot, FILE *objFile, char *externFileName, char *amFileName, int lineNumber)
{
	char *OP1;
	char *OP2;
	char *OP3;
	char justOperandsTokenize[LINE_SIZE];										/* Copy of operands to use in strtok function. */
	strcpy(justOperandsTokenize, justOperands);
	writeAddressToFile(objFile, IC);											/* Write the current instruction count to the object file as the address of the command word. */
	switch(howManyOperands(commandNumber))
	{
		case(0):
			writeZerosToFile(objFile, 2*OPERAND_ADDRESSING_SIZE);				/* Parameters bits are zeroed. */
			writeOPCodeToFile(objFile, commandNumber);							/* Write the opcode. */
			writeZerosToFile(objFile, 2*OPERAND_ADDRESSING_SIZE + AER_SIZE);	/* operands and AER bits are zeroed. */
			break;
		case(1):
			if(isJump(commandNumber))											/* If command is of jumping type. */
			{
				OP1 = strtok(justOperandsTokenize, "(");						/* If there is no "(", OP1 is the whole operands line. */
				if(!lableWasDeclared(OP1, lableRoot, amFileName, lineNumber))	/* if OP1 was not declared in source code file. */
					return 0;
				OP2 = strtok(NULL, ",");
				OP3 = strtok(NULL, ")");
				if(OP2 != NULL && OP3 != NULL)									/* If the instruction is a jump command with parameters. */
				{
					writeOperandAddressingToFile(objFile, OP2);					/* Write first parameter addressing to parameter1 bits of command word. */
					writeOperandAddressingToFile(objFile, OP3);					/* Write second parameter addressing to parameter2 bits of command word. */
					writeOPCodeToFile(objFile, commandNumber);					/* Write the opcode. */
					writeZerosToFile(objFile, OPERAND_ADDRESSING_SIZE);			/* Source operand bits are zeroed. */
					writeOperandAddressingToFile(objFile, justOperands);		/* Write jump lable addressing to destination operand bits. */
					writeZerosToFile(objFile, AER_SIZE);						/* AER bits are zeroed. */
					
					codeLable(objFile, IC, OP1, lableRoot, externFileName);		/* Code and write the lable address to the object file. */
					
					if(Addressing(OP2) == directRegister &&
							Addressing(OP3) == directRegister)					/* If both operands are registers. */
						codeTwoRegisters(objFile, IC, OP2, OP3);				/* Code and write the two registers to one word in the object file. */
					else if(whatAreYou(OP2) == lable && !lableWasDeclared(OP2, lableRoot, amFileName, lineNumber))
						return 0;
					else if(whatAreYou(OP3) == lable && !lableWasDeclared(OP3, lableRoot, amFileName, lineNumber))
						return 0;
					else 
					{
						codeOneOperand(objFile, IC, OP2, source, lableRoot,
							externFileName);									/* Code and write the first parameter data or address to the object file. */
						codeOneOperand(objFile, IC, OP3, dest, lableRoot,
							externFileName);									/* Code and write the second parameter data or address to the object file. */
					}
				}
				else 															/* If the instruction is a jump command without parameters. */
				{
					writeZerosToFile(objFile, 2*OPERAND_ADDRESSING_SIZE);		/* Parameters bits are zeroed. */
					writeOPCodeToFile(objFile, commandNumber);					/* Write the opcode. */
					writeZerosToFile(objFile, OPERAND_ADDRESSING_SIZE);			/* Source operand bits are zeroed. */
					writeOperandAddressingToFile(objFile, OP1);					/* Write jump lable addressing to destination operand bits. */
					writeZerosToFile(objFile, AER_SIZE);						/* AER bits are zeroed. */
					codeLable(objFile, IC, OP1, lableRoot, externFileName);		/* Code and write the lable address to the object file. */
				}
			}
			else 																/* If command is not of jumping type. */
			{
				OP1 = justOperands;
				if(whatAreYou(OP1) == lable && !lableWasDeclared(OP1, lableRoot, amFileName, lineNumber))
					return 0;
				writeZerosToFile(objFile, 2*OPERAND_ADDRESSING_SIZE);			/* Parameters bits are zeroed. */
				writeOPCodeToFile(objFile, commandNumber);						/* Write the opcode. */
				writeZerosToFile(objFile, OPERAND_ADDRESSING_SIZE);				/* Source operand bits are zeroed. */
				writeOperandAddressingToFile(objFile, OP1);						/* Write operand addressing to destination operand bits. */
				writeZerosToFile(objFile, AER_SIZE);							/* AER bits are zeroed. */
				codeOneOperand(objFile, IC, OP1, dest, lableRoot, externFileName);/* Code and write the operand to the object file. */
			}
			break;
		case(2):
			OP1 = strtok(justOperandsTokenize, " \t,");
			if(whatAreYou(OP1) == lable && !lableWasDeclared(OP1, lableRoot, amFileName, lineNumber))
				return 0;
			OP2 = strtok(NULL, " \t,");
			if(whatAreYou(OP2) == lable && !lableWasDeclared(OP2, lableRoot, amFileName, lineNumber))
				return 0;
			writeZerosToFile(objFile, 2*OPERAND_ADDRESSING_SIZE);				/* Parameters bits are zeroed. */
			writeOPCodeToFile(objFile, commandNumber);							/* Write the opcode. */
			writeOperandAddressingToFile(objFile, OP1);							/* Write first operand addressing to source operand bits. */
			writeOperandAddressingToFile(objFile, OP2);							/* Write second operand addressing to destination operand bits. */
			writeZerosToFile(objFile, AER_SIZE);								/* AER bits are zeroed. */
			if(Addressing(OP1) == directRegister &&
					Addressing(OP2) == directRegister)							/* If both operands are registers. */
				codeTwoRegisters(objFile, IC, OP1, OP2);						/* Code and write the two registers to one word in the object file. */
			else
			{
				codeOneOperand(objFile, IC, OP1, source, lableRoot, externFileName);/* Code and write the first operand to the object file. */
				codeOneOperand(objFile, IC, OP2, dest, lableRoot, externFileName);/* Code and write the second operand to the object file. */
			}
			break;
	}
	return 1;
}


/*
Code and saves the data inputs to a linked list of data words in order to write
them all together at the end of the object file.
Uses "insertDataWord" function with the pointer to DC, the function advances DC
by one each time it is called.
*/
void handleData(char *line, int *DC, DataWordsTable **DataWordRoot)
{
	char *token;
	removeWhitespaces(line);													/* Remove white-spaces from beginning and end of line. */
	token = strtok(line, ",");
	
	while(token != NULL)
	{
		insertDataWord(DataWordRoot, DC, atoi(token));							/* Saves the integer representation of the data chunk to the data words linked list with it's address and advances DC by one. */
		token = strtok(NULL, ",");
	}
}


/*
Code and saves the string inputs to a linked list of data words in order to
write them all together at the end of the object file.
Uses "insertDataWord" function with the pointer to DC, the function advances DC
by one each time it is called.
*/
void handleString(char *line, int *DC, DataWordsTable **DataWordRoot)
{
	int i;
	for(i=1; i<strlen(line)-1; i++)
		insertDataWord(DataWordRoot, DC, line[i]);								/* Saves the integer representation of the charcter to the data words linked list with it's address and advances DC by one. */
	insertDataWord(DataWordRoot, DC, '\0');										/* Insert the terminator for the string as well. */
}


/*
Make sure a ".entry"-declared lable was declared in the source code file and 
write the lable name and address of that declaration to the ".ent" file.
Retuen 1 if lable was declared properly.
Prints an error message and returns 0 otherwise.
*/
int handleEntry(char *lable, LableTable **lableRoot, char *entryFileName, char *amFileName, int lineNumber)
{
	LableTable *currentLable = *lableRoot;
	FILE *entFile;
	while(currentLable != NULL)
	{
		if(!strcmp(lable, currentLable->name) && currentLable->type != ENTRY)
		{
			entFile = fopen(entryFileName, "a"); 
			if(entFile == NULL)
			{
				printf("FILE OPENING ERROR: \"%s\" was not opend\n", entryFileName);
				exit(1);
			}
			fprintf(entFile, "%s	%d\n", lable, currentLable->address);
			fclose(entFile);
			return 1;
		}
		currentLable = currentLable->nextLable;
	}
	printf("%s:#%d ENTRY ERROR: \"%s\" was not declared in this file\n", amFileName, lineNumber, lable);
	return 0;
}


/*
makes sure that a lable that was given as an operand was declared in the source
code file.
Prints an error message and returns 0 if not.
Retuens 1 otherwise.
*/
int lableWasDeclared(char *operand, LableTable **lableRoot, char *amFileName, int lineNumber)
{
	if(!collision(operand, lableRoot, OPERAND))									/*  */
	{
		printf("%s:#%d: OPERAND ERROR: \"%s\" was not declared\n", amFileName, lineNumber, operand);
		return 0;
	}
	return 1;
}


/*
Inserts a character or an integer to the data word table with it's address.
Uses "NOT" and "left shift" operators to get the binary representation of
the character or integer.
Advances the data counter by one.
*/
void insertDataWord(DataWordsTable **DataWordRoot, int *DC, unsigned value)
{
	DataWordsTable *currentWord = *DataWordRoot;
	DataWordsTable *newWord;
	int i;
	int check = 1;																/* For "OR"ing with the number to get it's binary representation. */
	
	if((newWord = malloc(sizeof(DataWordsTable))) == NULL)
	{
		printf("MEMORY ALLOCATION ERROR: newWord was not created\n");
		exit(1);
	}
	
	newWord->nextWord = NULL;
	newWord->address = *DC;
	strcpy(newWord->word, "..............");
	
	if(value<0)
		value = ~(value)+1;														/* Get the 2's complement representation of the negative integer. */
	check <<= (WORD_SIZE-2);													/* Left shift the number "1" by 12 bits amout. */
	for(i=0; i<WORD_SIZE-1; i++)
	{
		if((value<<i)&check)
			newWord->word[i] = '/';
		else
			newWord->word[i] = '.';
	}
	
	if(*DataWordRoot == NULL)													/* If this is the first word in the table. */
	{
		*DataWordRoot = newWord;
		*DC += 1;
		return;
	}
	do 																			/* Look for the last word. */
	{
		if(currentWord->nextWord != NULL)
			currentWord = currentWord->nextWord;	
	}while(currentWord->nextWord != NULL);
		
	currentWord->nextWord = newWord;
	*DC += 1;																/* Advances DC by one. */
}


/*
Write the data words from the data words table to the object file and free the
memory of data words table linked list.
*/
void writeDataToFile(FILE *objFile, DataWordsTable** DataWordRoot, int IC)
{
	DataWordsTable *currentWord = *DataWordRoot;
	while(currentWord != NULL)
	{
		DataWordsTable *delete = currentWord;
		fprintf(objFile, "\n0%d\t\t%s", currentWord->address+IC, currentWord->word);
		currentWord = currentWord->nextWord;
		free(delete);
	}
	*DataWordRoot = NULL;
}


/*
Free the memory of data words table linked list.
*/
void freeDataWords(DataWordsTable **DataWordRoot)
{
	DataWordsTable *currentDataWord = *DataWordRoot;
	while(currentDataWord != NULL)
	{
		DataWordsTable *delete = currentDataWord;
		currentDataWord = currentDataWord->nextWord;
		free(delete);
	}
	*DataWordRoot = NULL;
}
