#include <ctype.h>
#include <string.h>

#include <stdio.h>

#include "clar.h"

#include "util.h"

char *no_need = "nothing required";
char *need_trim  = " text goes here \n";
char *just_spaces = " \n";

char *nonstatic = NULL;
char *no_need_copy = NULL;
char *need_trim_copy = NULL;
char *just_spaces_copy = NULL;


void test_util__initialize(void) {
	need_trim_copy = calloc(strlen(need_trim) + 1, sizeof(char));
	cl_assert_(need_trim_copy != NULL, "Failed to allocate memory in initializer.");

	just_spaces_copy = calloc(strlen(just_spaces) + 1, sizeof(char));
	cl_assert_(just_spaces_copy != NULL, "Failed to allocate memory in initializer.");

	no_need_copy = calloc(strlen(no_need) + 1, sizeof(char));
	cl_assert_(no_need_copy != NULL, "Failed to allocate memory in initializer.");

	strncpy(need_trim_copy, need_trim, strlen(need_trim));
	strncpy(just_spaces_copy, just_spaces, strlen(just_spaces));
	strncpy(no_need_copy, no_need, strlen(no_need));
}

void test_util__cleanup(void) {
	if(nonstatic != NULL) free(nonstatic);
	if(need_trim_copy != NULL) free(need_trim_copy);
	if(just_spaces_copy != NULL) free(just_spaces_copy);
	if(no_need_copy != NULL) free(no_need_copy);

	nonstatic = NULL;
	need_trim_copy = NULL;
	just_spaces_copy = NULL;
	no_need_copy = NULL;
}

void test_util__ltrim_without_copy(void) {
	size_t before = strlen(need_trim_copy);
	cl_assert_(str_ltrim(need_trim_copy, false) == need_trim_copy,
			   "str_ltrim(.., false) should return identity without a copy");

	cl_assert_(strlen(need_trim_copy) > 0,
			   "String should still have chars in it after an ltrim");

	cl_assert_(strlen(need_trim_copy) < before,
			   "String should appear smaller after ltrim.");

	cl_assert_(isalpha(need_trim_copy[0]) && !isspace(need_trim_copy[0]),
			   "String should not begin with spaces.");
	cl_assert_(isspace(need_trim_copy[strlen(need_trim_copy) - 1]),
			   "String should still end with a space after an ltrim");
}

void test_util__ltrim_handles_reducing_string_to_zero(void) {
	cl_assert(strlen(just_spaces_copy) > 0);
	cl_assert(str_ltrim(just_spaces_copy, false) == just_spaces_copy);
	cl_assert_equal_i_(strlen(just_spaces_copy), 0,
					   "An string of spaces should be reduced to nothing by str_ltrim");
}

void test_util__ltrim_with_copy(void) {
	nonstatic = str_ltrim(need_trim_copy, true);

	cl_assert_(nonstatic != NULL, "str_ltrim(.., true)  should succeed.");
	cl_assert_(nonstatic != need_trim_copy, "str_ltrim(..., true) should not return identity.");

	cl_assert_(strlen(nonstatic) > 0, "The return value should be a string of length.");
	cl_assert_(!isspace(nonstatic[0]), "The return value should not start with a space.");
	cl_assert(strlen(nonstatic) < strlen(need_trim_copy));

	cl_assert_(strncmp(need_trim_copy, need_trim, strlen(need_trim)) == 0,
			   "The original string should not have been modified.");
}

void test_util__ltrim_can_noop(void) {
	nonstatic = str_ltrim(no_need_copy, true);

	cl_assert(strlen(nonstatic) > 0);
	cl_assert_equal_i(strlen(nonstatic), strlen(no_need));
	cl_assert_(strncmp(no_need_copy, nonstatic, strlen(no_need)) == 0,
			   "Can't trim what doesn't need trimming.");
}
