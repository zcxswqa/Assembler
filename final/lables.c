/* Yam Tiram 312127814 */

#include "header.h"
#include "macros.h"
#include "lables.h"
#include "commands.h"
#include "registers.h"


/*
Check if a lable is of valid syntax and is not a macro name or a saved word.
Prints an error message and returns 0 if invalid.
Returns 1 otherwise.
*/
int validUniversalLable(char *Lable, Macro **macroRoot, LableTable **lableRoot, enum lableType type, char *filename, int lineNumber)
{
	int length = strlen(Lable);
	char noColon[MAX_LABLE_SIZE];												/* Copy of lable to remove colon from. */
	char *PLable = Lable;														/* Pointer to lable. */
	int i = 0;

	if(length >= MAX_LABLE_SIZE)
	{
		printError(Lable, type, tooLongError, filename, lineNumber);			/* Prints "lable is too long" error message. */
		return 0;
	}
	
	strcpy(noColon, Lable);
	if(Lable[strlen(Lable)-1] == ':')
		noColon[strlen(noColon)-1] = '\0';										/* Removes colon from end of lable. */	
	
	if(type == INSTRUCTION)
	{
		while(*PLable != ':')
		{
			if(*PLable++ == '\0')
			{
				printError(noColon, type, noColonError, filename, lineNumber);	/* Prints "no colon" error message. */
				return 0;		
			}
		}
		if(*++PLable != '\0')
		{
			printError(noColon, type, noSpaceError, filename, lineNumber);		/* Prints "no space after colon" error message. */
			return 0;
		}
	}
	
	if(type != OPERAND)
		if(collision(noColon, lableRoot, type))									/* Checks if foe lable collision with lable table content. */
		{
			printError(noColon, type, alreadyExistingError, filename,
				lineNumber);													/* Prints "lable already exist" error message. */
			return 0;		
		}
	
	if(!isalpha(noColon[0]))
	{
		printError(noColon, type, nonAlphabeticError, filename, lineNumber);	/* Prints "lable started with a non alphabetical character" error message. */
		return 0;
	}
	else if(getCommandNumber(noColon) != -1)									/* Check if lable is a command name. */
	{
		printError(noColon, type, commandNameError, filename, lineNumber);		/* Prints "lable is a command name" error message. */
		return 0;
	}
	else if(!strcmp(noColon, "data") || !strcmp(noColon, "string") || !strcmp(noColon, "extern") || !strcmp(noColon, "intern"))
	{
		printError(noColon, type, directiveNameError, filename, lineNumber);	/* Prints "lable is a directive name" error message. */
		return 0;
	}
	else if(getRegisterNumber(noColon) != -1)									/* Check if lable is a register name. */
	{
		printError(noColon, type, registerNameError, filename, lineNumber);		/* Prints "lable is a register name" error message. */
		return 0;
	}
	
	while(i < length-1 && noColon[i] != '\0' && noColon[i] != '\n')
	{
		if(!isalpha(noColon[i]) && !isdigit(noColon[i]))
		{
			printError(noColon, type, ilegalCharError, filename, lineNumber);	/* Prints "lable contains an ilegal character" error message. */
			return 0;
		}
		i++;
	}
			
	if(isMacro(noColon, macroRoot))												/* Check if lable is a macro name. */
	{
		printError(noColon, type, macroNameError, filename, lineNumber);		/* Prints "lable is a macro name" error message. */
		return 0;
	}
	return 1;
}


/*
Prints the corresponding error message.
*/
void printError(char *Lable, enum lableType type, enum errorType error, char *filename, int lineNumber)
{
	char Type[MAX_LABLE_SIZE];
	switch(type)
	{
		case(INSTRUCTION):
			strcpy(Type, "LABLE");
			break;
		case(OPERAND):
			strcpy(Type, "OPERAND");
			break;
		
		case(DATA):
			strcpy(Type, "DATA");
			break;
			
		case(ENTRY):
			strcpy(Type, "ENTRY");
			break;
			
		case(EXTERN):
			strcpy(Type, "EXTERN");
			break;
	}
	
	switch(error)
	{
		case(tooLongError):
			printf("%s:#%d:	%s ERROR: \"%s\" lable is longer than %d characters.\n", filename, lineNumber, Type, Lable, MAX_LABLE_SIZE-1);
			break;
		
		case(noSpaceError):
			printf("%s:#%d: %s ERROR: a missing white-space after colon in:\"%s\".\n", filename, lineNumber, Type, Lable);
			break; 
			
		case(noColonError):
			printf("%s:#%d: %s ERROR: \"%s\" did not ended with a colon linked to it.\n", filename, lineNumber, Type, Lable);
			break;
			
		case(alreadyExistingError):
			printf("%s:#%d: %s ERROR: \"%s\" was alredy declared.\n", filename, lineNumber, Type, Lable);
			break;
			
		case(ilegalCharError):
			printf("%s:#%d: %s ERROR: \"%s\" lable contains ilegal characters.\n", filename, lineNumber, Type, Lable);		
			break;
			
		case(nonAlphabeticError):
			printf("%s:#%d: %s ERROR: \"%s\" lable starts with a non Alphbetic character.\n", filename, lineNumber, Type, Lable);
			break;
		
		case(commandNameError):
			printf("%s:#%d: %s ERROR: \"%s\" lable is a command name.\n", filename, lineNumber, Type, Lable);
			break;
			
		case(directiveNameError):
			printf("%s:#%d: %s ERROR: \"%s\" lable is a directive name.\n", filename, lineNumber, Type, Lable);
			break;
			
		case(registerNameError):
			printf("%s:#%d: %s ERROR: \"%s\" lable is a register name.\n", filename, lineNumber, Type, Lable);
			break;
			
		case(macroNameError):
			printf("%s:#%d: %s ERROR: \"%s\" lable is a macro name.\n", filename, lineNumber, Type, Lable);
			break;
	}
	return;
}


/*
Insert a lable with it's address and type to the lables table.
*/
void insertLable(char *lableName, int address, enum lableType type, LableTable **lableRoot, char *filename, int lineNumber)
{
	LableTable *currentLable = *lableRoot;
	LableTable *newLable = malloc(sizeof(LableTable));
	if(newLable == NULL)
	{
		printf("%s:#%d:	LABLE CREATION ERROR: LABLE \"%s\" was not created.\n", filename, lineNumber, lableName);
		return;
	}
	
	newLable->nextLable = NULL;
	strcpy(newLable->name, lableName);
	newLable->address = address;
	newLable->type = type;
	newLable->used = 0;
	
	if(*lableRoot == NULL)														/* If this is the first lable in lables table. */
	{
		*lableRoot = newLable;
		return;
	}
	do																			/* Go to last lable in table. */
	{
		if(currentLable->nextLable != NULL)
			currentLable = currentLable->nextLable;	
	}while(currentLable->nextLable != NULL);
		
	currentLable->nextLable = newLable;
}	


/*
Checks if a lable was already declared, and if so, check if it's a contradicting
declarations.
Returns 1 if collision is found and 0 otherwise.
*/
int collision(char *lableName, LableTable **lableRoot, enum lableType type)
{
	LableTable *currentLable = *lableRoot;
	while(currentLable != NULL)
	{
		if(!strcmp(lableName, currentLable->name))
		{
			switch(type)
			{
				case(INSTRUCTION):
					if(currentLable->type != ENTRY)
						return 1;
					break;
				case(DATA):
					if(currentLable->type != ENTRY)
						return 1;
					break;
				case(ENTRY):
					if(currentLable->type == EXTERN)
						return 1;
					break;
				case(EXTERN):
					if(currentLable->type != EXTERN)
						return 1;
					break;
				case(OPERAND):
					return 1;
					break;
			}
		}
		currentLable = currentLable->nextLable;
	}
	return 0;
}


/*
Adds the final instruction count to all data type lables addresses.
*/
void updateLables(LableTable **lableRoot, int IC)
{
	LableTable *currentLable = *lableRoot;
	while(currentLable != NULL)
	{
		if(currentLable->type == DATA)
			currentLable->address += IC;
		currentLable = currentLable->nextLable;
	}
	return;
}


/*
Free the memory of lable table linked list.
*/
void freeLables(LableTable **lableRoot)
{
	LableTable *currentLable = *lableRoot;
	while(currentLable != NULL)
	{
		LableTable *delete = currentLable;
		currentLable = currentLable->nextLable;
		free(delete);
	}
	*lableRoot = NULL;
}


/*
Prints a notice message if an extern lable was not used in file.
*/
void checkExternsUses(LableTable **lableRoot, char *filename)
{
	LableTable *currentLable = *lableRoot;
	while(currentLable != NULL)
	{
		if(currentLable->type == EXTERN && !currentLable->used)
			printf("%s: LABLE NOTICE: extern lable \"%s\" was not used.\n", filename, currentLable->name);
		currentLable = currentLable->nextLable;
	}	
}


/*
USED FOR DEBUGING.
Prints each lable with it's type, name and address.
*/
void printLables(LableTable **lableRoot)
{
	LableTable *currentLable = *lableRoot;
	while(currentLable != NULL)
	{
		switch(currentLable->type)
		{
			case(INSTRUCTION):
				printf("INSTRUCTION: \"%s\" address: %d\n", currentLable->name, currentLable->address);
				break;
			
			case(OPERAND):
				break;
				
			case(DATA):
				printf("DATA: \"%s\" address: %d\n", currentLable->name, currentLable->address);
				break;
				
			case(ENTRY):
				printf("ENTRY: \"%s\" address: %d\n", currentLable->name, currentLable->address);
				break;
				
			case(EXTERN):
				printf("EXTERN: \"%s\" address: %d, used:%d\n", currentLable->name, currentLable->address, currentLable->used);
				break;
		}
		currentLable = currentLable->nextLable;
	}	
}
