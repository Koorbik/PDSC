#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

bool isNegative(char** endPtr) {
	
	if (**endPtr == '-') {
		(*endPtr)++;
		return true;
	}
	else if (**endPtr == '+') {
		(*endPtr)++;
	}
	return false;
}

int getCharValue (int character, int base) {
	int charValue = -1;
	if(isdigit(character)) {
		charValue = character - '0';
	}
	else if(isalpha(character) && (base > 10)) {
		
		if (islower(character)) {
			charValue = character -'a' + 10; 
		}
		else {
			charValue = character - 'A' + 10;
		}
	}

	if(charValue < base) {
		return charValue;
	}
	return -1;
}

int autoDetectBase(char **endPtr, int base) {
    
	int charValue = getCharValue(**endPtr, 10);
	
	if (charValue > 0) {
		return 10;
	}

	else if (charValue == 0) {
		
		(*endPtr)++;
		if (**endPtr == 'x' || **endPtr == 'X') {
			(*endPtr)++;
			return 16;
		}
		return 8;
	}

	return -1;
}

void discardSpaces(char** endPtr) {
	while(isspace(**endPtr)) {
		(*endPtr)++;
	}
}

void handleOverflow(long *num, bool negative) {
	if((*num >> 63 == -1) && !negative) {
		errno = ERANGE;
		*num = LONG_MAX;
	}
	
	if (negative && *num && (*num >> 63 == 0)) {
    errno = ERANGE;
    *num = LONG_MIN;
	}
}

bool handleWrongArgument(char **endPtr, int base) {
	if (!endPtr) {
		return false;
	}
	if (((base < 2) || (base > 36)) && (base != 0)) {
		errno = EINVAL;
		return false;
	}
	return true;
}

long convertString(char **endPtr, int base, bool negative) {
	long result = 0;
	int charVal;

	while((charVal=getCharValue(**endPtr, base)) >= 0) {
		(*endPtr)++;
		result = result*base;
		result += (negative ? -charVal : charVal);
	}

	return result;
}

void movePtrToLastValidChar(const char *nPtr, char **endPtr, int base, long result) {
	if ((getCharValue(*(*endPtr - 1), base) < 0) && (result == 0)) {
		
		while ((getCharValue(*(*endPtr-1), base) < 0) && (*endPtr != nPtr)) {
			(*endPtr)--;
		}
	}
} 

long strtol(const char* nPtr, char** endPtr, int base)
{
	
	if (!handleWrongArgument(endPtr, base)) {
		return 0;
	}

	discardSpaces(endPtr);
	bool negative = isNegative(endPtr);
	int autoDetectedBase = autoDetectBase(endPtr, base);
	if (base == 0) {
		base = autoDetectedBase;
	}

	long result = convertString(endPtr, base, negative);
	handleOverflow(&result, negative);

	movePtrToLastValidChar(nPtr, endPtr, base, result);
	return result;
} 
