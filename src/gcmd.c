#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "dbg.h"

#include "util.h"
#include "gcmd.h"

// static gcmd_utils
static char *gcmd_filtered_raw(gcmd_t *cmd) {
	char *filtered = NULL;
	char *cur = NULL;

	check_mem(cur = filtered = str_trim(cmd->raw, true));

	while(cur < (filtered + strlen(filtered))) {
		// Cut the line short at the first comment char
		if((*cur == ';') || (*cur == '(')) {
			*cur = '\0';
			break;
		}
		cur++;
	}

	// Clean up any space that may have preceded a comment
	return 	str_trim(filtered, false);
error:
	if(filtered != NULL) free(filtered);
	return NULL;
}

static int gcmd_store_comment(gcmd_t *cmd) {
	char *comment = NULL;
	check(cmd->comment == NULL, "gcmd(%p)'%s':%zd already has comment.", cmd, cmd->path, cmd->line);

	comment = strchr(cmd->raw, '(');
	comment = comment == NULL ? strchr(cmd->raw, ';') : comment;

	if(comment != NULL) {
		cmd->comment = str_dup(comment);
		check_mem(cmd->comment);
		str_trim(cmd->comment, false);
	}

	return 0;
error:
	return -1;
}

static int gcmd_parse_filtered(gcmd_t *cmd, char *filtered) {
	cmd->op = filtered[0];

	sentinel("TODO: Store the args to the op (e.g. G71 => .op = 'G', .arg = 71)");
	sentinel("TODO: Parse the rest of the args into K=V pairs and store them in cmd");

	return 0;
error:
	return -1;
}

// gcmd_t methods
gcmd_t *alloc_gcmd(char *path, char *raw, size_t line) {
	gcmd_t *cmd = calloc(1, sizeof(gcmd_t));
	check_mem(cmd);

	check_debug(gcmd_init(cmd, path, raw, line) != NULL,
				"Failed to init fresh gcmd_t(%p)(%s, %s:%zd)", cmd, raw, path, line);

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

gcmd_t *gcmd_init(gcmd_t *cmd, char *path, char *raw, size_t line) {
	if(cmd == NULL) return alloc_gcmd(path, raw, line);
	gcmd_clear(cmd);

	cmd->line = line;
	if((path != NULL) && strlen(path) > 0) {
		cmd->path = str_dup(path);
		check_mem(cmd->path);
	}
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
	if(cmd->path != NULL) {
		free(cmd->path);
		cmd->path = NULL;
	}
	if(cmd->raw != NULL) {
		free(cmd->raw);
		cmd->raw = NULL;
	}
	if(cmd->comment != NULL) {
		free(cmd->comment);
		cmd->comment = NULL;
	}
}

int gcmd_parse(gcmd_t *cmd) {
	// Only parse a command if we don't have a known opcode
	if(cmd->op != GCMD_NA) return 0;

	int rc = -1;
	char *filtered = NULL;
	check_mem(filtered = gcmd_filtered_raw(cmd));
	check(gcmd_store_comment(cmd) == 0, "Error storing comment for command '%s':%zd", cmd->path, cmd->line);

	// Only parse comands that filter to something actionable
	// The rest can return as NOOPs
	if(strlen(filtered) > 0) {
		rc = gcmd_parse_filtered(cmd, filtered);
		check(rc == 0,
			  "Failed to parse filtered command [%s] from '%s':%zd",
			  filtered, cmd->path, cmd->line);
	}
	else {
		cmd->op = GCMD_NOOP;
	}

	// We no longer need the filtered line, all the information
	// is now held in `cmd`
	free(filtered);
	return 0;
error:
	if(filtered != NULL) free(filtered);
	return -1;
}

bool gcmd_is_noop(gcmd_t *cmd) {
	return cmd->op == GCMD_NOOP;
}
