#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "dbg.h"

#include "util.h"

char *str_dup(char *str) {
	char *copy_str = NULL;
	check_mem(copy_str = calloc(strlen(str) + 1, sizeof(char)));
	strncpy(copy_str, str, strlen(str));
	return copy_str;
error:
	return NULL;
}

char *str_ltrim(char *str, bool copy) {
	if(!copy) {
		// Find the first non-space char in the string
		char *start = str;
		while(*start && isspace(*start)) {
			start++;
		}

		// Do nothing if the string starts on a sane char
		if(start != str) {
			// Shift the string and NULL-cap it if we found a subset
			// otherwise NULL the head of the string
			if((start - str) != strlen(str)) {
				memmove(str, start, strlen(start));
				str[strlen(start)] = '\0';
			}
			else {
				str[0] = '\0';
			}
		}
		return str;
	}
	else {
		char *copy_str = NULL;
		check_mem(copy_str = str_dup(str));
		return str_ltrim(copy_str, false);
	}
error:
	return NULL;
}

char *str_rtrim(char *str, bool copy) {
	if(!copy) {
		char *end = str + strlen(str) - 1;
		while((end > str) && isspace(*end)) {
			*end-- = '\0';
		}
		return str;
	}
	else {
		char *copy_str = NULL;
		check_mem(copy_str = str_dup(str));
		return str_ltrim(copy_str, false);
	}
error:
	return NULL;
}

char *str_trim(char *str, bool copy) {
	char *trim_str = copy ? str_dup(str) : str;
	check_mem(trim_str);

	// Since we have already made a copy if we need one,
	// we can just chain these two together, since non-copy
	// operations can't throw out a NULL and the pointer itself
	// cannot change.
	return str_ltrim(str_rtrim(trim_str, false), false);
error:
	return NULL;
}
