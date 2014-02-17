#include <string.h>

#include "clar.h"

#include "gcode.h"

char *shark_gcode = CLAR_FIXTURE_PATH "jaws.gcode";
gparser_t *parser = NULL;

void test_parser__initialize(void) {
	parser = alloc_gparser(shark_gcode);
	cl_assert_(parser != NULL, "Failed to make parser.");
}

void test_parser__cleanup(void) {
	free_gparser(parser);
	parser = NULL;
}

void test_parser__can_read_line(void) {
	char *line = gparser_next_line(parser);
	cl_assert_(line != NULL, "Parser should have read a line.");
	cl_assert_(strlen(line) > 0, "The line should not be empty.");
	free(line);
}

void test_parser__updates_line_count(void) {
	size_t count = 0;

	free(gparser_next_line(parser));
	count = parser->line;
	cl_assert_equal_i(count, 1);

	free(gparser_next_line(parser));
	cl_assert_(parser->line > count, "Line count should have incremented.");
	cl_assert_equal_i(parser->line, count + 1);
}
