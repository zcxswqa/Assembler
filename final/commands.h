/* Yam Tiram 312127814 */

#define NUMBER_OF_COMMANDS 16

enum addressing{immidiate = 1, direct = 2, jump = 4, directRegister = 8};		/* Used with bitwise operations to determain commands
																					and operands addressings. */
enum sourceOrDest{source = 1, dest};											/* Used in addressingCheck function. */

typedef struct Commands
{
	char name[MAX_COMMAND_NAME_SIZE];
	int operandsAmount;
	int sourceOP;
	int destOP;
} Commands;


/*
Gets a pointer to a word.
Returns the corresponding command number if found and -1 otherwise.
*/
int getCommandNumber(char *word);


/*
Gets a command number and returns a char pointer to its name.
*/
char *getCommandName(int commandNumber);


/*
Gets a command number and returns it's expected operands amount.
*/
int howManyOperands(int commandNumber);


/*
Gets a command number and returns it's expected source operand addressings.
*/
int commandSA(int commandNumber);


/*
Gets a command number and returns it's expected desination operand addressing.
*/
int commandDA(int commandNumber);


/*
Gets a command number and returns 1 if it's a jump command and 0 otherwise.
*/
int isJump(int commandNumber);


/*
Gets a char pointer to an operand.
Checks if it's addressings is immidiate, direct, jump or direct register.
Returns the answer if found and treat as "direct" otherwise.
*/
enum addressing Addressing(char *operand);


/*
Gets a char pointer to operand, command number and a flag for if operand is
a source operand or destination operand.
Check if the operand addressings is corresponding to the command addressings.
Return 0 if there is no correspondence and 1 otherwise.
*/
int addressingCheck(char *operand, int commandNumber, int sourceOrDest);


/*
USED FOR DEBUGING.
Print all commands details to terminal.
*/
void printCommand();
