/* Yam Tiram 312127814 */

#include "header.h"
#include "macros.h"
#include "preAssembler.h"
#include "commands.h"
#include "registers.h"
#include "stringModifications.h"

/*
This function will get a pointer to an assembly source code file, a char pointer
to the file name without extentions, and a pointer to a macro linked list root
poiner.
Will check for any macro declerations and call errors, if non were found, a 
".as" extention file is created in which all macros declarations are removed
and all macros call are replaced with macros code.

The file is read line by line and copies any line to the new file if that line
is not part of a macro decleration or call.
If any part of the line is a macro call it will not be copied and insted the
macro content will be copied from the original file.

In order to achive this, a linked list of macros is implemented in which each
time a macro decleration is encountered, that macro is inserted to the list with
it's name, starting and ending addresses in the original file.
Then when a macro name is encountered in the original file, the content from the
original file in the range between the starting and ending addresses of that 
macro decleration are copied.
*/
int preAssemble(FILE *asFile, char *filename, Macro **macroRoot)
{
	char *amFileName;
	char *asFileName;
	FILE *amFile;
	
	int location;																/* Current read location in the original file. */
	int macroWaswriten;															/* Flag for when a macro code was copied. */
	int lineNumber = 0;
	
	char line[LINE_SIZE];														/* Buffer to save line in. */
	char lineTokenized[LINE_SIZE];												/* Copy of line to use in strtok function. */
	
	char *token;																/* Used with strtok function. */

	int macroDecleration = 0;													/* Flag for when lines are part of a macro declaration. */
	char macroName[LINE_SIZE];
	int startingAddress, endingAddress;
	Macro *macroWrite;
		
	int errorLine;																/* Used in error messages. */
	int error = 0;																/* Decide whether to delete ".am" file. */
	
	if((amFileName = malloc(strlen(filename)+4)) == NULL)
	{
		printf("MEMORY ALLOCATION ERROR: \"%s\" was not created\n", amFileName);
		exit(1);
	}
	else if((asFileName = malloc(strlen(filename)+4)) == NULL)
	{
		printf("MEMORY ALLOCATION ERROR: \"%s\" was not created\n", asFileName);
		exit(1);
	}
	strcpy(asFileName, filename);
	strcpy(amFileName, filename);
	
	strcat(asFileName, ".as");
	strcat(amFileName, ".am");
	amFile = fopen(amFileName, "w");
	
	while(location = ftell(asFile), fgets(line, LINE_SIZE, asFile) != NULL)		/* Saves current file stream location and get next line.*/
	{	
		macroWaswriten = 0;
		lineNumber += 1;
		
		if(isempty(line))														/* If line is empty, get the next line. */
			continue;
		
		strcpy(lineTokenized, line);
		
		token = strtok(lineTokenized, WHITESPACES);
		if(!strcmp(token, "mcr"))												/* Look for beginning of macro definition. */
		{	
			token = strtok(NULL, WHITESPACES);									/* Read macro name. */
			if((token = strtok(NULL, WHITESPACES)) != NULL)						/* Check if there is something after macro name. */
			{
				printf("%s:#%d: MACRO ERROR: Exccess information after macro name.\n", asFileName, lineNumber);
				error = 1;
				continue;
			}
			fseek(asFile, location, SEEK_SET);									/* Set file stream to beginning of line. */
			fscanf(asFile, "%s", macroName);									/* Skip "mcr". */
			fscanf(asFile, "%s", macroName);									/* Get macro name. */
			errorLine = lineNumber;
			fgets(line, LINE_SIZE, asFile);										/* Ste file stream to next line. */
			startingAddress = ftell(asFile);
			macroDecleration = 1;
		}
		else if(!strcmp(token, "endmcr") && macroDecleration)					/* Look for end of macro definition. */
		{
			if((token = strtok(NULL, WHITESPACES)) != NULL)						/* Checl if there is something after "endmcr". */
			{
				
				printf("%s:#%d: MACRO ERROR: Exccess information after macro declaration.\n", asFileName, lineNumber);
				error = 1;
				macroDecleration = 0;
				continue;
			}
			endingAddress = ftell(asFile) - strlen(macroName);
			if(savedWord(macroName, asFileName, errorLine))						/* Check if macro name is a saved word. */
				error = 1;
			else
				insertMacro(macroRoot, macroName, startingAddress,				/* Insert macro to macro linked list. */
					endingAddress, asFileName, errorLine);						
			macroDecleration = 0;
			continue;
		}
		while(token != NULL)													/* Look for macro call somewhere in line. */
		{
			if((macroWrite = isMacro(token, macroRoot)) != NULL)				/* Look for token in macro linked list. */
			{
				writeMacro(asFile, amFile, macroWrite);							/* Write macro content to new file. */
				macroWaswriten = 1;
				token = strtok(NULL, WHITESPACES);
				continue;
			}
			else if(!macroDecleration)											/* If line was not a macro declaration. */
				fprintf(amFile, "%s ", token);
			token = strtok(NULL, WHITESPACES);
		}
		if(!macroDecleration && !macroWaswriten)
			fprintf(amFile, "\n");
	}
	fclose(amFile);
	
	if(macroDecleration)														/* If last macro declaration was not terminated. */
	{
		printf("%s:#%d: MACRO ERROR: \"%s\" declaration was not terminated with 'endmcr'.\n", asFileName, lineNumber, macroName);
		error = 1;
	}

	free(asFileName);
	if(error)
	{
		remove(amFileName);														/* If a macro error occured, delet new file. */
		free(amFileName);
		return 0;
	}
	else
	{
		free(amFileName);
		return 1;
	}
}


/*
Checks if potential macro name is a command name, a register name or a directive.
If it is, prints an error message.
Returns 1 if potential macro name is a saved word and 0 otherwise.
*/
int savedWord(char *macroName, char *asFileName, int lineNumber)
{
	if(getCommandNumber(macroName) != -1)										/* If macro name is a command name. */
	{
		printf("%s:#%d: MACRO ERROR: \"%s\" is a command name.\n", asFileName, lineNumber, macroName);
		return 1;
	}
	else if(getRegisterNumber(macroName) != -1)									/* If macro name is a register name. */
	{
		printf("%s:#%d: MACRO ERROR: \"%s\" is a register name.\n", asFileName, lineNumber, macroName);
		return 1;
	}
	else if(!strcmp(macroName, ".extern") || !strcmp(macroName, ".entry") || !strcmp(macroName, "mcr") || !strcmp(macroName, "endmcr"))
	{
		printf("%s:#%d: MACRO ERROR: \"%s\" is a saved word.\n", asFileName, lineNumber, macroName);
		return 1;
	}
	else
		return 0;
}
