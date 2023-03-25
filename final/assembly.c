/* Yam Tiram 312127814 */


#include "header.h"
#include "macros.h"
#include "lables.h"
#include "assembly.h"


/*
	This program will get assmebly files names (without their extentions) from
		the command line, and translate the assembly code to 14 bits binary
		coded words.
	In order to achive this, the program is divided to three major parts:
	
	1. Pre-Assembler - will find and monitor any macro declaration and call,
		and will produce a file without the declerations and calls but with the
		content of each macro wherever a call was made.
		
	2. First-Pass - will validate syntax of the assembly code and will print
		an error message for each invalid line describing what exactly is wrong.
		
	3. Second-Pass - will produce the final output files, in which the code is
		translated to binary and the usage of external and internal lables is
		documented.
	
	The program is split across many files each with it's own documentation.
	In order to know which files are connected you can browse the makefile or
	just follow the "#includes" at the beginning of each file.
*/
int main(int argc, char *argv[])
{
	int i;
	if(argc == 1)																/* Check if no files names were given. */
		printf("No files were given.\n");

	for(i = 1; i < argc; i++)													/* Loop over files names from command line. */
	{
		char *filename;
		Macro *macroRoot = NULL;												/* Macros linked list initialization. */
		LableTable *lableRoot = NULL;											/* Lables linked list initialization. */
		
		int IC = 0;
		int DC = 0;
		FILE *file;
		
		if((filename = malloc(strlen(argv[i])+4)) == NULL)
		{
			printf("MEMORY ALLOCATION ERROR: \"%s\" was not created.\n", filename);
			exit(1);
		}
		strcpy(filename, argv[i]);												/* Take file name. */
		strcat(filename, ".as");												/* Add the ".as" extention. */
		file = fopen(filename, "r");
		if(file == NULL)														/* If file does not exist, try the next one. */
		{
			printf("FILE ERROR: \"%s\" does not exist\n", filename);
			free(filename);
			continue;
		}
		else if(!preAssemble(file, argv[i], &macroRoot))						/* Try running Pre-Assembler. */
		{
			printf("\n\"%s\" failed Pre Assembly stage, moving to next file.\n\n\n", filename);
			free(filename);
			fclose(file);
			freeMacros(&macroRoot);												/* Free memory allocations for macros linked list. */
			continue;
		}
		else
		{
			fclose(file);
			printf("\"%s\" finished Pre Assembly stage successfully\n", filename);
		}

		strcpy(filename, argv[i]);												/* Take file name. */
		strcat(filename, ".am");												/* Add the ".am" extention. */
		file = fopen(filename, "r");
		if(file == NULL)
		{
			printf("FILE ERROR: Pre-Assembler did not create \"%s\"\n", filename);
			exit(0);
		}
		else if(!firstPass(file, filename, &IC, &DC, &macroRoot, &lableRoot))	/* Try running First-Pass. */
		{
			printf("\n\"%s\" failed First Pass stage, moving to next file.\n\n\n", filename);			
			free(filename);
			fclose(file);
			freeMacros(&macroRoot);												/* Free memory allocations for macros linked list. */
			freeLables(&lableRoot);												/* Free memory allocations for lables linked list. */
			continue;
		}
		else
			printf("\"%s\" finished First Pass stage successfully\n", filename);
		
		if(!second(file, argv[i], &IC, &DC,  &macroRoot, &lableRoot))			/* try running Second-Pass. */
		{
			printf("\n\"%s\" failed Second Pass stage, moving to next file.\n\n\n", filename);			
			free(filename);
			fclose(file);
			freeMacros(&macroRoot);												/* Free memory allocations for macros linked list. */
			freeLables(&lableRoot);												/* Free memory allocations for lables linked list. */
			continue;
		}
		else
			printf("\"%s\" finished Second Pass stage successfully\n\n", filename);

		fclose(file);
		free(filename);
		freeMacros(&macroRoot);													/* Free memory allocations for macros linked list. */
		freeLables(&lableRoot);													/* Free memory allocations for lables linked list. */
	}
	return 0;
}
