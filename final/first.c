/* Yam Tiram 312127814 */

#include "header.h"
#include "macros.h"
#include "registers.h"
#include "commands.h"
#include "lables.h"
#include "inputAnalysis.h"
#include "stringModifications.h"


/*
This function will get a pointer to an assembly source code file after macros
spread, a char pointer to ".am" file name, an int pointer to the instructions
counter, an int pointer to the data counter, a pointer to macros linke list
root pointer and a pointer to lables linked list root pointer.

Will validate syntax of source code, and print a specific error message
describing the error, filename and line number if an error is found.
Advances the instructions and data counter acording to the amount of word each
instruction and data is going to take in the memory.

The file is read line by line:
- If the line is a comment line (starting with ";"), ignores it.
- If the line starts with a valid lable, set the lableFound flag.
- If a valid command line is found advance the instruction counter, and if the
	lableFound is set, insert the lable to the lable table with it's address as
	an instruction lable.
- If valid data or string is found, advances data counter and if the lableFound
	flag is	set, insert the lable to the lable table with it's address as a data
	lable.
- If a valid extern lable is found, insert it to the lable table as an external
	lable with address of 0, if the lableFound flag is set, print a	notice that
	there should not be a lable there.
- If a valid entry lable is found, insert it to the lable table as an entry
	lable with address of 0, if the lableFound flag is set, print a notice that
	there should no be a lable there.

In order to achive this, a linked list for lable table is implemented and some
input analysis functions are used.
*/
int firstPass(FILE *f, char *amFileName, int *IC, int *DC, Macro **macroRoot, LableTable **lableRoot)
{
	char line[LINE_SIZE];														/* Buffer to save line in. */
	char lineTokenized[LINE_SIZE];												/* Copy of line to use in strtok function. */
	char *linePointer;															/* Pointer to line. */
	char *token;
	int lineNumber = 0;
	int lableFound;																/* Flag for when a line starts with a lable. */
	int error = 0;
	int finished;																/* Flag for when line treatment has been finished. */
	*IC = STARTING_ADDRESS;
	*DC = 0;
	while(fgets(line, LINE_SIZE, f) != NULL)									/* Get next line. */
	{
		char lableName[MAX_LABLE_SIZE];
		finished = 0;
		lineNumber++;
		if(tooLong(line, amFileName, lineNumber))								/* Check line is longer than max line size. */
		{
			error = 1;
			while(line[strlen(line)-1] != '\n')									/* Look for end of line. */
				fgets(line, LINE_SIZE, f);
			continue;
		}
		removeWhitespaces(line);												/* Remove white-spaces from beginning and end of line. */
		linePointer = line;
		lableFound = 0;
		
		strcpy(lineTokenized, line);
		token = strtok(lineTokenized, WHITESPACES);								/* Break down line to tokens. */
		while(token != NULL)
		{
			linePointer += strlen(token);										/* Makes linePointer a pointer to the line starting from after token. */
			removeWhitespaces(linePointer);										/* Remove white-spaces from beginning and end of line. */
			switch(whatAreYou(token))											/* Check what is that token. */
			{
				case(comment):
					finished = 1;
					break;
				
				case(lable):
					if(!validUniversalLable(token, macroRoot, lableRoot,
						INSTRUCTION, amFileName, lineNumber))					/* Check if lable is of valid syntax. */
					{
						error = finished = 1;
						break;
					}
					strcpy(lableName, token);									/* Save lable name to insert to lable table later. */
					lableName[strlen(lableName)-1] = '\0';
					lableFound = 1;
					break;

				case(command):
					if(!validCommandOperands(linePointer, token, macroRoot,
						lableRoot, amFileName, lineNumber))						/* Check if command operands are of valid syntax. */
					{
						error = finished = 1;
						break;
					}
					if(lableFound)
					{
						insertLable(lableName, *IC, INSTRUCTION, lableRoot,
						amFileName,	lineNumber);								/* Insert lable to lable table as INSTRUCTION lable with it's address. */
						lableFound = 0;
					}
					*IC += InstructionSize(linePointer, token);					/* Increase IC by number of memory words required for the command. */
					finished = 1;
					break;
			
				case(directive):
					switch(whatDirective(token, amFileName, lineNumber))		/* Check what directive is it. */
					{
						case(invalid):
							error = finished = 1;
							break;
						
						case(data):
							if(!validData(linePointer, amFileName, lineNumber))	/* Check if the data input is of valid syntax. */
							{
								error = finished = 1;
								break;
							}
							if(lableFound)
							{
								insertLable(lableName, *DC, DATA, lableRoot,
									amFileName, lineNumber);					/* Insert lable to lable table as DATA lable with it's address. */
								lableFound = 0;
							}
							*DC += dataSize(linePointer);						/* Increase DC by number of memory words required for data. */
							finished = 1;
							break;
					
						case(string):
							if(!validString(linePointer, amFileName, lineNumber)) /* Check if the string input is of valid syntax. */
							{
								error = finished = 1;
								break;
							}
							if(lableFound)
							{
								insertLable(lableName, *DC, DATA, lableRoot,
									amFileName, lineNumber);					/* Insert lable to lable table as DATA lable with it's address. */
								lableFound = 0;
							}
							*DC += strlen(linePointer) - 1;						/* Increase DC by number of memory words required for string. */
							finished = 1;
							break;
					
						case(ext):
							if(lableFound)
							{
								printf("%s:%d: LABLE NOTICE: Exccess lable for extern directive.\n", amFileName, lineNumber);
								lableFound = 0;
							}
							if(!validExternFirstPass(linePointer, macroRoot,
								lableRoot, amFileName, lineNumber))				/* Check if extern input is of valid syntax. */
							{
								error = finished = 1;
								break;
							}
							token = strtok(linePointer, " \t");
							insertLable(token, 0, EXTERN, lableRoot, amFileName,
								lineNumber);									/* Insert lable to lable table as an EXTERN lable with address 0. */
							break;
					
						case(entry):
							if(lableFound)
							{
								printf("%s:%d: LABLE NOTICE: Exccess lable for entry directive.\n", amFileName, lineNumber);
								lableFound = 0;
							}
							if(!validEntryFirstPass(linePointer, macroRoot,
								lableRoot, amFileName, lineNumber))				/* Check if entry input is of valid syntax. */
							{
								error = finished = 1;
								break;
							}
							token = strtok(linePointer, " \t");
							insertLable(token, 0, ENTRY, lableRoot, amFileName,
								lineNumber);									/* Insert lable to lable table as an ENTRY lable with address 0; */
							finished = 1;
							break;
					}
					break;
							
				case(variable):
				case(reg):
					break;
			
				default:
					printf("OH OH...\n");
			}
			if(finished)
				token = NULL;													/* To skip to next line. */
			else
				token = strtok(NULL, " \t");									/* To keep checking line. */
		}
		if(lableFound && !error)
		{
			printf("%s:#%d: LABLE ERROR: No information after \"%s\".\n", amFileName, lineNumber, lableName);
			error = 1;
		}
	}
	updateLables(lableRoot, *IC);												/* Increase DATA lables address by IC. */
	if(error)
		return 0;
	else
		return 1;
}
