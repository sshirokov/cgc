#include <stdlib.h>

#ifndef __GCMD_H
#define __GCMD_H

#define GCMD_NOP '\0'

typedef struct s_gcode_cmd {
	size_t line;
	char *raw;

	char op;
} gcmd_t;

gcmd_t *alloc_gcmd(char *raw, size_t line);
void free_gcmd(gcmd_t *cmd);

gcmd_t *gcmd_init(gcmd_t *cmd, char *raw, size_t line);
void gcmd_clear(gcmd_t *cmd);

#endif
