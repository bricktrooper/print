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

	return false;
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
		log_error("Could not find type specifier");
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
	log_success("Found type specifier '%c'", pattern[length - 1]);

	char * address = pattern;
	char * end = &pattern[length - 1];

	// SIGN //

	if (*address == '+' || *address == ' ')
	{
		if (format.type == TYPE_S || format.type == TYPE_C)
		{
			log_error("Cannot apply signs to non-numeric type");
			return ERROR;
		}

		format.sign = get_sign(*address);
		log_success("Found sign specifier '%c'", *address);
		address++;
	}
	else
	{
		format.sign = SIGN_NEGATIVE_ONLY;
		log_warning("Defaulting to only negative signs'");
	}

	// ALIGNMENT //

	if (*address == '-')
	{
		format.alignment = ALIGNMENT_LEFT;
		log_success("Found alignment specifier '%c'", *address);
		address++;
	}
	else
	{
		format.alignment = ALIGNMENT_RIGHT;
		log_warning("Defaulting to right alignment'");
	}

	// ZEROS //

	if (*address == '0')
	{
		if (format.type == TYPE_S || format.type == TYPE_C)
		{
			log_error("Cannot apply leading zeros to non-numeric type");
			return ERROR;
		}

		log_success("Found leading zeros specifier");
		format.zeros = true;
		address++;
	}
	else
	{
		log_warning("Defaulting to no leading zeros'");
		format.zeros = false;
	}

	// PADDING //

	int digits;
	format.padding = parse_int(address, end - address, &digits);

	if (digits > 0)
	{
		log_success("Found %d-digit padding specifier '%d'", digits, format.padding);
		address += digits;
	}
	else
	{
		log_warning("Defaulting to 0 padding'");
		format.padding = 0;
	}

	// ACCURACY //

	if (*address == '.')
	{
		if (format.type != TYPE_E && format.type != TYPE_F && format.type != TYPE_G)
		{
			log_error("Non-floating-point type contained accuracy specifiers");
			return ERROR;
		}

		int digits;
		format.accuracy = parse_int(address + 1, end - address, &digits);

		if (digits > 0)
		{
			log_success("Found %d-digit accuracy specifier '%d'", digits, format.accuracy);
			address += (digits + 1);
		}
		else
		{
			format.accuracy = 6;
			log_warning("Defaulting to accuracy '%d'", format.accuracy);
		}
	}
	else if (address == end)
	{
		format.accuracy = 6;
		log_warning("Defaulting to accuracy '%d'", format.accuracy);
	}

	return length;
}

int test(void)
{
	char string [] = {"%-13s %+-012.31f 0x%-0X\r\n"};

	char * address = string;
	char current = *address;
	int increment;

	while (current != '\0')
	{
		current = *address;
		increment = 1;
		log_debug("%c", current);

		if (current == '%')
		{
			char next = *(address + 1);

			if (next == '\0')
			{
				log_error("Missing format at end of string");
				return ERROR;
			}
			else if (next == '%')
			{
				log_info("Escaping '%%'");
				increment = 2;
			}
			else
			{
				increment = parse_format(address + 1) + 1;

				if (increment == ERROR)
				{
					log_error("Failed to parse string format");
					return ERROR;
				}
			}
		}

		address += increment;
	}

	printf("%s", string);

	return sizeof(string) - 1;
}
