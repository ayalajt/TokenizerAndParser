// Assignment 4: Parser.c
// CMSC 403
// Jesus Ayala
// September 27th, 2020

#include <stdio.h>
#include "Givens.h"

// First define all functions requried in the EBNF grammar, a counter to iterate
// through a lexical array, and an enum token that can be used to grab the next 
// token in a lexical array
enum token nextTok;
void peek(struct lexics *someLexics, int numLexs);
_Bool function(struct lexics *someLexics, int numLexs);
_Bool header(struct lexics *someLexics, int numLexs);
_Bool argsDecl(struct lexics *someLexics, int numLexs);
_Bool body(struct lexics *someLexics, int numLexs);
_Bool statementList(struct lexics *someLexics, int numLexs);
_Bool assignment(struct lexics *someLexics, int numLexs);
_Bool statement(struct lexics *someLexics, int numLexs);
_Bool returnT(struct lexics *someLexics, int numLexs);
_Bool whileT(struct lexics *someLexics, int numLexs);
_Bool term(struct lexics *someLexics, int numLexs);
_Bool expression(struct lexics *someLexics, int numLexs);
int counter = 0;

// This function is used to grab the next token in a lexical array,
// provided it does not go over the total number of lexical structures
void peek (struct lexics *someLexics, int numLexs) {
	if (counter < numLexs) {
		nextTok = someLexics[counter].token;
	}
}

_Bool parser(struct lexics *someLexics, int numberOfLexics) {
	
	// Start with the first EBNF grammar call, which is function
	int value = function(someLexics, numberOfLexics);
	if (value == 1) {
		
		// If function was parsed correctly and every lexical
		// structure in the lexical array was checked, then
		// the lexical array has been parsed correctly. 
		// Otherwise return 0 since it failed
		if (counter == numberOfLexics) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}

_Bool function(struct lexics *someLexics, int numLexs) {
	
	// Following the EBNF grammar, function calls header and body.
	// If header is parsed correctly, then continue to parse body
	int headerVal = header(someLexics, numLexs);
	if (headerVal == 1) {
		int bodyVal = body(someLexics, numLexs);
		if (bodyVal == 1) {
			return 1;	
		}
		else {
			return 0;
		}
	}
}

_Bool header(struct lexics *someLexics, int numLexs) {
	
	// In header, iterate through the array, making sure the 
	// tokens are in valid order
	if (someLexics[counter].token == VARTYPE) {
		counter++;
		if (someLexics[counter].token == IDENTIFIER) {
			counter++;
			if (someLexics[counter].token == LEFT_PARENTHESIS) {
				counter++;
				
				// Call argsVal and if argsVal is parsed correctly,
				// then just verify there is a right parenthesis token
				int argsVal = argsDecl(someLexics, numLexs);
				if (argsVal == 1) {
					if (someLexics[counter].token == RIGHT_PARENTHESIS) {
						counter++;
						return 1;
					}
					return 0;
				}
			}
		}
	}
}

_Bool body(struct lexics *someLexics, int numLexs) {
	
	// In body, check if the brackets 
	if (someLexics[counter].token == LEFT_BRACKET) {
		counter++;
		peek(someLexics, numLexs);

		// If the next token is a right bracket, then
		// the statement list is empty, and this is still valid,
		// so body is finished here
		if (nextTok == RIGHT_BRACKET) {
			counter++;
			return 1;
		}

		// Call statementList and if it is valid, verify there
		// is a right bracket for it to be correct
		int stmtListVal = statementList(someLexics, numLexs);
		if (stmtListVal == 1) {
			if (someLexics[counter].token == RIGHT_BRACKET) {
				counter++;
				return 1;
			}
		}	
	}
	return 0;	
}

_Bool argsDecl(struct lexics *someLexics, int numLexs) {
	
	// First check if argsDecl is essentially empty. If
	// it is, it is still valid so return 1
	peek(someLexics, numLexs);
	if (nextTok == RIGHT_PARENTHESIS) {
		return 1;
	}
	
	// Otherwise verify all tokens are in their correct order
	if (someLexics[counter].token == VARTYPE) {
		counter++;
		if (someLexics[counter].token == IDENTIFIER) {
			counter++;

			// If there is a comma after an IDENTIFIER, then
			// recursively call argsDecl to make sure the tokens after
			// the comma are in the correct order
			peek(someLexics, numLexs);
			if (nextTok == COMMA) {
				counter++;
				if (someLexics[counter].token == VARTYPE) {
					counter++;
					if (someLexics[counter].token == IDENTIFIER) {
						counter++;
						peek(someLexics, numLexs);
					}
				}
				int argsVal = argsDecl(someLexics, numLexs);	
			}
			return 1;
		}
	}
	return 0;	
}

_Bool statementList(struct lexics *someLexics, int numLexs) {

	// In statement list, to check for multiple statements, check
	// if next token is the first token of a while loop, return, assignment, 
	// or body, then recursively call statement list until each statement has been
	// checked for proper tokenization 
	peek(someLexics, numLexs);	
	int statementVal;
	if (nextTok == IDENTIFIER || nextTok == RETURN_KEYWORD || nextTok == WHILE_KEYWORD || nextTok == LEFT_BRACKET) {
		statementVal = statement(someLexics, numLexs);
		peek(someLexics, numLexs);
		int stmtListVal = statementList(someLexics, numLexs);
	}
	if (statementVal == 1) {
		return 1;
	}
	return 0;
}

_Bool statement(struct lexics *someLexics, int numLexs) {
	
	// In statement, grab the next token in the array and use it to
	// choose the next function to go to
	peek(someLexics, numLexs);
	if (nextTok == IDENTIFIER) {
		int assignmentVal = assignment(someLexics, numLexs);
		if (assignmentVal == 1) {
			return 1;
		}	
	}
	if (nextTok == RETURN_KEYWORD) {
		int returnVal = returnT(someLexics, numLexs);
		if (returnVal == 1) {
			return 1;
		}
	}
	if (nextTok == WHILE_KEYWORD) {
		int whileVal = whileT(someLexics, numLexs);
		if (whileVal == 1) {
			return 1;
		}
	}
	if (nextTok == LEFT_BRACKET) {
		int bodyVal = body(someLexics, numLexs);
		if (bodyVal == 1) {
			return 1;
		}	
	}
	return 0;	
}

_Bool whileT(struct lexics *someLexics, int numLexs) {

	// In while, verify the tokens obey the EBNF grammar
	// and then call the respective functions in the rule
	if (someLexics[counter].token == WHILE_KEYWORD) {
		counter++;
		if (someLexics[counter].token == LEFT_PARENTHESIS) {
			counter++;
			int expressionVal = expression(someLexics, numLexs);
			if (expressionVal == 1) {
				if (someLexics[counter].token == RIGHT_PARENTHESIS) {
					counter++;
					int statementVal = statement(someLexics, numLexs);
					if (statementVal == 1) {
						return 1;
					}
				}
			}
		}
	}
	return 0;				
}

_Bool returnT(struct lexics *someLexics, int numLexs) {

	// In return, verify the keyword is there as well as a
	// valid expression and then an EOL token
	if (someLexics[counter].token == RETURN_KEYWORD) {
		counter++;
		int expressionVal = expression(someLexics, numLexs);
		if (expressionVal == 1) {
			if (someLexics[counter].token == EOL) {
				counter++;	
				return 1;
			}
		}
	}
	return 0;		
}

_Bool assignment(struct lexics *someLexics, int numLexs) {
	
	// In assignment, verify the tokens are in the order
	// of the EBNF grammar and then verify the expression is
	// valid
	if (someLexics[counter].token == IDENTIFIER) {
		counter++;
		if (someLexics[counter].token == EQUAL) {
			counter++;
			int expressionVal = expression(someLexics, numLexs);
			if (expressionVal == 1) {
				if (someLexics[counter].token == EOL) {
					counter++;
					return 1;
				}
			}
		}
	}	
	return 0;
}

_Bool expression(struct lexics *someLexics, int numLexs) {

	// In expression, grab the next token to see which rule is being chosen.
	// If it is a number or an identifier, then continue on to term. If after the
	// term has been parsed there is a BINOP, then call term again
	peek(someLexics, numLexs);
	if (nextTok == NUMBER || nextTok == IDENTIFIER) {
		int termVal = term(someLexics, numLexs);
		if (termVal == 1) {
			peek(someLexics, numLexs);
			if(nextTok == BINOP) {
				counter++;
				termVal = term(someLexics, numLexs);
				peek(someLexics, numLexs);
			}
			return 1;
		}	
	}

	// If the next token is a left parenthesis, then recursively call expression until
	// the expression has been tokenized and then verify there is a right parenthesis after
	if (nextTok == LEFT_PARENTHESIS) {
		counter++;
		int expressionVal = expression(someLexics, numLexs);
		if (expressionVal == 1) {
			if (someLexics[counter].token == RIGHT_PARENTHESIS) {
				counter++;
				return 1;
			}
		}
	}
	return 0;
}

_Bool term(struct lexics *someLexics, int numLexs) {

	// For term, simply verify there is a number or an identifier
	if (someLexics[counter].token == NUMBER || someLexics[counter].token == IDENTIFIER) {
		counter++;
		return 1;
	}
	return 0;			
}
