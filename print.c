#include "print.h"

#include <stddef.h>
#include <stdbool.h>

#include "log.h"

#define SUCCESS   0
#define ERROR    -1

enum Sign
{
	SIGN_BOTH,
	SIGN_INVISIBLE_POSITIVE,
	SIGN_NEGATIVE_ONLY,
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
		case '+': return SIGN_BOTH;
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
			return ERROR;
		}

		address++;
	}

	return address - pattern + 1;
}

int parse_int(char * string, int length, int * num_digits)
{
	int value = 0;
	int digit;
	bool done = false;
	int count = 0;

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
			count++;
		}
	}

	if (num_digits != NULL)
	{
		*num_digits = count;
	}

	return value;
}

int parse_format(char * pattern)
{
	int length = get_length(pattern);

	if (length < 1)
	{
		log_error("[ %%%s ] Could not find type specifier", pattern);
		return ERROR;
	}

	log_debug("Length: %d", length);

	Format format;
	format.type = TYPE_UNKNOWN;
	format.alignment = ALIGNMENT_UNKNOWN;
	format.sign = SIGN_UNKNOWN;
	format.zeros = false;
	format.padding = ERROR;
	format.accuracy = ERROR;

	format.type = get_type(pattern[length - 1]);
	log_success("[ %%%s ] Found type specifier '%c'", pattern, pattern[length - 1]);

	char * address = pattern;
	char * end = &pattern[length - 1];
	char character;

	// SIGN //

	if (*address == '+' || *address == ' ')
	{
		if (format.type == TYPE_S || format.type == TYPE_C)
		{
			log_error("[ %%%s ] Cannot apply signs to non-numeric type", pattern);
			return ERROR;
		}

		format.sign = get_sign(*address);
		log_success("[ %%%s ] Found sign specifier '%c'", pattern, *address);
		address++;
	}
	else
	{
		format.sign = SIGN_NEGATIVE_ONLY;
		log_warning("[ %%%s ] Defaulting to only negative signs'", pattern);
	}

	// ALIGNMENT //

	if (*address == '-')
	{
		format.alignment = ALIGNMENT_LEFT;
		log_success("[ %%%s ] Found alignment specifier '%c'", pattern, *address);
		address++;
	}
	else
	{
		format.alignment = ALIGNMENT_RIGHT;
		log_warning("[ %%%s ] Defaulting to right alignment'", pattern);
	}

	// ZEROS //

	if (*address == '0')
	{
		if (format.type == TYPE_S || format.type == TYPE_C)
		{
			log_error("[ %%%s ] Cannot apply leading zeros to non-numeric type", pattern);
			return ERROR;
		}

		log_success("[ %%%s ] Found leading zeros specifier", pattern);
		format.zeros = true;
		address++;
	}
	else
	{
		log_warning("[ %%%s ] Defaulting to no leading zeros'", pattern);
		format.zeros = false;
	}

	// PADDING //

	int digits;
	format.padding = parse_int(address, end - address, &digits);

	if (digits > 0)
	{
		log_success("[ %%%s ] Found %d-digit padding specifier '%d'", pattern, digits, format.padding);
		address += digits;
	}
	else
	{
		log_warning("[ %%%s ] Defaulting to 0 padding'", pattern);
		format.padding = 0;
	}

	// ACCURACY //

	if (*address == '.')
	{
		if (format.type != TYPE_E && format.type != TYPE_F && format.type != TYPE_G)
		{
			log_error("[ %%%s ] Non-floating-point type contained accuracy specifiers", pattern);
			return ERROR;
		}

		int digits;
		format.accuracy = parse_int(address + 1, end - address, &digits);

		if (digits > 0)
		{
			log_success("[ %%%s ] Found %d-digit accuracy specifier '%d'", pattern, digits, format.accuracy);
			address += (digits + 1);
		}
		else
		{
			log_warning("[ %%%s ] Defaulting to accuracy 6'", pattern);
			format.accuracy = 6;
		}
	}
	else if (address == end)
	{
		log_warning("[ %%%s ] Defaulting to accuracy 6'", pattern);
		format.accuracy = 6;
	}
}

int test(void)
{
	// when you find the %, run a parser to extract only the format specifier pattern.
	// need to fix the parser so that it scans the flags before the rest (do it linearly)
	return parse_format("-13s");
}
