#include "tinyobj_internal_tests.h"
#include "tinyobj_api_tests.h"
#include "tinyobj_regression_tests.h"

#include "acutest.h"

TEST_LIST = {
    // API Tests
    { "tinyobj_attrib_init",    test_tinyobj_attrib_init },
    { "tinyobj_parse_mtl_file", test_tinyobj_parse_mtl_file },
    { "tinyobj_parse_obj",      test_tinyobj_parse_obj },

    // Implementation Tests
    { "skip_space",             test_skip_space },
    { "skip_space_and_cr",      test_skip_space_and_cr },
    { "until_space",            test_until_space },
    { "length_until_newline",   test_length_until_newline },
    { "my_atoi",                test_my_atoi },
    { "fix_index",              test_fix_index },
    { "parseRawTriple",         test_parseRawTriple },
    { "parseInt",               test_parseInt },
    { "tryParseDouble",         test_tryParseDouble },
    { "parseFloat",             test_parseFloat },
    { "parseFloat2",            test_parseFloat2 },
    { "parseFloat3",            test_parseFloat3 },
    { "my_strdup",              test_my_strdup },
    { "my_strndup",             test_my_strndup },
    { "initMaterial",           test_initMaterial },
    { "create_hash_table",      test_create_hash_table },
    { "hash_table_set",         test_hash_table_set },
    { "hash_table_exists",      test_hash_table_exists },
    { "hash_table_get",         test_hash_table_get },
    { "hash_table_maybe_grow",  test_hash_table_maybe_grow },

    { "crlf_string",  test_tinyobj_crlf_string },
    { "negative_exponent_issue26",  test_tinyobj_negative_exponent },

    { 0 } // required by acutest
};
