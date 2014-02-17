#include <stdbool.h>
#include <stdlib.h>

#ifndef __GCMD_H
#define __GCMD_H

#define GCMD_NA '\0'
#define GCMD_NOOP (char)(-1)

typedef struct s_gcode_cmd {
	char *path;
	size_t line;
	char *raw;

	char op;
} gcmd_t;

gcmd_t *alloc_gcmd(char *path, char *raw, size_t line);
void free_gcmd(gcmd_t *cmd);

gcmd_t *gcmd_init(gcmd_t *cmd, char *path, char *raw, size_t line);
void gcmd_clear(gcmd_t *cmd);

int gcmd_parse(gcmd_t *cmd);

bool gcmd_is_noop(gcmd_t *cmd);

#endif
