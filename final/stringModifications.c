/* Yam Tiram 312127814 */

#include "header.h"


/*
Check the linr letter by letter for any non white-space character.
Retrns 1 if line composed of white-spaces only, and 0 otherwise.
*/
int isempty(char *line)
{
	while(*line != '\0')
	{
		if(!isspace(*line))
			return 0;
		line++;
	}
	return 1;
}


/*
Removes white-spaces from beginning of a string and terminating the string after
all non white-space characters.
*/
void removeWhitespaces(char *line)
{
	int length = strlen(line);
	int i;
	int j = 0;
	i = 1;
	while(isspace(line[length-i]))
		i++;
	line[length-i+1] = '\0';
	i = 0;
	while(isspace(line[i]))
		i++;
	while(line[i] != '\0')
		line[j++] = line[i++];
	line[j] = '\0';
}



/*
Removes the first letter of a word. (used to remove "#" from immidiate operands)
*/
void removeFirstLetter(char *word)
{
	char *p = word;
	p++;
	while(*p != '\0')
		*word++ = *p++;
	*word = '\0';
}
