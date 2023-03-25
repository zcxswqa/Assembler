/* Yam Tiram 312127814 */

#include "header.h"
#include "commands.h"
#include "registers.h"


/*
Initialization of commands array in which each command has its name,
compatible operands amount, compatible source operand addresings and
compatible desination operand addresings.
*/
Commands commands[NUMBER_OF_COMMANDS] =
{{"mov", 2, immidiate | direct | directRegister, direct | directRegister},
{"cmp", 2, immidiate | direct | directRegister, immidiate | direct | directRegister},
{"add", 2, immidiate | direct | directRegister, direct | directRegister},
{"sub", 2, immidiate | direct | directRegister, direct | directRegister},
{"not", 1, 0, direct | directRegister},
{"clr", 1, 0, direct | directRegister},
{"lea", 2, direct, direct | directRegister},
{"inc", 1, 0, direct | directRegister},
{"dec", 1, 0, direct | directRegister},
{"jmp", 1, 0, direct | jump | directRegister},
{"bne", 1, 0, direct | jump | directRegister},
{"red", 1, 0, direct | directRegister},
{"prn", 1, 0,  immidiate | direct | directRegister},
{"jsr", 1, 0, direct | jump | directRegister},
{"rts", 0, 0, 0},
{"stop", 0, 0, 0}};


/*
Returns command number if found and -1 otherwise.
*/
int getCommandNumber(char *word)
{
	int i;
	for(i=0; i<NUMBER_OF_COMMANDS; i++)
	{
		if(!strcmp(word, commands[i].name))
			return i;
	}
	return -1;
}


/*
Returns command's name.
*/
char *getCommandName(int commandNumber)
{
	return commands[commandNumber].name;
}


/*
Returns command's expected operands amount.
*/
int howManyOperands(int commandNumber)
{
	return commands[commandNumber].operandsAmount;
}


/*
Returns command's expected source operand addresings.
*/
int commandSA(int commandNumber)
{
	return commands[commandNumber].sourceOP;
}


/*
Returns command's expected desination operand addresings
*/
int commandDA(int commandNumber)
{
	return commands[commandNumber].destOP;
}


/*
Returns 1 if command is a jump command and 0 otherwise.
*/
int isJump(int commandNumber)
{
	return (commandDA(commandNumber) & jump);
}


/*
Check what is the addresings of a given operand.
Returns it's most probable addressing.
*/
enum addressing Addressing(char *Operand)
{
	int isNumber = 1;
	int isJump = 0;
	char *operandPointer = Operand;
	while(*operandPointer != '\0')
	{
		if(!isdigit(*operandPointer++))
			isNumber = 0;
		if(*operandPointer == '(' || *operandPointer == ')' || *operandPointer == ',')
			isJump = 1;
	}
	if(Operand[0] == '#' || isNumber)
		return immidiate;
	else if(getRegisterNumber(Operand) != -1)									/* Check if operand is a register name. */
		return directRegister;
	else if(isJump)
		return jump;
	else
		return direct;
}


/*
Check if an operand addresings is corresponding to command addresings,
considering if it's a source or destination operand.
Return 0 if there is no correspondence and 1 otherwise.
*/
int addressingCheck(char *operand, int commandNumber, int sourceOrDest)
{
	switch(sourceOrDest)
	{
		case(source):
			if((Addressing(operand) & commandSA(commandNumber)) == 0)
				return 0;
			break;
		
		case(dest):
			if((Addressing(operand) & commandDA(commandNumber)) == 0)
				return 0;
			break;
	}
	return 1;
}


/*
USED FOR DEBUGING.
Print all commands details to terminal.
*/
void printCommand()
{
	int i;
	for(i=0; i<NUMBER_OF_COMMANDS; i++)
	{
		printf("\n%s:\n", commands[i].name);
		printf("operandsAmount: %d\n", commands[i].operandsAmount);
		printf("sourceOP addressing:\n");
		if((commands[i].sourceOP & immidiate) != 0)
			printf("immidiate\n");
		if((commands[i].sourceOP & direct) != 0)
			printf("direct\n");
		if((commands[i].sourceOP & jump) != 0)
			printf("jump\n");
		if((commands[i].sourceOP & directRegister) != 0)
			printf("directRegister\n");
		printf("destOP addressing:\n");
		if((commands[i].destOP & immidiate) != 0)
			printf("immidiate\n");
		if((commands[i].destOP & direct) != 0)
			printf("direct\n");
		if((commands[i].destOP & jump) != 0)
			printf("jump\n");
		if((commands[i].destOP & directRegister) != 0)
			printf("directRegister\n");
	}
}
