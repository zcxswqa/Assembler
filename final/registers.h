/* Yam Tiram 312127814 */

#define NUMBER_OF_REGISTERS 8

typedef struct Registers														
{
	char name[4];
	int number;
} Registers;


/*
Gets a char pointer to a word.
Returns the corresponding register number if found and -1 otherwise.
*/
int getRegisterNumber(char *word);
