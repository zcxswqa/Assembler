/* Yam Tiram 312127814 */

#define MACRO_NAME_SIZE 80

typedef struct Macro															/* Macros linked list. */
{
	char name[MACRO_NAME_SIZE];
	int start;
	int end;
	int used;
	struct Macro *nextMacro;
} Macro;


/*
Gets a pointer to macros linked list root pointer, a char pointer to macro name,
macro beginning address, macro end address, char pointer to as file name and
the line number in which the macro declaration was made.
If macro name was already declared, prints an error message and return.
Insert macro name, beginning and ending addresses to the macros linked list.
*/
void insertMacro(Macro** root, char *macroName, int startOfMacro, int endOfMacro, char *asFileName, int lineNumber);


/*
Gets a char pointer to the current word read in file and a pointer to macros 
linked list root pointer.
Check if that word is a macro name.
Return 1 if it is and 0 otherwise.
*/
Macro *isMacro(char *word, Macro **macroRoot);


/*
Gets a file pointer to the original file, a file pointer to the new file and a
pointer to the macro to write.
Copy content of macro from original file to the new file.
*/
void writeMacro(FILE *read, FILE *write, Macro *current);


/*
Gets a pointer to macros linked list root pointer.
Free memory of macros linked list and allerts for unused macros.
*/
void freeMacros(Macro **root);


/*
USED FOR DEBUGING.
Gets a pointer to macros linked list root pointer.
Print macros names to terminal.
*/
void printMacros(Macro **macroRoot);
