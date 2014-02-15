#include <stdio.h>
#include <stdlib.h>

#include "dbg.h"

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

int main(int argc, char **argv) {
	if(argc < 2) return usage_exit(argv[0], EXIT_FAILURE);
	char *input = NULL;

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
					sentinel("Unknown option '%c'", o);
				}
			}
		}
		else {
			// Input file
			check(input == NULL, "Input already specified as '%s', cannot specify '%s'", input, arg);
			input = arg;

		}
	}

	return EXIT_SUCCESS;
error:
	return EXIT_FAILURE;
}
