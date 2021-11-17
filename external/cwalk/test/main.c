#include "tests.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * This is just a small macro which calculates the size of an array.
 */
#define CWK_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

struct cwk_test
{
  const char *unit_name;
  const char *test_name;
  const char *full_name;
  int (*fn)(void);
};

#define XX(u, t) extern int u##_##t(void);
UNIT_TESTS(XX)
#undef XX

static struct cwk_test tests[] = {
#define XX(u, t)                                                               \
  {.unit_name = #u, .test_name = #t, .full_name = #u "/" #t, .fn = u##_##t},
  UNIT_TESTS(XX)
#undef XX
};

static int call_test(struct cwk_test *test)
{
  size_t i;

  printf(" Running '%s' ", test->full_name);
  for (i = strlen(test->full_name); i < 45; ++i) {
    fputs(".", stdout);
  }

  if (test->fn() == EXIT_FAILURE) {
    fputs(" FAILURE\n", stdout);
    return EXIT_FAILURE;
  }

  fputs(" SUCCESS\n", stdout);
  return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
  size_t i, count, failed, succeeded;
  const char *requested_unit_name, *requested_test_name;
  struct cwk_test *test;
  double rate;

  count = 0;
  failed = 0;
  if (argc < 2) {
    fputs("No unit specified. Running all tests.\n\n", stdout);
    for (i = 0; i < CWK_ARRAY_SIZE(tests); ++i) {
      test = &tests[i];
      ++count;
      if (call_test(test) == EXIT_FAILURE) {
        ++failed;
      }
    }
  } else if (argc < 3) {
    requested_unit_name = argv[1];
    printf("Running all unit tests of '%s'.\n\n", requested_unit_name);
    for (i = 0; i < CWK_ARRAY_SIZE(tests); ++i) {
      test = &tests[i];
      if (strcmp(test->unit_name, requested_unit_name) == 0) {
        ++count;
        if (call_test(test) == EXIT_FAILURE) {
          ++failed;
        }
      }
    }
  } else {
    requested_unit_name = argv[1];
    requested_test_name = argv[2];
    printf("Running a single test '%s/%s'.\n\n", requested_unit_name,
      requested_test_name);
    for (i = 0; i < CWK_ARRAY_SIZE(tests); ++i) {
      test = &tests[i];
      if (strcmp(test->unit_name, requested_unit_name) == 0 &&
          strcmp(test->test_name, requested_test_name) == 0) {
        ++count;
        if (call_test(test) == EXIT_FAILURE) {
          ++failed;
        }
      }
    }
  }

  if (count == 1) {
    fputs("\nThe test has been executed.\n", stdout);
  } else if (count > 0) {
    succeeded = count - failed;
    rate = (double)succeeded / (double)count * 100;
    printf("\n%zu/%zu (%.2f%%) of those tests succeeded.\n", succeeded, count,
      rate);
  } else {
    printf("\nNo tests found.\n");
    return EXIT_FAILURE;
  }

  if (failed > 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
