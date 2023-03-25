/* Yam Tiram 312127814 */


/*
Gets a pointer to ".as" file, a char pointer to ".am" file name, and a pointer
to a macro linked list root pointer.
Makes an assembly file in which all macros definitions are removed, and wherever
a macro was called, there is now that macro's content.
Retuens 1 if no macro syntax errors are found and 0 otherwise.
*/
int preAssemble(FILE *f, char *amfilename, Macro **MacroRoot);


/*
Gets a pointer to ".am" file, a char pointer to a filename with ".am",
instruction counter, data counter, a pointer to a macro linked list root pointer
and a pointer to lables linked list root pointer.
Runs over the source code one time only, checks and notifies for syntax errors,
makes the lables table (each lable in the source code is saved with it's address
and type) and advances the instruction and data counters in relation to the 
number of words each will take in the memory.
Returns 1 if no syntax errors are found and 0 otherwise.
*/
int firstPass(FILE *f, char *amFileName, int *IC, int *DC, Macro **macroRoot, LableTable **lableRoot);


/*
Gets a pointer to ".am" file, the file name with no extentions, instruction 
counter, data counter, a pointer to the macro linked list root pointer and a 
pointer to the lables linked list root pointer.
Runs over the source code one time only, checks and notifies for any entry
declared lables which were not used and if no errors found:
Makes ".obj" file in which the source code is translated to binary words each
with its memory address
If the source code contained any extern directives, makes ".ext" file in which
any extern-lable call is documented.
If the source code contained any entry directives, makes ".ent" file in which
any entry-declared lable is documented with it's address.
Returns 1 if no syntax errors are found and 0 otherwise.
*/
int second(FILE *f, char *filename, int *IC, int *DC, Macro **macroRoot, LableTable **lableRoot);
