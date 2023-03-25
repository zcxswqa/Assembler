/* Yam Tiram 312127814 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define LINE_SIZE 90
#define MAX_LINE_SIZE 80
#define MAX_COMMAND_NAME_SIZE 5
#define WHITESPACES " \n\t\v\f\r"
#define STARTING_ADDRESS 100

enum what{comment, lable, command, directive, variable, reg};					/* Used to classify words from file.*/
enum directiveType{invalid, data, string, ext, entry};							/* Used to classify directives. */
