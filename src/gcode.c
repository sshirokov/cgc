#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dbg.h"

#include "gcode.h"
#include "gcmd.h"

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

char *gparser_next_line(gparser_t *parser) {
	char read_buffer[512] = {0};
	char *line = NULL;
	char *rc = NULL;

	// Sanity check the stream before we go on,
	check_debug(feof(parser->stream) == 0, "parser(%p)[%s]:%zd is at EOF", parser, parser->path, parser->line);
	check(ferror(parser->stream) == 0, "Error in parser(%p)[%s]:%zd stream.", parser, parser->path, parser->line);

	// Advance the line first
	parser->line++;

	// Try reading, with the hope that we get the entire line at once
	rc = fgets(read_buffer, sizeof(read_buffer), parser->stream);
	check(rc != NULL, "Failed to read line from '%s':%zd", parser->path, parser->line);
	check_mem(line = calloc(strlen(read_buffer) + 1, sizeof(char)));
	strncpy(line, read_buffer, strlen(read_buffer));

	// See if we need to finish reading the line
	while(line[strlen(line) - 1] != '\n') {
		rc = fgets(read_buffer, sizeof(read_buffer), parser->stream);
		if((rc == NULL) && feof(parser->stream)) {
			// We got everything that we can get, so we'll
			// call it a "line"
			break;
		}
		else {
			// Append the new data to the end of the line
			char *new_line = NULL;
			check(rc != NULL, "Error finishing line %s:%zd", parser->path, parser->line);
			check_mem(new_line = calloc(strlen(line) + strlen(read_buffer) + 1, sizeof(char)));

			strncpy(new_line, line, strlen(line));
			strncpy(new_line + strlen(new_line), read_buffer, strlen(read_buffer));

			free(line);
			line = new_line;
		}
	}

	return line;
error:
	if(line != NULL) free(line);
	if(feof(parser->stream)) debug("parser(%p)[%s]:%zd: EOF", parser, parser->path, parser->line);
	if(ferror(parser->stream)) debug("parser(%p)[%s]:%zd: ERROR. %s", parser, parser->path, parser->line, clean_errno());
	return NULL;
}

gcmd_t *gparser_next_cmd(gparser_t *parser) {
	gcmd_t *cmd = NULL;
	char *line = NULL;

	while((cmd == NULL) && ((line = gparser_next_line(parser)) != NULL)) {
		cmd = alloc_gcmd(parser->path, line, parser->line);
		check(cmd != NULL, "Failed to parse line '%s':%zd", parser->path, parser->line);
		free(line);

		// Don't return NOOPs, wait until we have a command
		if(gcmd_is_noop(cmd)) {
			free_gcmd(cmd);
			cmd = NULL;
		}
	}

	return cmd;
error:
	if(line != NULL) free(line);
	if(cmd != NULL) free_gcmd(cmd);
	return NULL;
}
