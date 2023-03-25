/* Yam Tiram 312127814 */

#include "header.h"
#include "macros.h"
#include "stringModifications.h"


/*
Insert a macro to the macros linked list with it's starting and ending addresses.
Check if macro was already declared, if it was, print an error message and if
not insert it.
*/
void insertMacro(Macro **MacroRoot, char *macroName, int startOfMacro, int endOfMacro, char *asFileName, int lineNumber)
{
	Macro *currentMacro = *MacroRoot;
	Macro *newMacro = malloc(sizeof(Macro));
	if(newMacro == NULL)
	{
		printf("%s:#%d: MACRO CREATION ERROR: \"%s\" was not created.\n", asFileName, lineNumber, macroName);
		return;
	}

	newMacro->nextMacro = NULL;
	strcpy(newMacro->name, macroName);
	newMacro->start = startOfMacro;
	newMacro->end = endOfMacro;
	newMacro->used = 0;
	
	if(*MacroRoot == NULL)														/* If this is the first macro in the file. */
	{
		*MacroRoot = newMacro;
		return;
	}
	do																			/* Check if macro is already in list. */
	{
		if(!strcmp(newMacro->name, currentMacro->name))
		{
			printf("%s:#%d: MACRO ERROR: \"%s\" was already declared.\n", asFileName, lineNumber, macroName);
			free(newMacro);
			return;
		}
		if(currentMacro->nextMacro != NULL)
			currentMacro = currentMacro->nextMacro;	
	}while(currentMacro->nextMacro != NULL);
	
	currentMacro->nextMacro = newMacro;
}


/*
Check if a given string is a macro name which is in the macros linked list.
Return that macro if found, and NULL if non was found.
*/
Macro *isMacro(char *word, Macro **macroRoot)
{
	Macro *currentMacro = *macroRoot;
	char *noWhiteSpaces = malloc(strlen(word)+1);
	
	strcpy(noWhiteSpaces, word);
	removeWhitespaces(noWhiteSpaces);											/* Remove white-spaces from beginning and end of string. */
	while(currentMacro != NULL)
	{
		if(!strcmp(noWhiteSpaces, currentMacro->name))
			break;
		currentMacro = currentMacro->nextMacro;
	}
	free(noWhiteSpaces);
	return currentMacro;
}


/*
Copy the content of macro from original file to new file.
*/
void writeMacro(FILE *read, FILE *write, Macro *current)
{
	char line[MAX_LINE_SIZE];
	int location;
	
	location = ftell(read);														/* Saves initial file stream location. */
	fseek(read, current->start, SEEK_SET);										/* Set file stream to the beginning of macro content. */
	while((fgets(line, LINE_SIZE, read) != NULL) && ftell(read) < current->end)	/* As long as line is not last line of file and file stream location is not after macro content ending. */
		if(!isempty(line))
			fputs(line, write);													/* Write that line to new file. */
	fseek(read, location, SEEK_SET);											/* Go back to innitial file stream location. */
	current->used = 1;
}


/*
Free memory taken by macros linked list and allerts for unused macros,
used after second pass or in case of error found.
*/
void freeMacros(Macro **macroRoot)
{
	Macro *current = *macroRoot;
	while(current != NULL)														/* Run over all macros in list. */
	{
		Macro *delete = current;												/* Save current macro to delete. */
		if(!current->used)
			printf("MACRO NOTICE: \"%s\" was not used\n", current->name);		
		current = current->nextMacro;											/* Go to next macro in list. */
		free(delete);															/* Free previous macro memory location. */
	}
	*macroRoot = NULL;
}


/*
USED FOR DEBUGING.
Print all macros names in linked list to the terminal.
*/
void printMacros(Macro **macroRoot)
{
	Macro *current = *macroRoot;
	printf("LIST OF MACROS:\n");
	while(current != NULL)
	{	
		printf("%s\n", current->name);
		current = current->nextMacro;
	}
}
