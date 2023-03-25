/* Yam Tiram 312127814*/

#define MAX_LABLE_SIZE 31

enum lableType																	/* Used to distinguish between lables types. */
{
	INSTRUCTION,
	OPERAND,
	DATA,
	ENTRY,
	EXTERN
};

enum errorType																	/* Used in printError function. */
{
	tooLongError,
	noSpaceError,
	noColonError,
	alreadyExistingError,
	ilegalCharError,
	nonAlphabeticError,
	commandNameError,
	directiveNameError,
	registerNameError,
	macroNameError
};

typedef struct LableTable														/* Lable table linked list. */
{
	char name[MAX_LABLE_SIZE];
	int address;
	enum lableType type;
	int used;
	struct LableTable *nextLable;
} LableTable;


/*
Gets a char pointer to a word, a pointer to macros linked list root pointer,
a pointer to lable-table linked list root pointer, an enumeration of lableType,
char pointer to the file name and the current line number.
Checks if a lable is of valid syntax and is not a macro name or a saved word.
Prints an error message and returns 0 if invalid.
Returns 1 otherwise.
*/
int validUniversalLable(char *word, Macro **root, LableTable **lableRoot, enum lableType type, char *filename, int lineNumber);


/*
Gets a char pointer to lable name, an enumeration of the lable type, an
enumeration of the error type, a char pointer to the file name and the line
number.
Prints the corresponding error message.
*/
void printError(char *Lable, enum lableType type, enum errorType error, char *filename, int lineNumber);


/*
Gets a char pointer to lable name, the address of the lable, an enumeration of
lable type, a pointer to lable-table linked list root pointer, a char pointer
to the file name and the line number.
Insert the lable with it's address and type to the lable table linked list.
*/
void insertLable(char *lableName, int address, enum lableType type, LableTable **lableRoot, char *filename, int lineNumber);


/*
Gets a char pointer to lable name, a pointer to lable-table linked list root
pointer and an enumeration of lable type.
Checks if the lable was already declared and if so, checks if it's a
contradicting declarations.
Returns 1 if collision is found and 0 otherwise.
*/
int collision(char *lableName, LableTable **lableRoot, enum lableType type);


/*
Gets a pointer to lable-table linked list root pointer and the current
instruction count.
Adds the instruction count to all data type lables addresses.
*/
void updateLables(LableTable **lableRoot, int IC);


/*
Gets a pointer to lable-table linked list root pointer.
Free the memory of lable table linked list.
*/
void freeLables(LableTable **root);


/*
Gets a pointer to lable-table linked list root pointer and a char pointer to
the file name.
Allerts if an extern lable was not used.
*/
void checkExternsUses(LableTable **lableRoot, char *filename);


/*
USED FOR DEBUGING.
Gets a pointer to lable-table linked list root pointer.
Prints lables with their type, name and address
*/
void printLables(LableTable **lableRoot);
