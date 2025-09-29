//
// test_trim.c
//
// Unit tests for trim functions (trim_left, trim_right, trim)
// Testing library/framework used: Standard C 'assert' with a minimal custom harness.
// No external C testing framework was detected in the repository.
//
// Focus: Thorough coverage of core behaviors, edge cases, and failure-like conditions
// (e.g., all-whitespace and empty strings). Tests validate in-place mutation, return
// pointer stability, and correct null-terminator placement.
//
// Copyright
// MIT licensed
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "trim.h"

// ---------- Minimal test harness ----------
#define TEST(name) static void test_##name(void)
static int tests_run = 0;
static int tests_passed = 0;

#define RUN_TEST(name) do { \
  ++tests_run; \
  printf("Running %-40s ... ", "test_" #name); \
  fflush(stdout); \
  test_##name(); \
  ++tests_passed; \
  printf("PASSED\n"); \
} while (0)

#define ASSERT_STR_EQ(expected, actual) do { \
  if (strcmp((expected), (actual)) \!= 0) { \
    fprintf(stderr, "\nAssertion failed: strings differ\n  expected: \"%s\"\n  actual:   \"%s\"\n", (expected), (actual)); \
    exit(1); \
  } \
} while (0)

// Helper: heap copy to get a modifiable C string
static char* make_string(const char* s) {
  if (\!s) return NULL;
  size_t n = strlen(s);
  char* d = (char*)malloc(n + 1);
  if (\!d) {
    fprintf(stderr, "malloc failed\n");
    exit(1);
  }
  memcpy(d, s, n + 1);
  return d;
}

// ---------- trim_left tests ----------
TEST(trim_left_no_whitespace) {
  char* s = make_string("hello");
  char* r = trim_left(s);
  ASSERT_STR_EQ("hello", r);
  ASSERT_STR_EQ("hello", s);
  free(s);
}

TEST(trim_left_leading_spaces) {
  char* s = make_string("   hello");
  char* r = trim_left(s);
  ASSERT_STR_EQ("hello", r);
  free(s);
}

TEST(trim_left_leading_tabs) {
  char* s = make_string("\t\thello");
  char* r = trim_left(s);
  ASSERT_STR_EQ("hello", r);
  free(s);
}

TEST(trim_left_mixed_whitespace) {
  char* s = make_string(" \t\n\r hello");
  char* r = trim_left(s);
  ASSERT_STR_EQ("hello", r);
  free(s);
}

TEST(trim_left_only_whitespace) {
  char* s = make_string("   \t\n");
  char* r = trim_left(s);
  ASSERT_STR_EQ("", r);
  free(s);
}

TEST(trim_left_empty) {
  char* s = make_string("");
  char* r = trim_left(s);
  ASSERT_STR_EQ("", r);
  free(s);
}

TEST(trim_left_single_char) {
  char* s = make_string("a");
  char* r = trim_left(s);
  ASSERT_STR_EQ("a", r);
  free(s);
}

TEST(trim_left_single_space) {
  char* s = make_string(" ");
  char* r = trim_left(s);
  ASSERT_STR_EQ("", r);
  free(s);
}

TEST(trim_left_preserves_trailing_ws) {
  char* s = make_string("  hello  ");
  char* r = trim_left(s);
  ASSERT_STR_EQ("hello  ", r);
  free(s);
}

TEST(trim_left_returns_same_pointer) {
  char* s = make_string("   hello");
  char* p = s;
  char* r = trim_left(s);
  assert(r == p);
  free(s);
}

// ---------- trim_right tests ----------
TEST(trim_right_no_whitespace) {
  char* s = make_string("hello");
  char* r = trim_right(s);
  ASSERT_STR_EQ("hello", r);
  free(s);
}

TEST(trim_right_trailing_spaces) {
  char* s = make_string("hello   ");
  char* r = trim_right(s);
  ASSERT_STR_EQ("hello", r);
  free(s);
}

TEST(trim_right_trailing_tabs) {
  char* s = make_string("hello\t\t");
  char* r = trim_right(s);
  ASSERT_STR_EQ("hello", r);
  free(s);
}

TEST(trim_right_mixed_whitespace) {
  char* s = make_string("hello \t\n\r");
  char* r = trim_right(s);
  ASSERT_STR_EQ("hello", r);
  free(s);
}

TEST(trim_right_only_whitespace) {
  char* s = make_string("   \t\n");
  char* r = trim_right(s);
  ASSERT_STR_EQ("", r);
  free(s);
}

// NOTE: The implementation of trim_right does not explicitly guard empty strings.
// This test asserts expected behavior (result should remain empty). If this crashes,
// it likely reveals a bug (reading before buffer start).
TEST(trim_right_empty) {
  char* s = make_string("");
  char* r = trim_right(s);
  ASSERT_STR_EQ("", r);
  free(s);
}

TEST(trim_right_single_char) {
  char* s = make_string("a");
  char* r = trim_right(s);
  ASSERT_STR_EQ("a", r);
  free(s);
}

TEST(trim_right_single_space) {
  char* s = make_string(" ");
  char* r = trim_right(s);
  ASSERT_STR_EQ("", r);
  free(s);
}

TEST(trim_right_preserves_leading_ws) {
  char* s = make_string("  hello  ");
  char* r = trim_right(s);
  ASSERT_STR_EQ("  hello", r);
  free(s);
}

TEST(trim_right_all_ws_flavors) {
  char* s = make_string("hello \t\n\r\v\f");
  char* r = trim_right(s);
  ASSERT_STR_EQ("hello", r);
  free(s);
}

TEST(trim_right_returns_same_pointer) {
  char* s = make_string("hello  ");
  char* p = s;
  char* r = trim_right(s);
  assert(r == p);
  free(s);
}

TEST(trim_right_null_terminator_placement) {
  char buf[20];
  strcpy(buf, "hello     ");
  trim_right(buf);
  assert(strlen(buf) == 5);
  ASSERT_STR_EQ("hello", buf);
}

// ---------- trim (both sides) tests ----------
TEST(trim_no_whitespace) {
  char* s = make_string("hello");
  char* r = trim(s);
  ASSERT_STR_EQ("hello", r);
  free(s);
}

TEST(trim_both_sides_spaces) {
  char* s = make_string("  hello  ");
  char* r = trim(s);
  ASSERT_STR_EQ("hello", r);
  free(s);
}

TEST(trim_only_leading) {
  char* s = make_string("   hello");
  char* r = trim(s);
  ASSERT_STR_EQ("hello", r);
  free(s);
}

TEST(trim_only_trailing) {
  char* s = make_string("hello   ");
  char* r = trim(s);
  ASSERT_STR_EQ("hello", r);
  free(s);
}

TEST(trim_mixed_ws_both_sides) {
  char* s = make_string(" \t\nhello\r\n ");
  char* r = trim(s);
  ASSERT_STR_EQ("hello", r);
  free(s);
}

TEST(trim_only_whitespace) {
  char* s = make_string("   \t\n  ");
  char* r = trim(s);
  ASSERT_STR_EQ("", r);
  free(s);
}

TEST(trim_empty) {
  char* s = make_string("");
  char* r = trim(s);
  ASSERT_STR_EQ("", r);
  free(s);
}

TEST(trim_preserves_internal_ws) {
  char* s = make_string("  hello world  ");
  char* r = trim(s);
  ASSERT_STR_EQ("hello world", r);
  free(s);
}

TEST(trim_multiple_words_and_gaps) {
  char* s = make_string("  hello   world   test  ");
  char* r = trim(s);
  ASSERT_STR_EQ("hello   world   test", r);
  free(s);
}

TEST(trim_various_ascii_ws) {
  char* s = make_string("\f\v\r\n\t hello \t\n\r\v\f");
  char* r = trim(s);
  ASSERT_STR_EQ("hello", r);
  free(s);
}

TEST(trim_very_long_string) {
  // 1000 chars: 500 leading spaces + "hello" + 495 trailing spaces
  char* s = (char*)malloc(1001);
  if (\!s) { fprintf(stderr, "malloc failed\n"); exit(1); }
  memset(s, ' ', 500);
  memcpy(s + 500, "hello", 5);
  memset(s + 505, ' ', 495);
  s[1000] = '\0';
  char* r = trim(s);
  ASSERT_STR_EQ("hello", r);
  free(s);
}

TEST(trim_single_char_with_ws) {
  char* s = make_string("  a  ");
  char* r = trim(s);
  ASSERT_STR_EQ("a", r);
  free(s);
}

TEST(trim_returns_same_pointer) {
  char* s = make_string("  hello  ");
  char* p = s;
  char* r = trim(s);
  assert(r == p);
  free(s);
}

TEST(trim_in_place_modification) {
  char* s = make_string("  hello  ");
  trim(s);
  ASSERT_STR_EQ("hello", s);
  free(s);
}

// ---------- main ----------
int main(void) {
  printf("=== trim functions unit tests ===\n");

  // trim_left
  RUN_TEST(trim_left_no_whitespace);
  RUN_TEST(trim_left_leading_spaces);
  RUN_TEST(trim_left_leading_tabs);
  RUN_TEST(trim_left_mixed_whitespace);
  RUN_TEST(trim_left_only_whitespace);
  RUN_TEST(trim_left_empty);
  RUN_TEST(trim_left_single_char);
  RUN_TEST(trim_left_single_space);
  RUN_TEST(trim_left_preserves_trailing_ws);
  RUN_TEST(trim_left_returns_same_pointer);

  // trim_right
  RUN_TEST(trim_right_no_whitespace);
  RUN_TEST(trim_right_trailing_spaces);
  RUN_TEST(trim_right_trailing_tabs);
  RUN_TEST(trim_right_mixed_whitespace);
  RUN_TEST(trim_right_only_whitespace);
  RUN_TEST(trim_right_empty); // may reveal bug if implementation mishandles empty strings
  RUN_TEST(trim_right_single_char);
  RUN_TEST(trim_right_single_space);
  RUN_TEST(trim_right_preserves_leading_ws);
  RUN_TEST(trim_right_all_ws_flavors);
  RUN_TEST(trim_right_returns_same_pointer);
  RUN_TEST(trim_right_null_terminator_placement);

  // trim (both)
  RUN_TEST(trim_no_whitespace);
  RUN_TEST(trim_both_sides_spaces);
  RUN_TEST(trim_only_leading);
  RUN_TEST(trim_only_trailing);
  RUN_TEST(trim_mixed_ws_both_sides);
  RUN_TEST(trim_only_whitespace);
  RUN_TEST(trim_empty);
  RUN_TEST(trim_preserves_internal_ws);
  RUN_TEST(trim_multiple_words_and_gaps);
  RUN_TEST(trim_various_ascii_ws);
  RUN_TEST(trim_very_long_string);
  RUN_TEST(trim_single_char_with_ws);
  RUN_TEST(trim_returns_same_pointer);
  RUN_TEST(trim_in_place_modification);

  printf("\n=== Test Summary ===\n");
  printf("Passed %d/%d tests\n", tests_passed, tests_run);
  return tests_run == tests_passed ? 0 : 1;
}