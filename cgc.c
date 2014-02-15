#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "dbg.h"

#include "gcode.h"

void usage(char *argv0) {
	fprintf(stderr, "Usage: %s [options] input\n\n", argv0);
	fprintf(stderr, "\t-" "h" "\t" "Display help" "\n");
	fprintf(stderr, "\n");
}

int usage_exit(char *argv0, int status) {
	usage(argv0);
	exit(status);
	return status;
}

int usage_fail(char *argv0, char *error, ...) {
	va_list args;
	va_start(args, error);
	fprintf(stderr, "Error: ");
	vfprintf(stderr, error, args);
	fprintf(stderr, "\n");
	va_end(args);

	return usage_exit(argv0, EXIT_FAILURE);
}

int main(int argc, char **argv) {
	char *input = NULL;

	// If we're blindly invoked, just print usage as a failure
	if(argc < 2) return usage_exit(argv[0], EXIT_FAILURE);

	// Parse CLI args
	char *arg = argv[1];
	for(int i = 1; i < argc; arg = argv[++i]) {
		if((strlen(arg) > 1) && (arg[0] == '-')) {
			// Potential chain of many options with one '-'
			for(char o = *(++arg); o != '\0'; o = *(++arg)) {
				switch(o) {
				case 'h':
					usage_exit(argv[0], EXIT_SUCCESS);
					break;
				case 'v':
					log_err("TODO: Implement verbosity");
					break;
				default:
					usage_fail(argv[0], "Unknown option '%c'", o);
					break;
				}
			}
		}
		else {
			// Input file
			if(input != NULL) usage_fail(argv[0], "Input already specified as '%s', cannot specify '%s'", input, arg);
			input = arg;

		}
	}
	if(input == NULL) return usage_fail(argv[0], "No input file specified.");

	// TODO: Perform the request actions
	gparser_t *parser = alloc_gparser(input);
	check(parser != NULL, "Failed to initialize parser for '%s'", input);

	free_gparser(parser);
	return EXIT_SUCCESS;
error:
	if(parser != NULL) free_gparser(parser);
	return EXIT_FAILURE;
}
