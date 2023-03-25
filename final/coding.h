/* Yam Tiram 312127814 */

#define WORD_SIZE 15
#define OPCODE_ADDRESSING_SIZE 4
#define OPERAND_ADDRESSING_SIZE 2
#define REGISTER_ADDRESSING_SIZE 6
#define AER_SIZE 2

enum ARE{A,E,R};																/* Used for coding ARE bits. */


/*
Gets a FILE pointer to the object file and an int pointer to address.
Writes addresses to the object file.
Advances address by one.
*/
void writeAddressToFile(FILE *objFile, int *address);


/*
Gets a FILE pointer to the object file and the amount of bits to zero.
Write that amount of zeroes to the file.
*/
void writeZerosToFile(FILE *objFile, int amount);


/*
Gets a FILE pointer to the object file and a command number.
Writes the command opcode to the file.
*/
void writeOPCodeToFile(FILE *objFile, int commandNumber);


/*
Gets a FILE pointer to the object file and a char pointer to an operand.
Write the operand addressing to the file.
*/
void writeOperandAddressingToFile(FILE *objFile, char *operand);


/*
Gets a FILE pointer to the object file, an unsigned int to write and an int
for how many bits of representation.
Writes the unsigend integer binary representation to the file.
*/
void writeBitsToFile(FILE *objFile, unsigned num, int representation);


/*
Gets a FILE pointer to the object file, an int pointer to the instruction
counter, an enumeration of sourceOrDest, a pointer to lable-table linked list
root pointer and a char pointer to the ".ext" file name.
Directs an operand to the right coding function based on his addressing type.
*/
void codeOneOperand(FILE *objFile, int *IC, char *OP, enum sourceOrDest SD, LableTable **lableRoot, char *externFileName);


/*
Gets a FILE pointer to the object file, an int pointer to the instruction
counter, a pointer to lable-table linked list root pointer and a char pointer
to the ".ext" file name.
writes lable's address to the object file, if it's a ".extern"-declared lable,
writes its name and it's usage address.
*/
void codeLable(FILE *objFile, int *IC, char *operand, LableTable **lableRoot, char *externFileName);


/*
Gets a FILE pointer to the object file, an int pointer to the instruction
counter and a char pointer to an operand.
Code and write immidiate variable to the object file.
*/
void codeImmidiate(FILE *objFile, int *IC, char *operand);


/*
Gets a FILE pointerto the object file, an int pointer to the instruction
counter, and an enumeration of sourceOrDest.
Code and write register number to th object file.
*/
void codeOneRegister(FILE *objFile, int *address, char *OP, enum sourceOrDest SD);


/*
Gets a FILE pointer to the object file, an int pointer to the instruction
counter, a char pointer to the source operand and a char pointer to the 
destination operand.
Code and write two registers numbers to the object file as one word.
*/
void codeTwoRegisters(FILE *objFile, int *IC, char *Source, char *Dest);
