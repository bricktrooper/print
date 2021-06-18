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
	char * string;
	int length;
	Type type;
	Alignment alignment;
	Sign sign;
	bool leading_zeros;
};

typedef struct Format Format;

char SPECIFIERS [] = {' ', '+', '-', '0'};
int NUM_SPECIFIERS = sizeof(SPECIFIERS);

char FORMATS [] = {'c', 'd', 'e', 'f', 'g', 'i', 'o', 's', 'u', 'p', 'x', 'X'};
int NUM_FORMATS = sizeof(FORMATS);

Type type(char character)
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

int find_length(char * pattern)
{
	char * character = pattern;

	while (!is_type(*character))
	{
		if (*character == '\0')
		{
			log_error("Found NULL terminator when parsing for type character");
			return ERROR;
		}

		character++;
	}

	return character - pattern + 1;
}

int parse(char * pattern)
{
	Format format;

	format.length = find_length(pattern);
	log_debug("%d", format.length);
	return 0 ;

	//format.type = type(pattern[length - 1]));
	//return;

	//for (int i = 0; i < length; i++)
	//{
	//	char character = pattern[i];
	//	log_debug("%c", character);
	//}
}

int test(void)
{
	// when you find the %, run a parser to extract only the format specifier pattern.
	parse("-3.2f");
	return 0;
}
