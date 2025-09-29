//
// tests/test_substr.c
//
// Thorough unit tests for substr(const char *str, int start, int end)
//
// Testing library/framework: Standard C <assert.h> with a minimal custom runner.
// Rationale: No external C testing framework detected in the repository.
// These tests focus on the behaviors introduced in the current substr.c diff.
//

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../__src__code__/clib-masterllvm-15/deps/substr/substr.h"

static void expect_substr_eq(const char *input, int start, int end, const char *expected) {
  char *out = substr(input, start, end);
  assert(out \!= NULL);
  assert(strcmp(out, expected) == 0);
  free(out);
}

static void expect_substr_is_empty(const char *input, int start, int end) {
  char *out = substr(input, start, end);
  assert(out \!= NULL);
  assert(out[0] == '\0');
  assert(strlen(out) == 0);
  free(out);
}

static void expect_substr_null(const char *input, int start, int end) {
  char *out = substr(input, start, end);
  assert(out == NULL);
}

static void run_test(const char *name, void (*fn)(void)) {
  printf("Running %-50s ... ", name);
  fflush(stdout);
  fn();
  puts("OK");
}

/* -------------------- Happy paths -------------------- */

static void test_basic_extractions(void) {
  expect_substr_eq("hello world", 0, 5, "hello");
  expect_substr_eq("hello world", 6, 11, "world");
  expect_substr_eq("abcde", 1, 4, "bcd");
}

static void test_end_minus_one_expands_to_len(void) {
  expect_substr_eq("hello", 0, -1, "hello");
  expect_substr_eq("hello", 2, -1, "llo");
  expect_substr_eq("abcdef", 3, -1, "def");
}

static void test_middle_and_tail_slices(void) {
  expect_substr_eq("programming", 3, 7, "gram");
  expect_substr_eq("abcdef", 2, 6, "cdef");
  expect_substr_eq("substring", 4, 8, "ring");
}

/* -------------------- Boundary and edge semantics -------------------- */

static void test_end_less_or_equal_start_returns_null(void) {
  expect_substr_null("hello", 0, 0);
  expect_substr_null("hello", 3, 3);
  expect_substr_null("hello", 4, 1);
}

static void test_negative_start_returns_null(void) {
  expect_substr_null("hello", -1, 3);
  expect_substr_null("hello", -5, 2);
  expect_substr_null("", -1, 0);
}

static void test_start_beyond_length_returns_null(void) {
  expect_substr_null("hi", 3, 4);
  expect_substr_null("test", 6, 10);
  expect_substr_null("hello", 10, 12);
}

static void test_len_plus_one_rule(void) {
  expect_substr_eq("hello", 0, 6, "hello");
  expect_substr_is_empty("hello", 5, 6);
  expect_substr_null("hello", 5, -1);
}

static void test_end_greater_than_len_plus_one_returns_null(void) {
  expect_substr_null("hello", 0, 7);
  expect_substr_null("hello", 2, 100);
  expect_substr_null("world", 0, 9);
}

/* -------------------- strdup path and allocation behavior -------------------- */

static void test_diff_equals_len_uses_strdup_path(void) {
  const char *s = "world";
  char *out = substr(s, 0, 5);  // diff == len triggers strdup path
  assert(out \!= NULL);
  assert(strcmp(out, s) == 0);
  assert(out \!= s);             // ensure a fresh allocation
  free(out);
}

/* -------------------- Special characters and bytes -------------------- */

static void test_special_characters(void) {
  expect_substr_eq("line1\nline2", 0, 5, "line1");
  expect_substr_eq("tab\there", 4, -1, "here");
  expect_substr_eq("quote\"test", 6, -1, "test");
  expect_substr_eq("backslash\\test", 9, -1, "test");
}

static void test_multibyte_utf8_bytes(void) {
  expect_substr_eq("café", 0, 3, "caf");       // byte slice, not character-aware
  expect_substr_eq("mañana", 1, 5, "\xC3\xB1an"); // demonstrates byte-level slicing
}

/* -------------------- Empty string semantics -------------------- */

static void test_empty_string_cases(void) {
  expect_substr_null("", 0, -1);
  expect_substr_null("", 0, 0);
  expect_substr_null("", 0, 1);
  expect_substr_null("", 1, 2);
}

/* -------------------- Larger inputs and chaining -------------------- */

static void test_large_input_slices(void) {
  char buf[1000];
  memset(buf, 'a', sizeof(buf) - 1);
  buf[sizeof(buf) - 1] = '\0';

  char *out = substr(buf, 0, 10);
  assert(out \!= NULL);
  assert(strlen(out) == 10);
  for (int i = 0; i < 10; i++) assert(out[i] == 'a');
  free(out);

  out = substr(buf, 500, 510);
  assert(out \!= NULL);
  assert(strlen(out) == 10);
  for (int i = 0; i < 10; i++) assert(out[i] == 'a');
  free(out);
}

static void test_chained_substrings(void) {
  char *first = substr("programming", 0, 7);
  assert(first \!= NULL);
  assert(strcmp(first, "program") == 0);

  char *second = substr(first, 0, 4);
  assert(second \!= NULL);
  assert(strcmp(second, "prog") == 0);

  free(second);
  free(first);
}

/* -------------------- Regression-oriented checks -------------------- */

static void test_off_by_one_regressions(void) {
  expect_substr_eq("hello", 0, 4, "hell");
  expect_substr_eq("hello", 1, 5, "ello");
  expect_substr_null("hello", 0, 6);  // beyond len + 1
  expect_substr_null("hello", 5, 7);  // len + 1 < end
}

int main(void) {
  puts("Starting substr unit tests (assert-based)...");

  run_test("basic_extractions",                 test_basic_extractions);
  run_test("end_minus_one_expands_to_len",      test_end_minus_one_expands_to_len);
  run_test("middle_and_tail_slices",            test_middle_and_tail_slices);

  run_test("end_leq_start_returns_null",        test_end_less_or_equal_start_returns_null);
  run_test("negative_start_returns_null",       test_negative_start_returns_null);
  run_test("start_beyond_length_returns_null",  test_start_beyond_length_returns_null);
  run_test("len_plus_one_rule",                 test_len_plus_one_rule);
  run_test("end_gt_len_plus_one_returns_null",  test_end_greater_than_len_plus_one_returns_null);

  run_test("diff_equals_len_strdup_path",       test_diff_equals_len_uses_strdup_path);

  run_test("special_characters",                test_special_characters);
  run_test("multibyte_utf8_bytes",              test_multibyte_utf8_bytes);

  run_test("empty_string_cases",                test_empty_string_cases);

  run_test("large_input_slices",                test_large_input_slices);
  run_test("chained_substrings",                test_chained_substrings);

  run_test("off_by_one_regressions",            test_off_by_one_regressions);

  puts("All substr tests passed.");
  return 0;
}