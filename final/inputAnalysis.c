/* Yam Tiram 312127814 */

#include "header.h"
#include "macros.h"
#include "lables.h"
#include "inputAnalysis.h"
#include "commands.h"
#include "registers.h"
#include "stringModifications.h"


/*
Checks if line size is greater than max size.
If it is, prints an error message and returns 1, otherwise returns 0.
*/
int tooLong(char *line, char *filename, int lineNumber)
{
	if(strlen(line) > MAX_LINE_SIZE)
	{
		printf("%s:#%d:	LINE ERROR: line is longer than 80 characters.\n", filename, lineNumber);
		return 1;
	}
	
	return 0;
}


/*
Checks what is a given word.
Returns it's most probable type.
*/
enum what whatAreYou(char *word)
{
	if(word[0] == ';')
		return comment;
	else if(word[strlen(word)-1] == ':')
		return lable;
	else if(getCommandNumber(word) != -1)										/* Check if a word is a command name. */
		return command;
	else if(!strcmp(word, "data") || !strcmp(word, "string") || !strcmp(word, "extern") || !strcmp(word, "intern") || word[0] == '.')
		return directive;
	else if(word[0] == '#')
		return variable;
	else if(getRegisterNumber(word) != -1)											/* Check if a word is a register name. */
		return reg;
	else
		return lable;
}


/*
Checks if operands are of valid syntax.
Returns 0 and prints an error message if invalid and returns 1 otherwise.
*/
int validCommandOperands(char *line, char *command, Macro **macroRoot, LableTable **lableRoot, char *filename, int lineNumber)
{
	char *OP;
	int commandNumber;
	char *commandName;
	int numberOfOperands;
	int destAddressing;
	int i = 0;
	int jumping = 0;															/* Flag for if command is of jump type. */
	char lineTokenized[MAX_LINE_SIZE] = {0};										/* Copy of line to use in strtok function. */
	
	commandNumber = getCommandNumber(command);
	commandName = getCommandName(commandNumber);
	numberOfOperands = howManyOperands(commandNumber);
	destAddressing = commandDA(commandNumber);
	jumping = (destAddressing & 4);
	
	if(line[0] == ',')
	{
		if(numberOfOperands == 0)
		{
			printf("%s:#%d: OPERAND ERROR: \"%s\" is a zero operands command, extraneous text.\n", filename, lineNumber, commandName);
			return 0;
		}
		else
		{
			printf("%s:#%d: OPERAND ERROR: a comma before first operand.\n", filename, lineNumber);
			return 0;
		}
	}
	else if(line[strlen(line)-1] == ',')
	{
		printf("%s:#%d: OPERAND ERROR: a comma after the last operand.\n", filename, lineNumber);
		return 0;
	}
	
	strcpy(lineTokenized, line);
	OP = strtok(lineTokenized, ",");
	while(OP != NULL)
	{
		removeWhitespaces(OP);													/* Remove white-spaces from beginning and end of operand. */
		if(OP[0] == '\0')
		{
			printf("%s:#%d:	OPERAND ERROR: consecutive commas.\n", filename, lineNumber);
			return 0;
		}
		OP = strtok(NULL, ",");
	}
	
	/* Check for legal number of operands and compatiblity of operand addresing
		depending on the command. */
	strcpy(lineTokenized, line);
	switch(numberOfOperands)
	{
		case(0):
			if((OP = strtok(lineTokenized, " ,\t")) != NULL)
			{
				printf("%s:#%d: OPERAND ERROR: \"%s\" is a zero operands command, exccess operands were given\n", filename, lineNumber, commandName);
				return 0;
			}
			break;
		case(1):
			if((OP = strtok(lineTokenized, " \t")) == NULL)
			{
				printf("%s:#%d: OPERAND ERROR: \"%s\" is a one operand command, but none were given.\n", filename, lineNumber, commandName);
				return 0;
			}
			if(!jumping)
			{
				while(OP[i] != '\0')
				{
					if(OP[i] == ',')
					{
						printf("%s:#%d: OPERAND ERROR: \"%s\" is a one operand command, exccess operands were given.\n", filename, lineNumber, commandName);
						return 0;					
					}
					i++;
				}
			}
			if(strtok(NULL, WHITESPACES) != NULL)
			{
				printf("%s:#%d: OPERAND ERROR: \"%s\" is a one operand command, exccess operands were given.\n", filename, lineNumber, commandName);
				return 0;
			}
			if(!validOperand(OP, macroRoot, lableRoot, filename, lineNumber))	/* Check if operand is of valid syntax. */
				return 0;
			else if(!addressingCheck(OP, commandNumber, dest))					/* Check if operand addresing is compatible with command. */
			{
				printf("%s:#%d: OPERAND ERROR: desination operand addresing is not compatible with \"%s\" command.\n", filename, lineNumber, commandName);
				return 0;
			}
			else if((OP = strtok(NULL, " \t")) != NULL)
			{
				printf("%s:#%d: OPERAND ERROR: \"%s\" is a one operand command, exccess operands were given.\n", filename, lineNumber, commandName);
				return 0;
			}
			break;
		case(2):
			if((OP = strtok(lineTokenized, " ,\t")) == NULL)
			{
				printf("%s:#%d: OPERAND ERROR: \"%s\" is a two operands command, but none were given.\n", filename, lineNumber, commandName);
				return 0;
			}
			else if(!validOperand(OP, macroRoot, lableRoot, filename, lineNumber))/* Check if operand is of valid syntax. */
				return 0;			
			else if(!addressingCheck(OP, commandNumber, source))				/* Check if operand addresing is compatible with command. */
			{
				printf("%s:#%d: OPERAND ERROR: source operand addresing is not compatible with \"%s\" command.\n", filename, lineNumber, commandName);
				return 0;
			}
			
			else if((OP = strtok(NULL, " ,\t")) == NULL)
			{
				printf("%s:#%d: OPERAND ERROR: \"%s\" is a two operands command but only one was given.\n", filename, lineNumber, commandName);
				return 0;
			}
			else if(!validOperand(OP, macroRoot, lableRoot, filename, lineNumber))/* Check if operand is of valid syntax. */
				return 0;			
			else if(!addressingCheck(OP, commandNumber, dest))					/* Check if operand addresing is compatible with command. */
			{
				printf("%s:#%d: OPERAND ERROR: desination operand addresing is not compatible with \"%s\" command.\n", filename, lineNumber, commandName);
				return 0;	
			}
			
			else if((OP = strtok(NULL, " \t")) != NULL)
			{
				printf("%s:#%d: OPERAND ERROR: \"%s\" is a two operands command, exccess operands were given.\n", filename, lineNumber, commandName);
				return 0;
			}
			break;
	}
	return 1;
}


/*
Counts number of memory words an instruction operands will need and adds 1 for
instruction word.
Returns that number. 
*/
int InstructionSize(char *line, char *command)
{
	char lineTokenized[MAX_LINE_SIZE];											/* Copy of line to use in strtok function. */
	char *token;
	int IC = 0;
	int wasRegister = 0;														/* Flag fow when first operand is a register. */
	
	strcpy(lineTokenized, line);
	token = strtok(lineTokenized, " ,\t()");
	while(token != NULL)
	{
		switch(Addressing(token))												/* Get operand addresing. */
		{
			case(immidiate):
				IC += 1;
				break;
			case(direct):
				IC += 1;
				break;
			case(directRegister):
				if(!wasRegister)												/* If first operand is a register, IC should not be increased. */
				{
					IC += 1;
					wasRegister = 1;
				}
			case(jump):
				break;
		}
		token = strtok(NULL, " ,\t()");
	}
	return IC+1;	
}


/*
Checks what kind of directive is a given directive.
Returns that directive type if found.
Prints an error message and returns "invalid" type otherwise.
*/
enum directiveType whatDirective(char *word, char *filename, int lineNumber)
{
	if(word[0] != '.')
	{
		printf("%s:#%d: DERECTIVE ERROR: \"%s\" didnt start with \".\".\n", filename, lineNumber, word);
		return invalid;
	}
	else if(!strcmp(word, ".data"))
		return data;
	else if(!strcmp(word, ".string"))
		return string;
	else if(!strcmp(word, ".extern"))
		return ext;
	else if(!strcmp(word, ".entry"))
		return entry;
	else
	{
		printf("%s:#%d:	DIRECTIVE ERROR: \"%s\"  is not a valid directive name.\n", filename, lineNumber, word);
		return invalid;
	}
}


/*
Checks if data directive input is of valid syntax.
Prints an error message and returns 0 if invalid, returns 1 otherwise.
*/
int validData(char *data, char *filename, int lineNumber)
{
	char dataTokenized[LINE_SIZE];												/* Copy of data input to use in strtok function. */
	char noWhiteSpaces[LINE_SIZE];
	char *token;	
	int i = 0;
	int asInt;
	
	if(strlen(data) == 0)
	{
		printf("%s:#%d: DATA ERROR: No data was given.\n", filename, lineNumber);
		return 0;
	}
	else if(data[0] == ',')
	{
		printf("%s:#%d:	DATA ERROR: a comma before first number.\n", filename, lineNumber);
		return 0;		
	}
	else if(data[strlen(data)-1] == ',')
	{
		printf("%s:#%d:	DATA ERROR: a comma after last number.\n", filename, lineNumber);
		return 0;
	}

	strcpy(dataTokenized, data);	
	token = strtok(dataTokenized, ",");
	while(token != NULL)
	{
		strcpy(noWhiteSpaces, token);
		removeWhitespaces(token);												/* Remove white-spaces from beginning and end of token. */
		i = 0;
		if(token[i] == '\0')
		{
			printf("%s:#%d:	DATA ERROR: consecutive commas.\n", filename, lineNumber);
			return 0;
		}
		else if(!isdigit(token[i]) && token[i] != '-' && token[i] != '+')
		{
			printf("%s:#%d:	DATA ERROR: \"%s\" is not a valid integer.\n", filename, lineNumber, token);
			return 0;
		}
		i++;
		while(!isspace(token[i]) && token[i] != '\0')
		{
			if(!isdigit(token[i]))
			{
				printf("%s:#%d:	DATA ERROR: \"%s\" is not a valid integer.\n", filename, lineNumber, token);
				return 0;
			}
			i++;
		}
		if(token[i] != '\0')													/* Meaning there was a white-space. */
		{
			printf("%s:#%d:	DATA ERROR: Missing comma.\n", filename, lineNumber);
			return 0;
		}
		asInt = atoi(token);
		if(asInt > LARGEST_NUMBER)
		{
			printf("%s:#%d:	DATA ERROR: \"%s\" is greater than %d.\n", filename, lineNumber, token, LARGEST_NUMBER);
			return 0;
		}
		if(asInt < SMALLEST_NUMBER)
		{
			printf("%s:#%d:	DATA ERROR: \"%s\" is smaller than %d.\n", filename, lineNumber, token, SMALLEST_NUMBER);
			return 0;
		}
		token = strtok(NULL, ",");
	}
	return 1;
}


/*
Counts number of memory words a data directive input will need.
Returns that number.
*/
int dataSize(char *line)
{
	char *token;
	char lineTokenized[MAX_LINE_SIZE];											/* Copy of line to use in strtok function. */
	int i = 0;
	
	strcpy(lineTokenized, line);
	token = strtok(lineTokenized, ",");
	while(token != NULL)
	{
		i++;
		token = strtok(NULL, ",");
	}
	return i;
}


/*
Checks if string directive input is of valid syntax.
Prints an error message and return 0 if invalid.
Returns 1 otherwise.
*/
int validString(char *line, char *filename, int lineNumber)
{
	char string[LINE_SIZE];

	if(strlen(line) == 0)
	{
		printf("%s:#%d: STRING ERROR: No string was given.\n", filename, lineNumber);
		return 0;
	}

	strcpy(string, line);
	removeWhitespaces(string);													/* Remove white-spaces from beginning and end of string. */
	if(string[0] != '"')
	{
		printf("%s:#%d: STRING ERROR: No quatation mark in the begining of: \"%s\".\n", filename, lineNumber, line);
		return 0;
	}
	if(string[strlen(string)-1] != '"')
	{
		printf("%s:#%d: STRING ERROR: No quatation mark at the end of: \"%s\".\n", filename, lineNumber, line);
		return 0;
	}
	return 1;
}


/*
Checks if extern directive input is of valid syntax.
Prints an error message and return 0 if invalid.
Returns 1 otherwise.
*/
int validExternFirstPass(char *line, Macro **macroRoot, LableTable **lableRoot, char *filename, int lineNumber)
{
	char lineTokenized[MAX_LINE_SIZE];											/* Copy of line to use in strtok function. */
	char *token;
	
	token = strcpy(lineTokenized, line);
	if((token = strtok(lineTokenized, " \t")) == NULL)
	{
		printf("%s:#%d: EXTERN ERROR: No lable was give as extern.\n", filename, lineNumber);
		return 0;
	}
	else if(!validUniversalLable(token, macroRoot, lableRoot, EXTERN, filename,
		lineNumber))															/* Check if lable is of valid syntax. */
		return 0;
	else if((token = strtok(NULL, " \t")) != NULL)
	{
		printf("%s:#%d: EXTERN ERROR: Exccess information after extern lable.\n", filename, lineNumber);			
		return 0;
	}
	return 1;
}


/*
Checks if entry directive input is of valid syntax.
Prints an error message and return 0 if invalid.
Returns 1 otherwise.
*/
int validEntryFirstPass(char *line, Macro **macroRoot, LableTable **lableRoot, char *filename, int lineNumber)
{
	char lineTokenized[MAX_LINE_SIZE];											/* Copy of line to use in strtok function. */
	char *token;
	
	strcpy(lineTokenized, line);
	if((token = strtok(lineTokenized, " \t")) == NULL)
	{
		printf("%s:#%d: ENTRY ERROR: No lable was give as entry.\n", filename, lineNumber);
		return 0;
	}
	else if(!validUniversalLable(token, macroRoot, lableRoot, ENTRY, filename,
		lineNumber))															/* Check if lable is of valid syntax. */
		return 0;
	else if((token = strtok(NULL, " \t")) != NULL)
	{
		printf("%s:#%d: ENTRY ERROR: Exccess information after entry lable.\n", filename, lineNumber);
		return 0;
	}
	return 1;
}


/*
Checks if operand is of valid syntax.
Prints an error message and return 0 if invalid.
Returns 1 otherwise.
*/
int validOperand(char *OP, Macro **macroRoot, LableTable **lableRoot, char *filename, int lineNumber)
{
	int i = 0;
	int parentheses = 0;
	char OPTokenized[MAX_LINE_SIZE];
	char *token;
	
	switch(Addressing(OP))														/* Get operand addresing. */
	{
		case(immidiate):
			if(*OP != '#')
			{
				printf("%s:#%d: OPERAND ERROR: \"%s\" is missing a hashtag.\n", filename, lineNumber, OP);
				return 0;
			}
			else if(!isdigit(*++OP) && *OP != '+' && *OP !='-')
			{
				printf("%s:#%d: OPERAND ERROR: \"%s\" is not a valid immidiate operand.\n", filename, lineNumber, OP);
				return 0;
			}
			while(isdigit(*++OP))
				;
				if(*OP != '\0')
				{
					printf("%s:#%d: OPERAND ERROR: \"%s\" is not a valid immidiate operand.\n", filename, lineNumber, OP);
					return 0;
				}
			break;
		case(direct):
			if(!validUniversalLable(OP, macroRoot, lableRoot, OPERAND, filename,
				lineNumber))													/* Check if lable is of valid syntax. */
				return 0;
			break;
		
		case(jump):
			if(OP[i] == '(')
			{
				printf("%s:#%d: OPERAND ERROR: \"%s\" missing a lable to jump to.\n", filename, lineNumber, OP);
				return 0;
			}
			while(OP[i] != '\0')
			{
				if(OP[i] == '(')
				{
					parentheses = 1;
					while(OP[i] != ')' && OP[i] != '\0')
						i++;
					if(OP[i] == '\0')
					{
						printf("%s:#%d: OPERAND ERROR: Parentheses around operands to jump with are not closed.\n", filename, lineNumber);
						return 0;
					}
					else if(OP[++i] != '\0')
					{
						printf("%s:#%d: OPERAND ERROR: \"%s\" ended ilegally.\n", filename, lineNumber, OP);
						return 0;
					}
				}
				else if(OP[i] == ')')
				{
					printf("%s:#%d: OPERAND ERROR: Parentheses around operands to jump with were not opened.\n", filename, lineNumber);
					return 0;
				}
				i++;
			}
			if(!parentheses)
			{
				printf("%s:#%d: OPERAND ERROR: Operands to jump with are not enclosed with parentheses.\n", filename, lineNumber);
				return 0;
			}

			strcpy(OPTokenized, OP);
			token = strtok(OPTokenized, "(");
			if(!validUniversalLable(token, macroRoot, lableRoot, OPERAND,
				filename, lineNumber))											/* Check if lable is of valid syntax. */
				return 0;
			if((token = strtok(NULL, ",)")) == NULL)
			{
				printf("%s:#%d: OPERAND ERROR: \"%s\" should contain two operands to jump with, but none were given.\n", filename, lineNumber, OP);
				return 0;
			}
			if(!validOperand(token, macroRoot, lableRoot, filename, lineNumber))/* Check if operand is of valid syntax. */
				return 0;
			if((token = strtok(NULL, ",)")) == NULL)
			{
				printf("%s:#%d: OPERAND ERROR: \"%s\" should contain two operands to jump with, but only one was given.\n", filename, lineNumber, OP);
				return 0;
			}
			if(!validOperand(token, macroRoot, lableRoot, filename, lineNumber))/* Check if operand is of valid syntax. */
				return 0;
			if((token = strtok(NULL, ",)")) != NULL)
			{
				printf("%s:#%d: OPERAND ERROR: \"%s\" should contain two operands to jump with but more were given.\n", filename, lineNumber, OP);
				return 0;
			}
			break;
			
		case(directRegister):
			break;
	}
	return 1;
}
