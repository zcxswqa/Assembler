/* Yam Tiram 312127814 */


typedef struct DataWordsTable													/* Data words linked list, used to print all data words at the end of ".obj" file. */
{
	char word[WORD_SIZE];
	int address;
	struct DataWordsTable *nextWord;
} DataWordsTable;


/*
Gets a char pointer to the command operands, the command number, an int pointer
to the instruction counter, a pointer to lable-table linked list root pointer,
a FILE pointer to the object file, a char pointer to the ".ext" file name,
a char pointer to the ".am" file name and the current line number.
Direct the handling of command coding to other functions depending on operands
given, validates that any lable that is given as operand is declared.
Returns 0 if not and 1 otherwise.
*/
int handleCommand(char *justOpperands, int commandNumber, int *IC, LableTable **lableRoot, FILE *objFile, char *externFileName, char *amFileName, int lineNumber);


/*
Gets a char pointer to the operands, an int pointer to the data counter and a 
pointer to the data words table linked list root pointer.
Saves the data inputs to a data words linked list in order to write them all
together at the end of the object file.
*/
void handleData(char *justOperands, int *DC, DataWordsTable **DataWordRoot);


/*
Gets a char pointer to line, an int pointer to the data counter and a 
pointer to the data words table linked list root pointer.
Saves the string inputs to a data words linked list in order to write them all
together at the end of the object file.
*/
void handleString(char *line, int *DC, DataWordsTable **DataWordRoot);


/*
Gets a char pointer to a lable, a pointer to lable-table linked list root
pointer, a char pointer for the ".ent" file name, a char pointer to the ".am"
file name and the current line number.
Makes sure a ".entry"-declared lable was declared in the source code file and 
write the lable name and address of that declaration to the ".ent" file.
Retuen 1 if lable was declared properly.
Prints an error message and returns 0 otherwise.
*/
int handleEntry(char *lable, LableTable **lableRoot, char *entryFileName, char *amFileName, int lineNumber);


/*
Gets a char pointer to operand, a pointer to lable-table linked list root
pointer, a char pointer for the ".am" file name, and the current line number.
Makes sure a lable used as an operand was declared in the source code file.
Prints an error message and returns 0 if not.
Retuens 1 otherwise.
*/
int lableWasDeclared(char *operand, LableTable **lableRoot, char *amFileName, int lineNumber);


/*
Gets a pointer to the data words table linked list root pointer, an int pointer
to address and an unsigned int value.

*/
void insertDataWord(DataWordsTable **DataWordRoot, int *DC, unsigned value);


/*
Gets a FILE pointer to the object file, a pointer to the data words table
linked list root pointer and the current data count.
Write the data words from the data words table to the object file and free the
memory of data words table linked list.
*/
void writeDataToFile(FILE *objFile, DataWordsTable** DataWordRoot, int IC);


/*
Gets a pointer to the data words table linked list root pointer.
Free the memory of data words table linked list.
*/
void freeDataWords(DataWordsTable **DataWordRoot);
