// Assignment 4: Tokenizer.c
// Jesus Ayala
// CMSC 403
// September 27th, 2020

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Givens.h"
#include "Tokenizer.h"

// This function sets a string into its respective token
enum token tokenFinder(char *lexeme) {
	if (strcmp(lexeme, "(") == 0) {
		return LEFT_PARENTHESIS;
	}
	else if (strcmp(lexeme, ")") == 0) {        
              	return RIGHT_PARENTHESIS;
	}
	else if (strcmp(lexeme, "{") == 0) {
		return LEFT_BRACKET;
	}
	else if (strcmp(lexeme, "}") == 0) {
		return RIGHT_BRACKET;
	}
	else if (strcmp(lexeme, "while") == 0) {
		return WHILE_KEYWORD;
	}
	else if (strcmp(lexeme, "return") == 0) {
		return RETURN_KEYWORD;
	}
	else if (strcmp(lexeme, "=") == 0) {
		return EQUAL;	
	}
	else if (strcmp(lexeme, ",") == 0) {
		return COMMA;
	}
	else if (strcmp(lexeme, ";") == 0) {
		return EOL;
	}
	else if (strcmp(lexeme, "int") == 0) {
		return VARTYPE;
	}
	else if (strcmp(lexeme, "void") == 0) {
		return VARTYPE;
	}
	else if (strcmp(lexeme, "+") == 0) {
		return BINOP;
	}
	else if (strcmp(lexeme, "*") == 0) {
		return BINOP;
	}
	else if (strcmp(lexeme, "!=") == 0) {
		return BINOP;
	}
	else if (strcmp(lexeme, "==") == 0) {
		return BINOP;
	}
	else if (strcmp(lexeme, "%") == 0) {
		return BINOP;
	}
	else if (validIdentifier(lexeme) == 1) {
		return IDENTIFIER;
	}
	else if (validNumber(lexeme) == 1) {
		return NUMBER;
	}
	else {
		return -1;
	}
}

// This function sets a new lexical structure into the lexical array
// after finding the lexeme's token
void aLexAssigner(struct lexics *aLex, int *numLex, char *lexeme) {
	struct lexics newElement;
	newElement.token = tokenFinder(lexeme);
	strcpy(newElement.lexeme, lexeme);
	aLex[*numLex] = newElement;
	*numLex = *numLex + 1;
}

_Bool tokenizer(struct lexics *aLex, int *numLex, FILE *inf) {
	
	// to tokenize a string, first use fgets to get the text file
	// into one single string
	char line[MY_CHAR_MAX];
	char delim[] = " ";
	char *ptr;
	//char getChar[MY_CHAR_MAX];
	while ((fgets(line, MY_CHAR_MAX, inf)) != NULL) {
		int i = 0;

		// Iterate through the string and change any possible whitespace
		// to a single space so the delimmiter can work properly later on
		while (i < strlen(line)) {
			char getChar[MY_CHAR_MAX];
			if ((line[i] == '\t') || (line[i] == '\n')) {
				line[i] = ' ';
			}
			//printf("String before getting cut up: %s\n", line);
			// If the current character is a valid single character lexeme, then spaces have to be placed before and after the character
			if ((line[i] == '(') || (line[i] == ')') || (line[i] == '{')  || (line[i] == '}') || (line[i] == ',') || (line[i] == ';')
				|| (line[i] == '+') || (line[i] == '*') || (line[i] == '%')) {
			//	char getChar[MY_CHAR_MAX];
				// For a space before the character, grab the string with the character, replace the character with a space, 
				// then add the string that was after the character at the index + 1 
				strncpy(getChar, line + i, strlen(line));
				line[i] = ' ';
				int j = i;
				int k = 0;
				while (j < strlen(line)) {
					line[j+1] = getChar[k];
					k++;
					j++;
				}
				// For a space after the character, grab the string after the character, replace the current index with a space,
				// then add the string that was after the character at the index + 2
				strncpy(getChar, line + i + 2, strlen(line));
				line[i+2] = ' ';
				int t = i + 2;
				int l = 0;
				while (t < strlen(line)) {
					line[t+1] = getChar[l];
					t++;
					l++;
				}
				i = i + 2;
			}

			// Here, if there is an equal sign, check if there is another equal sign right after, since "==" is a valid token. If it is
			// "==", then add spaces before and after the operator. If not, repeat the single character space operation to the single equal sign
			else if (line[i] == '=') {
				if (line[i+1] == '=') {
					strncpy(getChar, line + i + 1, strlen(line));
					line[i] = ' ';
					int j = i;
					int k = 0;
					while (j < strlen(line)) {
						line[j+2] = getChar[k];
						k++;
						j++;
					}
					strncpy(getChar, line + i + 3, strlen(line));
					line[i+3] = ' ';
					int t = i + 3;
					int l = 0;
					while (t < strlen(line)) {
						line[t+1] = getChar[l];
						t++;
						l++;
					}
					i = i + 3;
				}
				else {
					strncpy(getChar, line + i, strlen(line));
					line[i] = ' ';
					int j = i;
					int k = 0;
					while (j < strlen(line)) {
						line[j+1] = getChar[k];
						k++;
						j++;
					}
					strncpy(getChar, line + i + 2, strlen(line));
					line[i+2] = ' ';
					int t = i + 2;
					int l = 0;
					while (t < strlen(line)) {
						line[t+1] = getChar[l];
						t++;
						l++;
					}
					i = i + 2;
				}
			}

			// Similarily, if the character is '!', check if the next character is '='. If it is, then it is a valid
			// token string, so add spaces before and after
			else if (line[i] == '!') {
				if (line[i+1] == '=') {
					strncpy(getChar, line + i + 1, strlen(line));
					line[i] = ' ';
					int j = i;
					int k = 0;
					while (j < strlen(line)) {
						line[j+2] = getChar[k];
						k++;
						j++;
					}
					strncpy(getChar, line + i + 3, strlen(line));
					line[i+3] = ' ';	
					int t = i + 3;
					int l = 0;
					while (t < strlen(line)) {
						line[t+1] = getChar[l];
						t++;
						l++;
					}
					line[i+1] = '!';
					i = i + 3;		
				}
			}
			else {
				i++;
			}
		}
		printf("The string is: %s\n", line);
		// Here, separate the string by using a space as a delimiter and then loop
		// through the string pointer until there are no more tokens to add
		ptr = strtok(line, delim);
		while (ptr != NULL) {
	
			// If it is a valid token, assign it to a new lexical structure in the lexical array
			if (tokenFinder(ptr) != -1) {
				aLexAssigner(aLex, numLex, ptr);
			}	

			// increment the string pointer to the next string value
			ptr = strtok(NULL, delim);
		}				
	}
	return 1;		
}

