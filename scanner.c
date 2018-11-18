/**
 * @file scanner.h
 * @author Tomas Dorda (xdorda00), Jakub Konetzny ()
 * @brief Error codes constants and function to print errors.
 */


#include "scanner.h"
#include "dynamic_string.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include "error.h"


//end of includes

Dynamic_string *contentstring;
FILE *source_file;

void getToken(tToken *token)
{
	if (dynamic_string == NULL || source_file == NULL)
	{
            ErrorPrint(1,"");
	}


	token->content_string = dynamic_string;


	//Setuji STATE na DEFAULT
	SCANNER_STATE current_state = START;
	token->set_type_of_token = EMPTY;

	//Declaration of the scanner char
	char c = 0;

	while (true)
	{
	
	c = getc(source_file);

	switch(current_state)
	{
		case (START):
		
			if (c == '\n')
			{
				current_state = EOL;
			}
			else if (c == '/')
			{
				current_state = BACKSLASH;
			}
			else if (c == '\'')
			{
				current_state = COMMENT_START;
			}
			else if (c == '_' || isalpha(c))
			{
				current_state = KEYWORD;
			}
			else if (isdigit(c))
			{
				current_state = NUMBER;
			}
			else if (c == '<')
			{
				current_state = LESS_THAN;
			}
			else if (c == '>')
			{
				current_state = GREATER_THAN;
			}
			else if (c == '=')
			{
				token->set_type_of_token = CHAR_ASSIGN;
			}
			else if (isspace(c))
			{
				current_state = START;
			}
			
			else
			{
			
			}
			break;

			case (SCANNER_STATE_EOL):
				if (isspace(c))
				{
					break;
				}

				ungetc(c, source_file);
				token->set_type_of_token = CHAR_EOF;

			case (NUMBER):
				if (isdigit(c))
				{
					if ()// ALLOC CHECK

				}
				else if (c == '.')
				{
					state = NUMBER_DEC;
				}
				else if (tolower(c) == 'e')
				{
					state = NUMBER_EXP;
				}
				else
				{
					ungetc(c, source_file);
					//process_integer
				}

				break;

			case (NUMBER_DEC):
			
				if (isdigit(c))
				{
					state = NUMBER_DOUBLE;
				}
				else //clean

				break;

			case (NUMBER_DOUBLE):
			
				if (isdigit(c))
				{
					//addto string

				}

				else if (tolower(c) == 'e')
				{
					state = NUMBER_EXP;
					//add
				}

				else
				{
					ungetc(c, source_file);
					//process_decimal
				}
				break;

				case (NUMBER_EXP):
					if (isdigit(c))
					{
						state = NUMBER_EXP_DONE;
						//add;
					}
					else if (c == '-' || c == '+')
					{
						state = NUMBER_EXP_SIGN;
						//add;
					}
					else //return free;

					break;


			}
			}
		
	}


	}
}


