#include <stdbool.h>

#ifndef __UTIL_H
#define __UTIL_H

char *str_dup(char *str);

char *str_ltrim(char *str, bool copy);
char *str_rtrim(char *str, bool copy);
char *str_trim(char *str, bool copy);

#endif
