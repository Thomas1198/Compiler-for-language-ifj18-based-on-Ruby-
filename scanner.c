/**
 * @file scanner.c
 * @author Patrik Strnad (xstrna11)
 * @brief Error codes constants and function to print errors.
 */


#include "scanner.h"
#include "dynamic_string.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include "error.h"


//end of includes

Dynamic_string *content_string;
FILE *source_file;

void getToken(tToken *token)
{
	if (content_string == NULL || source_file == NULL)
	{
            ErrorPrint(1,"");
	}


	token->content_string = content_string;

	//Setuji STATE na DEFAULT
	SCANNER_STATE current_state = START;
	token->set_type_of_token = EMPTY;

	if (!dynamic_string_init(content_string))
	{
		ErrorPrint(1,"");
	}

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
			else if (c == '#')
			{
				current_state = COMMENT_START;
			}
			else if (c == '_' || isalpha(c))
			{
				if (!dynamic_string_add_char(str, (char) tolower(c)))
				{
					dynamic_string_free(str);
					return;
				}
				current_state = KEYWORD;
			}
			else if (isdigit(c))
			{
				if (!dynamic_string_add_char(str, c))
				{
					dynamic_string_free(str);
					ErrorPrint(1,"");
				}
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
				current_state = EQUALS;
			}
			else if (isspace(c))
			{
				current_state = START;
			}

			else if (c == '+')
			{
				token->set_type_of_token = CHAR_OPERATOR_PLUS;
				dynamic_string_free(str);
				return;
			}
			else if (c == '-')
			{
				token->set_type_of_token = CHAR_OPERATOR_MINUS;
				dynamic_string_free(str);
				return;
			}
			else if (c == '*')
			{
				token->set_type_of_token = CHAR_OPERATOR_MUL;
				dynamic_string_free(str);
				return;
			}
			else if (c == ',')
			{
				token->set_type_of_token = CHAR_COMMA;
				dynamic_string_free(str);
				return;
			}
			else if (c == '(')
			{
				token->set_type_of_token = CHAR_LEFT_BRACKET;
				dynamic_string_free(str);
				return;
			}
			else if (c == ')')
			{
				token->set_type_of_token = CHAR_RIGHT_BRACKET;
				dynamic_string_free(str);
				return;
			}
			else if (c == ';')
			{
				token->set_type_of_token = CHAR_SEMICOLON;
				dynamic_string_free(str);
				return;
			}

			else if (c == EOF)
				{
					token->type = CHAR_EOF;
					dynamic_string_free(str);
					return;
				}

			else
			{
				dynamic_string_free(str);
				return false;
			}
			break;
			
			//__________________

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
					if (!dynamic_string_add_char(str, c))
					{
						dynamic_string_free(str);
						ErrorPrint(1,"");
					}

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
				else dynamic_string_free(str);

				break;

			case (NUMBER_DOUBLE):
			
				if (isdigit(c))
				{
					if (!dynamic_string_add_char(str, c))
					{
						dynamic_string_free(str);
						ErrorPrint(1,"");
					}

				}

				else if (tolower(c) == 'e')
				{
					state = NUMBER_EXP;
					if (!dynamic_string_add_char(str, c))
					{
						dynamic_string_free(str);
						ErrorPrint(1,"");
					}
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
						if (!dynamic_string_add_char(str, c))
					{
						dynamic_string_free(str);
						ErrorPrint(1,"");
					}
					}
					else if (c == '-' || c == '+')
					{
						state = NUMBER_EXP_SIGN;
						if (!dynamic_string_add_char(str, c))
					{
						dynamic_string_free(str);
						ErrorPrint(1,"");
					}
					}
					else dynamic_string_free(str);

					break;

				case (BACKSLASH):
				if (c == '/')
				{
					state = BACKSLASH;
				}
				else
				{
					ungetc(c, source_file);
					token->set_type_of_token = CHAR_OPERATOR_DIV;
					dynamic_string_free(str);
					return;
				}

				break;

				case (COMMENTARY):
					if (c == '\n' || c == EOF)
				{
					state = START;
					ungetc(c, source_file);
				}
				break;

				case (EQUALS):
				if (isalpha(c))
				{
					if (!dynamic_string_add_char(str, (char) tolower(c)))
					{
						//FSEEK VOLE
					}
				}
			}
			}
		
	}


	}
}


