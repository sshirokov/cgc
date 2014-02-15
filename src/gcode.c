#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dbg.h"

#include "gcode.h"

gparser_t *alloc_gparser(char *path) {
	gparser_t *parser = calloc(1, sizeof(gparser_t));
	check_mem(parser);

	check(gparser_init(parser, path) == parser, "Failed initialize gparser_t(%p)", parser);

	return parser;
error:
	if(parser != NULL) free_gparser(parser);
	return NULL;
}

void free_gparser(gparser_t *parser) {
	if(parser == NULL) return;
	gparser_clear(parser);
	free(parser);
}

void gparser_clear(gparser_t *parser) {
	if(parser != NULL) return;

	if(parser->path != NULL) {
		free(parser->path);
		parser->path = NULL;
	}

	if(parser->stream != NULL) {
		if(parser->stream != stdin) {
			// We don't actually care if there's an error closing
			fclose(parser->stream);
		}
		parser->stream = NULL;
	}
}

gparser_t *gparser_init(gparser_t *parser, char *path) {
	if(parser == NULL) return alloc_gparser(path);
	gparser_clear(parser);

	if(path != NULL) {
		size_t len = strlen(path);
		parser->path = calloc(len + 1, sizeof(char));
		check_mem(parser->path);
		strncpy(parser->path, path, len);
	}

	if(parser->path != NULL) {
		// Either bind STDIN to the parser input or open
		// the named file
		if((strlen(parser->path) == 1) && (parser->path[0] == '-')) {
			parser->stream = stdin;
		}
		else {
			parser->stream = fopen(parser->path, "r");
			check(parser->stream != NULL, "Failed to open '%s' for reading.", parser->path);
		}
	}

	return parser;
error:
	if(parser != NULL) gparser_clear(parser);
	return NULL;
}
