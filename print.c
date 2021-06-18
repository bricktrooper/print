#include "print.h"

#include <stdbool.h>

#include "log.h"

#define SUCCESS   0
#define ERROR    -1

enum Sign
{
	SIGN_POSITIVE,
	SIGN_NEGATIVE,
	SIGN_INVISIBLE_POSITIVE,
	SIGN_UNKNOWN
};

enum Alignment
{
	ALIGNMENT_LEFT,
	ALIGNMENT_RIGHT,
	ALIGNMENT_UNKNOWN
};

enum Type
{
	TYPE_C,
	TYPE_D,
	TYPE_E,
	TYPE_F,
	TYPE_G,
	TYPE_I,
	TYPE_O,
	TYPE_S,
	TYPE_U,
	TYPE_P,
	TYPE_X_LOWER_CASE,
	TYPE_X_UPPER_CASE,
	TYPE_UNKNOWN
};

typedef enum Sign Sign;
typedef enum Alignment Alignment;
typedef enum Type Type;

struct Format
{
	Type type;
	Alignment alignment;
	Sign sign;
	bool zeros;
	int padding;
	int accuracy;
};

typedef struct Format Format;

char SPECIFIERS [] = {' ', '+', '-', '0'};
int NUM_SPECIFIERS = sizeof(SPECIFIERS);

char FORMATS [] = {'c', 'd', 'e', 'f', 'g', 'i', 'o', 's', 'u', 'p', 'x', 'X'};
int NUM_FORMATS = sizeof(FORMATS);

//void print_format(Format * format)
//{

//}

Type get_type(char character)
{
	switch (character)
	{
		case 'c': return TYPE_C;
		case 'd': return TYPE_D;
		case 'e': return TYPE_E;
		case 'f': return TYPE_F;
		case 'g': return TYPE_G;
		case 'i': return TYPE_I;
		case 'o': return TYPE_O;
		case 's': return TYPE_S;
		case 'u': return TYPE_U;
		case 'p': return TYPE_P;
		case 'x': return TYPE_X_LOWER_CASE;
		case 'X': return TYPE_X_UPPER_CASE;
		default:  return TYPE_UNKNOWN;
	}
}

Sign get_sign(char character)
{
	switch (character)
	{
		case '+': return SIGN_POSITIVE;
		case '-': return SIGN_NEGATIVE;
		case ' ': return SIGN_INVISIBLE_POSITIVE;
		default:  return SIGN_UNKNOWN;
	}
}

bool is_type(char character)
{
	for (int i = 0; i < NUM_FORMATS; i++)
	{
		if (character == FORMATS[i])
		{
			return true;
		}
	}
}

int get_length(char * pattern)
{
	char * address = pattern;

	while (!is_type(*address))
	{
		if (*address == '\0')
		{
			log_error("Could not find type specifier for '%s'", pattern);
			return ERROR;
		}

		address++;
	}

	return address - pattern + 1;
}

int parse_int(char * string, int length)
{
	int value = 0;
	int digit;
	bool done = false;

	for (int i = 0; i < length && !done; i++)
	{
		switch (string[i])
		{
			case '0':
				digit = 0;
				break;
			case '1':
				digit = 1;
				break;
			case '2':
				digit = 2;
				break;
			case '3':
				digit = 3;
				break;
			case '4':
				digit = 4;
				break;
			case '5':
				digit = 5;
				break;
			case '6':
				digit = 6;
				break;
			case '7':
				digit = 7;
				break;
			case '8':
				digit = 8;
				break;
			case '9':
				digit = 9;
				break;
			default:
				done = true;
				break;
		}

		if (!done)
		{
			value = (value * 10) + digit;
		}
	}

	return value;
}

int parse_format(char * pattern)
{
	int length = get_length(pattern);
	log_debug("Length: %d", length);

	Format format;
	format.type = TYPE_UNKNOWN;
	format.alignment = ALIGNMENT_UNKNOWN;
	format.sign = SIGN_UNKNOWN;
	format.zeros = false;
	int padding = ERROR;
	int accuracy = ERROR;

	format.type = get_type(pattern[length - 1]);

	for (int i = 0; i < length - 1; i++)
	{
		char character = pattern[i];
		log_info("%c", character);

		switch (character)
		{
			case '+':
			case '-':
			case ' ':
			{
				if (format.sign != SIGN_UNKNOWN)
				{
					log_error("Found multiple sign specifiers in '%s'", pattern);
					return ERROR;
				}

				format.sign = get_sign(character);
				break;
			}
			case '.':
			{
				// parse accuracy
				break;
			}
			case 0:
			{
				if (format.zeros)
				{
					log_error("Found multiple leading zeros specifiers in '%s'", pattern);
					return ERROR;
				}

				format.zeros = true;
				break;
			}
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			{
				// parse padding
				break;
			}
			default:
			{
				log_error("Unknown format specifier '%c'", character);
				return ERROR;
			}
		}
	}
}

int test(void)
{
	// when you find the %, run a parser to extract only the format specifier pattern.
	char num [] = "1";
	int res = parse_int(num, sizeof(num) - 1);
	log_debug("%d %u 0x%X", res, res);
	return parse_format("-3.2d");
}
