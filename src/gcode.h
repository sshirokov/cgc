#include <stdio.h>

#ifndef __GCODE_H
#define __GCODE_H

typedef struct s_gcode_cmd {
	size_t line;
	char *raw;
} gcmd_t;

typedef struct s_gcode_parser {
	FILE *stream;
	char *path;
} gparser_t;

gparser_t *alloc_gparser(char *path);
void free_gparser(gparser_t *parser);

void gparser_clear(gparser_t *parser);
gparser_t *gparser_init(gparser_t *parser, char *path);

#endif