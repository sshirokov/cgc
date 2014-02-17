#include <stdio.h>

#include "gcmd.h"

#ifndef __GCODE_H
#define __GCODE_H

typedef struct s_gcode_parser {
	FILE *stream;
	char *path;
	size_t line;
} gparser_t;

gparser_t *alloc_gparser(char *path);
void free_gparser(gparser_t *parser);

gparser_t *gparser_init(gparser_t *parser, char *path);
void gparser_clear(gparser_t *parser);

char *gparser_next_line(gparser_t *parser);
gcmd_t *gparser_next_cmd(gparser_t *parser);

#endif
