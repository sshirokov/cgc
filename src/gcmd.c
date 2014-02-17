#include <stdlib.h>
#include <string.h>

#include "dbg.h"

#include "gcmd.h"

gcmd_t *alloc_gcmd(char *raw, size_t line) {
	gcmd_t *cmd = calloc(1, sizeof(gcmd_t));
	check_mem(cmd);

	check_debug(gcmd_init(cmd, raw, line) != NULL,
		  "Failed to init fresh gcmd_t(%p)(%s, %zd)", cmd, raw, line);

	return cmd;
error:
	free_gcmd(cmd);
	return NULL;
}

void free_gcmd(gcmd_t *cmd) {
	if(cmd == NULL) return;
	gcmd_clear(cmd);
	free(cmd);
}

gcmd_t *gcmd_init(gcmd_t *cmd, char *raw, size_t line) {
	if(cmd == NULL) return alloc_gcmd(raw, line);
	gcmd_clear(cmd);

	cmd->line = line;
	if((raw != NULL) && (strlen(raw) > 0)) {
		cmd->raw = calloc(strlen(raw) + 1, sizeof(char));
		check_mem(cmd->raw);
		strncpy(cmd->raw, raw, strlen(raw));

		// If we have a line, parse the line
		check(gcmd_parse(cmd) == 0, "Failed to parse line %zd", line);
	}

	return cmd;
error:
	return NULL;
}

void gcmd_clear(gcmd_t *cmd) {
	if(cmd == NULL) return;
	cmd->line = 0;
	if(cmd->raw != NULL) {
		free(cmd->raw);
		cmd->raw = NULL;
	}
}

int gcmd_parse(gcmd_t *cmd) {
	check(cmd->raw != NULL, "No line in gcmd(%p) to parse", cmd);
	check(strlen(cmd->raw) > 0, "Empty buffer in gcmd(%p)", cmd);

	// Only parse a command if we don't have a known opcode
	if(cmd->op != GCMD_NA) return 0;

	debug("Parse command: [%s]", cmd->raw);

	return 0;
error:
	return -1;
}
