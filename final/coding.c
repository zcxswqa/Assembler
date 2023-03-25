/* Yam Tiram 312127814 */

#include "header.h"
#include "macros.h"
#include "lables.h"
#include "commands.h"
#include "registers.h"
#include "stringModifications.h"
#include "coding.h"


/*
Writes addresses to the object file.
Advances address by one.
*/
void writeAddressToFile(FILE *objFile, int *address)
{
	fprintf(objFile, "\n0%d\t\t", *address);
	*address+=1;
}


/*
Write a given amount of zeroes to the file.
*/
void writeZerosToFile(FILE *objFile, int amount)
{
	writeBitsToFile(objFile, 0, amount);
}


/*
Write a command opcode to the file.
*/
void writeOPCodeToFile(FILE *objFile, int commandNumber)
{
	writeBitsToFile(objFile, commandNumber, OPCODE_ADDRESSING_SIZE);
}


/*
Write operand addressing to the file.
*/
void writeOperandAddressingToFile(FILE *objFile, char *operand)
{
	switch(Addressing(operand))
	{
		case(immidiate):
			writeBitsToFile(objFile, 0, OPERAND_ADDRESSING_SIZE);
			break;
		case(direct):
			writeBitsToFile(objFile, 1, OPERAND_ADDRESSING_SIZE);
			break;
		case(jump):
			writeBitsToFile(objFile, 2, OPERAND_ADDRESSING_SIZE);
			break;
		case(directRegister):
			writeBitsToFile(objFile, 3, OPERAND_ADDRESSING_SIZE);
			break;
	}
}


/*
Writes an unsigend integer binary representation to the file.
*/
void writeBitsToFile(FILE *objFile, unsigned num, int representation)
{
	int check = 1;
	int i;
	if(num<0)
		num = ~(num)+1;															/* Get the 2's complement representation of the negative integer. */
	check <<= (representation-1);												/* Left shift the number "1" by 12 bits amout. */
	for(i=0; i<representation; i++)
	{
		if((num<<i)&check)
			fprintf(objFile, "%c", '/');
		else
			fprintf(objFile, "%c", '.');
	}
}


/*
Directs an operand to the right coding function based on his addressing type.
*/
void codeOneOperand(FILE *objFile, int *IC, char *OP, enum sourceOrDest SD, LableTable **lableRoot, char *externFileName)
{
	switch(Addressing(OP))
	{
		case(immidiate):
			codeImmidiate(objFile, IC, OP);
			break;
		case(direct):
			codeLable(objFile, IC, OP, lableRoot, externFileName);
			break;
		case(jump):
			break;
		case(directRegister):
			codeOneRegister(objFile, IC, OP, SD);
	}
}


/*
writes lable's address to the object file, if it's a ".extern"-declared lable,
writes its name and it's usage address.
*/
void codeLable(FILE *objFile, int *IC, char *operand, LableTable **lableRoot, char *externFileName)
{
	LableTable *currentLable = *lableRoot;
	FILE *extFile;
	int operandIsExtern = 0;
	writeAddressToFile(objFile, IC);
	while(currentLable != NULL)
	{
		if(!strcmp(operand, currentLable->name) && currentLable->type == EXTERN)
		{
			operandIsExtern = 1;
			currentLable->used++;
			extFile = fopen(externFileName, "a"); 
			if(extFile == NULL)
			{
				printf("FILE OPENING ERROR: \"%s\" was not opend\n", externFileName);
				exit(1);
			}
			fprintf(extFile, "%s	%d\n", operand, *IC-1);
			fclose(extFile);
		}
		if(!strcmp(operand, currentLable->name) && currentLable->type != ENTRY)
			writeBitsToFile(objFile, currentLable->address, WORD_SIZE-AER_SIZE-1);	/* Write lable address to the object file. */
		currentLable = currentLable->nextLable;
	}
	if(operandIsExtern)
		writeBitsToFile(objFile, E, AER_SIZE);
	else
		writeBitsToFile(objFile, R, AER_SIZE);
}


/*
Code and write immidiate variable to the object file.
*/
void codeImmidiate(FILE *objFile, int *IC, char *operand)
{
	writeAddressToFile(objFile, IC);
	removeFirstLetter(operand);
	writeBitsToFile(objFile, atoi(operand), WORD_SIZE - AER_SIZE - 1);
	writeBitsToFile(objFile, A, AER_SIZE);
}


/*
Code and write register number to th object file.
*/
void codeOneRegister(FILE *objFile, int *IC, char *OP, enum sourceOrDest SD)
{
	writeAddressToFile(objFile, IC);
	if(SD == source)
	{
		writeBitsToFile(objFile, getRegisterNumber(OP), REGISTER_ADDRESSING_SIZE);
		writeBitsToFile(objFile, 0, REGISTER_ADDRESSING_SIZE);
	}
	else if(SD == dest)
	{
		writeBitsToFile(objFile, 0, REGISTER_ADDRESSING_SIZE);
		writeBitsToFile(objFile, getRegisterNumber(OP), REGISTER_ADDRESSING_SIZE);
	}
	writeBitsToFile(objFile, A, AER_SIZE);
}


/*
Code and write two registers numbers to the object file as one word.
*/
void codeTwoRegisters(FILE *f, int *address, char *Source, char *Dest)
{
	writeAddressToFile(f, address);
	writeBitsToFile(f, getRegisterNumber(Source), REGISTER_ADDRESSING_SIZE);
	writeBitsToFile(f, getRegisterNumber(Dest), REGISTER_ADDRESSING_SIZE);
	writeBitsToFile(f, 0, AER_SIZE);
}
