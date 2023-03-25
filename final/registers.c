/* Yam Tiram 312127814 */

#include "header.h"
#include "registers.h"


/*
Initialization of registers array in which each register has its name and number.
*/
Registers registers[NUMBER_OF_REGISTERS] = 
{{"r0", 0},
{"r1", 1},
{"r2", 2},
{"r3", 3},
{"r4", 4},
{"r5", 5},
{"r6", 6},
{"r7", 7}};


/*
Returns register number if found and -1 otherwise.
*/
int getRegisterNumber(char *word)
{
	int i;
	for(i=0; i<NUMBER_OF_REGISTERS; i++)
		if(!strcmp(word, registers[i].name))
			return registers[i].number;
	return -1;
}
