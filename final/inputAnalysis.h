/* Yam Tiram 312127814 */

#define LARGEST_NUMBER 2047
#define SMALLEST_NUMBER -2048


/*
Gets a char pointer to line, a char pointer to the file name and the current
line number.
Checks if line is longer than max line size if it is, prints an error message.
Returns 1 if line is too long and 0 otherwise.
*/
int tooLong(char *line, char *filename, int lineNumber);


/*
Gets a char pointer to a word.
Check if the word is a comment, lable, command, directive, variable or a register.
Returns the asnwer if found and treat as lable otherwise.
*/
enum what whatAreYou(char *word);


/*
Gets a char pointer to line, a char pointer to command name, a pointer to macros
linked list root pointer, a pointer to lable-table linked list root pointer,
a char pointer to the file name and the current line number.
Check if operands are of valid syntax and their addresings are compatible with
that command.
Returns 0 if invalid and 1 otherwise.
*/
int validCommandOperands(char *line, char *command, Macro **root, LableTable **lableRoot, char *filename, int lineNumber);


/*
Gets a char pointer to line and a char pointer to command name.
Count how many memory words an instruction will need.
Returns that number.
*/
int InstructionSize(char *line, char *command);


/*
Gets a char pointer to a word, a char pointer to the file name and the current
line number.
Check if the word is a .data, .string, .extern or .entry directive.
Returns directive type if found.
Prints an error message and reutnrs an "invalid" type otherwise.
*/
enum directiveType whatDirective(char *word, char *filename,int lineNumber);


/*
Gets a char pointer to the data part of line, a char pointer to the file name
and the current line number.
Checks if a .data directive inputs are valid.
Prints an error message and returns 0 if invalid.
Returns 1 if valid.
*/
int validData(char *data, char *filename, int lineNumber);


/*
Gets a char pointer to line.
Count how many memory words data inputs will need.
Returns that number.
*/
int dataSize(char *line);


/*
Gets a char pointer to line, char pointer to the file name and the current line
number.
Checks if a .string directive input is a valid string.
Prints an error message and returns 0 if invalid.
Returns 1 if valid.
*/
int validString(char *line, char *filename, int lineNumber);


/*
Gets a char pointer to line, a pointer to macros linked list root pointer,
a pointer to lable-table linked list root pointer, a char pointer to the file
name and the current line number.
Checks if a .extern directive input is a valid lable.
Prints an error message and returns 0 if invalid.
Returns 1 otherwise.
*/
int validExternFirstPass(char *line, Macro **root, LableTable **lableRoot, char *filename, int lineNumber);


/*
Gets a char pointer to line, a pointer to macros linked list root pointer,
a pointer to lable-table linked list root pointer, a char pointer to the file
name and the current line number.
Checks if a .entry directive input is a valid lable.
Prints an error message and returns 0 if invalid.
Returns 1 otherwise.
*/
int validEntryFirstPass(char *line, Macro **root, LableTable **lableRoot, char *filename, int lineNumber);


/*
Gets a char pointer to operand, a pointer to macros linked list root pointer,
a pointer to lable-table linked list root pointer, a char pointer to the file
name and the current line number.
Checks if operand is of valid syntax.
Prints an error message and returns 0 if invalid.
Returns 1 otherwise.
*/
int validOperand(char *OP, Macro **root, LableTable **lableRoot, char *filename, int lineNumber);
