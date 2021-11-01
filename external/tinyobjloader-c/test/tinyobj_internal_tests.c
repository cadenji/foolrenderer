#define TINYOBJ_LOADER_C_IMPLEMENTATION
#define TEST_NO_MAIN

#include "tinyobj_internal_tests.h"
#include "tinyobj_loader_c.h"
#include "acutest.h"

void test_skip_space(void)
{
    {
        // Regular string should remain unchanged.
        const char * test_string = "foo bar";
        skip_space(&test_string);
        TEST_CHECK(strcmp(test_string, "foo bar") == 0);
    }

    {
        // String with leading spaces & tabs should have its prefix trimmed.
        const char * test_string = "\t \t foo bar";
        skip_space(&test_string);
        TEST_CHECK(strcmp(test_string, "foo bar") == 0);
    }

    {
        // String with trailing spaces & tabs should remain unchanged.
        const char * test_string = "foo bar \t \t";
        skip_space(&test_string);
        TEST_CHECK(strcmp(test_string, "foo bar \t \t") == 0);
    }
}

void test_skip_space_and_cr(void)
{
    {
        // Regular string should remain unchanged.
        const char * test_string = "foo bar";
        skip_space_and_cr(&test_string);
        TEST_CHECK(strcmp(test_string, "foo bar") == 0);
    }

    {
        // String with leading spaces, tabs, & carriage returns should have its
        // prefix trimmed.
        const char * test_string = "\r \t \t \rfoo bar";
        skip_space_and_cr(&test_string);
        TEST_CHECK(strcmp(test_string, "foo bar") == 0);
    }

    {
        // String with trailing spaces, tabs, & carriage returns should remain
        // unchanged.
        const char * test_string = "foo bar\r \t \t\r";
        skip_space_and_cr(&test_string);
        TEST_CHECK(strcmp(test_string, "foo bar\r \t \t\r") == 0);
    }
}

void test_until_space()
{
    {
        // Return value for regular string should be the number of non-null chars.
        char test_string[] = "potato";
        TEST_CHECK(until_space(test_string) == 6);
    }

    {
        // Return value for string with spaces should be number of chars to the
        // first space.
        char test_string[] = "foo bar baz";
        TEST_CHECK(until_space(test_string) == 3);
    }

    {
        // Return value for string with tabs should be number of chars to the
        // first tab.
        char test_string[] = "foo\tbar\tbaz";
        TEST_CHECK(until_space(test_string) == 3);
    }

    {
        // Return value for string with carriage returns should be number of
        // chars to the first carriage return.
        char test_string[] = "foo\rbar\rbaz";
        TEST_CHECK(until_space(test_string) == 3);
    }

    {
        // Return value for string with null characters should be number of
        // chars to the first null character.
        char test_string[] = "foo\0bar\0baz";
        TEST_CHECK(until_space(test_string) == 3);
    }
}

// TODO: Function assumes null terminated string. If this is the case, the
// string length isn't necessary as we can loop over the characters until
// '\0' is found.
void test_length_until_newline(void)
{
    {
        // Return value for null-terminated string without line breaks should be
        // number of non-null chars.
        char test_string[] = "potato";
        TEST_CHECK(length_until_newline(test_string, sizeof(test_string)) == 6);
    }

    {
        // Return value for null terminated string with a linebreak at the end should
        // be number of chars to newline.
        char test_string[] = "potato\n";
        TEST_CHECK(length_until_newline(test_string, sizeof(test_string)) == 6);
    }

    {
        // Return value for null-terminated string with a linebreak in the middle
        // should be number of chars to newline.
        char test_string[] = "potato\nmonkey";
        TEST_CHECK(length_until_newline(test_string, sizeof(test_string)) == 6);
    }

    {
        // Return value for null-terminated string with a linebreak in the middle and
        // at the end should be number of chars to first newline.
        char test_string[] = "potato\nmonkey\n";
        TEST_CHECK(length_until_newline(test_string, sizeof(test_string)) == 6);
    }

    {
        // Return value for empty null-terminated string should be 0.
        char test_string[] = "";
        TEST_CHECK(length_until_newline(test_string, sizeof(test_string)) == 0);
    }

    {
        // Return value for null-terminated string with a null character in the middle
        // should be the number of non-null chars plus the number of null characters
        // before the end of the string.
        char test_string[] = "pot\0ato";
        TEST_CHECK(length_until_newline(test_string, sizeof(test_string)) == 7);
    }
}

void test_my_atoi(void)
{
    // Results for input strings should become corresponding ints.
    TEST_CHECK(my_atoi("1") == 1);
    TEST_CHECK(my_atoi("-1") == -1);
    TEST_CHECK(my_atoi("+1") == 1);
    TEST_CHECK(my_atoi("0") == 0);
    TEST_CHECK(my_atoi("-0") == 0);
    TEST_CHECK(my_atoi("+0") == 0);
}

void test_fix_index(void)
{
    // Return value for 0 should be 0.
    TEST_CHECK(fixIndex(0, 12) == 0);

    // Return value for non 0-indexed value should become 0-indexed.
    TEST_CHECK(fixIndex(1, 12) == 0);
    TEST_CHECK(fixIndex(2, 12) == 1);

    // Return value for relative-indexed value should be offset from the total size.
    TEST_CHECK(fixIndex(-1, 12) == 11);
    TEST_CHECK(fixIndex(-12, 12) == 0);
    TEST_CHECK(fixIndex(-13, 12) == -1); // TODO: should the result be clamped to 0?
}

// TODO: Function name should be snake_case.
// TODO: Should 0x80000000 be a constant?
void test_parseRawTriple()
{
    {
        // TODO: Should this error on empty string?
        // Return value for empty string should give an index value of 0, and
        // uninitialised normal and texture coordinates.
        const char * test_string = "";
        tinyobj_vertex_index_t result = parseRawTriple(&test_string);
        TEST_CHECK(result.v_idx == 0);
        TEST_CHECK(result.vn_idx == 0x80000000);
        TEST_CHECK(result.vt_idx == 0x80000000);
    }

    {
        // Return value for a single number should return a vertex index value
        // of that number.
        const char * test_string = "3";
        tinyobj_vertex_index_t result = parseRawTriple(&test_string);
        TEST_CHECK(result.v_idx == 3);
        TEST_CHECK(result.vn_idx == 0x80000000);
        TEST_CHECK(result.vt_idx == 0x80000000);
    }

    {
        // Return value for two sequential numbers should return a vertex and
        // texture index value of those numbers.
        const char * test_string = "1/2";
        tinyobj_vertex_index_t result = parseRawTriple(&test_string);
        TEST_CHECK(result.v_idx == 1);
        TEST_CHECK(result.vt_idx == 2);
    }

    {
        // Return value for three numbers should return an index value, texture
        // coordinate and a normal value of those numbers.
        const char * test_string = "1/2/3";
        tinyobj_vertex_index_t result = parseRawTriple(&test_string);
        TEST_CHECK(result.v_idx == 1);
        TEST_CHECK(result.vt_idx == 2);
        TEST_CHECK(result.vn_idx == 3);
    }


    {
        // Return value for two split numbers should return a vertex and normal index
        // value of those numbers.
        const char * test_string = "1//3";
        tinyobj_vertex_index_t result = parseRawTriple(&test_string);
        TEST_CHECK(result.v_idx == 1);
        TEST_CHECK(result.vt_idx == 0x80000000);
        TEST_CHECK(result.vn_idx == 3);
    }
}

// TODO: Function name should be snake_case
// TODO: Function updates source pointer. is this intentional?
void test_parseInt()
{
    {
        // TODO: Should this error on empty string?
        // Return value for empty string should be integer 0.
        const char * test_string = "";
        TEST_CHECK(parseInt(&test_string) == 0);
    }

    {
        // Return value for string 0 should be integer 0.
        const char * test_string = "0";
        TEST_CHECK(parseInt(&test_string) == 0);
    }


    {
        // Return value for string +0 should be integer 0.
        const char * test_string = "+0";
        TEST_CHECK(parseInt(&test_string) == 0);
    }

    {
        // Return value for string -0 should be integer 0.
        const char * test_string = "-0";
        TEST_CHECK(parseInt(&test_string) == 0);
    }

    {
        // Return value for string 1 should be integer 1.
        const char * test_string = "1";
        TEST_CHECK(parseInt(&test_string) == 1);
    }


    {
        // Return value for string +1 should be integer 1.
        const char * test_string = "+1";
        TEST_CHECK(parseInt(&test_string) == 1);
    }

    {
        // Return value for string -1 should be integer -1.
        const char * test_string = "-1";
        TEST_CHECK(parseInt(&test_string) == -1);
    }
}

void test_tryParseDouble(void)
{
    // Empty string
    {
        // Return value for empty string should be false. Result should be double 0.
        const char * test_string = "";
        double result = 0.0;
        int success = tryParseDouble(test_string, test_string + strlen(test_string), &result);
        TEST_CHECK(!success);
        TEST_CHECK(result == 0.0);
    }

    // Zero
    {
        // Return value for string 0 should be true. Result should be double 0.
        const char * test_string = "0";
        double result = 0.0;
        int success = tryParseDouble(test_string, test_string + strlen(test_string), &result);
        TEST_CHECK(success);
        TEST_CHECK(result == 0.0);
    }

    {
        // Return value for string +0 should be true. Result should be double 0.
        const char * test_string = "+0";
        double result = 0.0;
        int success = tryParseDouble(test_string, test_string + strlen(test_string), &result);
        TEST_CHECK(success);
        TEST_CHECK(result == 0.0);
    }

    {
        // Return value for string -0 should be true. Result should be double 0.
        const char * test_string = "-0";
        double result = 0.0;
        int success = tryParseDouble(test_string, test_string + strlen(test_string), &result);
        TEST_CHECK(success);
        TEST_CHECK(result == 0.0);
    }

    {
        // Return value for string 0.0 should be true. Result should be double 0.
        const char * test_string = "0.0";
        double result;
        int success = tryParseDouble(test_string, test_string + strlen(test_string), &result);
        TEST_CHECK(success);
        TEST_CHECK(result == 0.0);
    }

    // One
    {
        // Return value for string 1 should be true. Result should be double 1.
        const char * test_string = "1";
        double result;
        int success = tryParseDouble(test_string, test_string + strlen(test_string), &result);
        TEST_CHECK(success);
        TEST_CHECK(result == 1.0);
    }

    {
        // Return value for string +1 should be true. Result should be double 1.
        const char * test_string = "+1";
        double result;
        int success = tryParseDouble(test_string, test_string + strlen(test_string), &result);
        TEST_CHECK(success);
        TEST_CHECK(result == 1.0);
    }

    {
        // Return value for string -1 should be true. Result should be double -1.
        const char * test_string = "-1";
        double result;
        int success = tryParseDouble(test_string, test_string + strlen(test_string), &result);
        TEST_CHECK(success);
        TEST_CHECK(result == -1.0);
    }

    {
        // Return value for string 1.0 should be true. Result should be double 1.
        const char * test_string = "1.0";
        double result;
        int success = tryParseDouble(test_string, test_string + strlen(test_string), &result);
        TEST_CHECK(success);
        TEST_CHECK(result == 1.0);
    }

    // Floating point
    {
        // Return value for string 1.25 should be true. Result should be double 1.25.
        const char * test_string = "1.25";
        double result;
        int success = tryParseDouble(test_string, test_string + strlen(test_string), &result);
        TEST_CHECK(success);
        TEST_CHECK(result == 1.25);
    }

    {
        // Return value for string +1.25 should be true. Result should be double 1.25.
        const char * test_string = "+1.25";
        double result;
        int success = tryParseDouble(test_string, test_string + strlen(test_string), &result);
        TEST_CHECK(success);
        TEST_CHECK(result == 1.25);
    }

    {
        // Return value for string -1.25 should be true. Result should be double -1.25.
        const char * test_string = "-1.25";
        double result;
        int success = tryParseDouble(test_string, test_string + strlen(test_string), &result);
        TEST_CHECK(success);
        TEST_CHECK(result == -1.25);
    }

    // Exponents
    {
        // Return value for string 1e6 should be true. Result should be double 1_000_000.
        const char * test_string = "1e6";
        double result;
        int success = tryParseDouble(test_string, test_string + strlen(test_string), &result);
        TEST_CHECK(success);
        TEST_CHECK(result == 1000000.0);
    }

    {
	// leading zero in exponent
        // Return value for string 1e06 should be true. Result should be double 1_000_000.
        const char * test_string = "1e06";
        double result;
        int success = tryParseDouble(test_string, test_string + strlen(test_string), &result);
        TEST_CHECK(success);
        TEST_CHECK(result == 1000000.0);
    }

    {
        // Return value for string 1E6 should be true. Result should be double 1_000_000.
        const char * test_string = "1E6";
        double result;
        int success = tryParseDouble(test_string, test_string + strlen(test_string), &result);
        TEST_CHECK(success);
        TEST_CHECK(result == 1000000.0);
    }

    {
        // Return value for string 1.024e3 should be true. Result should be double 1_024.
        const char * test_string = "1.024e3";
        double result;
        int success = tryParseDouble(test_string, test_string + strlen(test_string), &result);
        TEST_CHECK(success);
        TEST_CHECK(result == 1024.0);
    }

    {
        // Return value for string 1.024E3 should be true. Result should be double 1_024.
        const char * test_string = "1.024E3";
        double result;
        int success = tryParseDouble(test_string, test_string + strlen(test_string), &result);
        TEST_CHECK(success);
        TEST_CHECK(result == 1024.0);
    }

    {
    	// Negative exponent
        const char * test_string = "1e-3";
        double result;
        int success = tryParseDouble(test_string, test_string + strlen(test_string), &result);
        TEST_CHECK(success);
        TEST_CHECK(result == 0.001);
    }

    {
    	// Negative exponent with leading zero
        const char * test_string = "1e-03";
        double result;
        int success = tryParseDouble(test_string, test_string + strlen(test_string), &result);
        TEST_CHECK(success);
        TEST_CHECK(result == 0.001);
    }

    // Tests for exiting before end of the string.
    {
        // Return value for string 123 with a length of 2 characters should be true. Result should be double 12.
        const char * test_string = "123";
        double result;
        int success = tryParseDouble(test_string, test_string + strlen(test_string) - 1, &result);
        TEST_CHECK(success);
        TEST_CHECK(result == 12.0);
    }

    {
        // Return value for string 12 3 should be true. Result should be double 12.
        const char * test_string = "12 3";
        double result;
        int success = tryParseDouble(test_string, test_string + strlen(test_string), &result);
        TEST_CHECK(success);
        TEST_CHECK(result == 12.0);
    }
}

void test_parseFloat(void)
{
    // Empty string
    {
        // Return value for empty string should be float 0.
        const char * test_string = "";
        TEST_CHECK(parseFloat(&test_string) == 0.0);
        TEST_CHECK(strlen(test_string) == 0);
    }

    // Zero
    {
        // Return value for string 0 should be float 0.
        const char * test_string = "0";
        TEST_CHECK(parseFloat(&test_string) == 0.0);
        TEST_CHECK(strlen(test_string) == 0);
    }

    {
        // Return value for string +0 should be float 0.
        const char * test_string = "+0";
        TEST_CHECK(parseFloat(&test_string) == 0.0);
        TEST_CHECK(strlen(test_string) == 0);
    }

    {
        // Return value for string -0 should be float 0.
        const char * test_string = "-0";
        TEST_CHECK(parseFloat(&test_string) == 0.0);
        TEST_CHECK(strlen(test_string) == 0);
    }

    {
        // Return value for string 0.0 should be float 0.
        const char * test_string = "0.0";
        TEST_CHECK(parseFloat(&test_string) == 0.0);
        TEST_CHECK(strlen(test_string) == 0);
    }

    // One
    {
        // Return value for string 1 should be float 1.
        const char * test_string = "1";
        TEST_CHECK(parseFloat(&test_string) == 1.0);
        TEST_CHECK(strlen(test_string) == 0);
    }

    {
        // Return value for string +1 should be float 1.
        const char * test_string = "+1";
        TEST_CHECK(parseFloat(&test_string) == 1.0);
        TEST_CHECK(strlen(test_string) == 0);
    }

    {
        // Return value for string -1 should be float -1.
        const char * test_string = "-1";
        TEST_CHECK(parseFloat(&test_string) == -1.0);
        TEST_CHECK(strlen(test_string) == 0);
    }

    {
        // Return value for string 1.0 should be float 1.
        const char * test_string = "1.0";
        TEST_CHECK(parseFloat(&test_string) == 1.0);
        TEST_CHECK(strlen(test_string) == 0);
    }


    // Floating point
    {
        // Return value for string 1.25 should be float 1.25.
        const char * test_string = "1.25";
        TEST_CHECK(parseFloat(&test_string) == 1.25);
        TEST_CHECK(strlen(test_string) == 0);
    }

    {
        // Return value for string +1.25 should be float 1.25.
        const char * test_string = "+1.25";
        TEST_CHECK(parseFloat(&test_string) == 1.25);
        TEST_CHECK(strlen(test_string) == 0);
    }

    {
        // Return value for string -1.25 should be float -1.25.
        const char * test_string = "-1.25";
        TEST_CHECK(parseFloat(&test_string) == -1.25);
        TEST_CHECK(strlen(test_string) == 0);
    }

    // Exponents
    {
        // Return value for string 1e6 should be float 1_000_000.
        const char * test_string = "1e6";
        TEST_CHECK(parseFloat(&test_string) == 1000000.0);
        TEST_CHECK(strlen(test_string) == 0);
    }

    {
        // Return value for string 1E6 should be float 1_000_000.
        const char * test_string = "1E6";
        TEST_CHECK(parseFloat(&test_string) == 1000000.0);
        TEST_CHECK(strlen(test_string) == 0);
    }

    {
        // Return value for string 1.024e3 should be float 1_024.
        const char * test_string = "1.024e3";
        TEST_CHECK(parseFloat(&test_string) == 1024.0);
        TEST_CHECK(strlen(test_string) == 0);
    }

    {
        // Return value for string 1.024E3 should be float 1_024.
        const char * test_string = "1.024E3";
        TEST_CHECK(parseFloat(&test_string) == 1024.0);
        TEST_CHECK(strlen(test_string) == 0);
    }

    {
        // TODO: Negative exponents do not work.
        // Return value for string 1e-3 should be float 0.001.
        // const char * test_string = "1e-3";
        // TEST_CHECK(parseFloat(&test_string) == 0.001);
        // TEST_CHECK(strlen(test_string) == 0);
    }

    // Tests for exiting before end of the string
    {
        // Return value for string 12 3 should be float 12.
        const char * test_string = "12 3";
        TEST_CHECK(parseFloat(&test_string) == 12.0);
        TEST_CHECK(strcmp(test_string, " 3") == 0);
    }
}

void test_parseFloat2(void)
{
    // Simple test - this only calls parseFloat.
    float x, y;
    const char * test_string = "3.0 -2e3";
    parseFloat2(&x, &y, &test_string);
    TEST_CHECK(x == 3.0);
    TEST_CHECK(y == -2000.0);
    TEST_CHECK(strlen(test_string) == 0);
}

void test_parseFloat3(void)
{
    // Simple test - this only calls parseFloat.
    float x, y, z;
    const char * test_string = "3.0 -2e3 1";
    parseFloat3(&x, &y, &z, &test_string);
    TEST_CHECK(x == 3.0);
    TEST_CHECK(y == -2000.0);
    TEST_CHECK(z == 1.0);
    TEST_CHECK(strlen(test_string) == 0);
}

void test_my_strdup(void)
{
    {
        // Return value for a regular string should be a new string whose
        // contents are equal to the original.
        char * test_string = "potato";
        char * result = my_strdup(test_string, strlen(test_string));
        TEST_CHECK(test_string != result);
        TEST_CHECK(strcmp(test_string, result) == 0);
        free(result);
    }

    {
        // Return value for empty string should be a new empty string.
        char * test_string = "";
        char * result = my_strdup(test_string, strlen(test_string));
        TEST_CHECK(test_string != result);
        TEST_CHECK(strcmp(test_string, result) == 0);
        free(result);
    }
}

void test_my_strndup(void)
{
    {
        // Return value for a regular string and the length of that string should be a new
        // string whose contents are equal to the original.
        char * test_string = "potato";
        char * result = my_strndup(test_string, 6);
        TEST_CHECK(test_string != result);
        TEST_CHECK(strcmp(test_string, result) == 0);
        free(result);
    }

    {
        // Return value for a regular string and zero should be a new empty string.
        char * test_string = "potato";
        char * result = my_strndup(test_string, 0);
        TEST_CHECK(result == 0);
    }
}

void test_initMaterial(void)
{
    // Initialised material should have expected defaults.
    int i;
    tinyobj_material_t material;
    initMaterial(&material);
    TEST_CHECK(material.name == NULL);
    TEST_CHECK(material.ambient_texname == NULL);
    TEST_CHECK(material.diffuse_texname == NULL);
    TEST_CHECK(material.specular_texname == NULL);
    TEST_CHECK(material.specular_highlight_texname == NULL);
    TEST_CHECK(material.bump_texname == NULL);
    TEST_CHECK(material.displacement_texname == NULL);
    TEST_CHECK(material.alpha_texname == NULL);
    for (i = 0; i < 3; i++) {
        TEST_CHECK(material.ambient[i] == 0.f);
        TEST_CHECK(material.diffuse[i] == 0.f);
        TEST_CHECK(material.specular[i] == 0.f);
        TEST_CHECK(material.transmittance[i] == 0.f);
        TEST_CHECK(material.emission[i] == 0.f);
    }
    TEST_CHECK(material.illum == 0);
    TEST_CHECK(material.dissolve == 1.f);
    TEST_CHECK(material.shininess == 1.f);
    TEST_CHECK(material.ior == 1.f);
}

void test_create_hash_table(void)
{
    {
        // Initialised hash table should be initialised with a default capacity.
        hash_table_t table;
        create_hash_table(0, &table);
        TEST_CHECK(table.hashes != NULL);
        TEST_CHECK(table.entries != NULL);
        TEST_CHECK(table.capacity == HASH_TABLE_DEFAULT_SIZE);
        TEST_CHECK(table.n == 0);
        destroy_hash_table(&table);
    }

    {
        // Initialised hash table should be initialised with supplied capacity.
        hash_table_t table;
        create_hash_table(20, &table);
        TEST_CHECK(table.hashes != NULL);
        TEST_CHECK(table.entries != NULL);
        TEST_CHECK(table.capacity == 20);
        TEST_CHECK(table.n == 0);
        destroy_hash_table(&table);
    }
}

void test_hash_table_set(void)
{
    {
        // Values should be stored in the hash table.
        hash_table_t table;
        int foundFirst = 0;
        int foundSecond = 0;
        int foundOther = 0;
        int i;

        create_hash_table(20, &table);

        TEST_CHECK(table.hashes != NULL);
        TEST_CHECK(table.entries != NULL);
        TEST_CHECK(table.capacity == 20);
        TEST_CHECK(table.n == 0);

        hash_table_set("potato", 3, &table);
        hash_table_set("monkey", 8, &table);

        TEST_CHECK(table.n == 2);

        for (i = 0; i < table.capacity; ++i) {
            hash_table_entry_t n = table.entries[i];
            if (n.value == 3) {
                foundFirst++;
            } else if (n.value == 8) {
                foundSecond++;
            } else {
                foundOther++;
            }
        }

        TEST_CHECK(foundFirst == 1);
        TEST_CHECK(foundSecond == 1);
        TEST_CHECK(foundOther == 18);

        destroy_hash_table(&table);
    }

    {
        // Values with different hashes but same % capacity value should exist in the
        // the same hash table.
        hash_table_t table;
        int foundFirst = 0;
        int foundSecond = 0;
        int foundOther = 0;
        int i;

        create_hash_table(20, &table);

        TEST_CHECK(table.hashes != NULL);
        TEST_CHECK(table.entries != NULL);
        TEST_CHECK(table.capacity == 20);
        TEST_CHECK(table.n == 0);

        // hashes are different but % 20 value is the same - they will end up in the same bucket
        hash_table_set("potato", 3, &table); // hash 356168476
        hash_table_set("potat[", 3, &table); // hash 356168456

        // hashes are different but % 20 value is the same - they will end up in the same bucket
        hash_table_set("monkey", 8, &table); // hash 238557080
        hash_table_set("monkee", 8, &table); // hash 238557060

        TEST_CHECK(table.n == 4);

        for (i = 0; i < table.capacity; ++i) {
            if ((table.entries + i)->value == 3) {
                foundFirst++;
            } else if ((table.entries + i)->value == 8) {
                foundSecond++;
            } else {
                foundOther++;
            }
        }

        TEST_CHECK(foundFirst == 2);
        TEST_CHECK(foundSecond == 2);
        TEST_CHECK(foundOther == 16);

        destroy_hash_table(&table);
    }

    {
        // TODO: This fails. Should the key also be stored in the entry so it can be compared
        // with the key being inserted?
        // Values with identical hashes but different keys should exist in the same hash table.
        hash_table_t table;
        int foundFirst = 0;
        int foundSecond = 0;
        int foundOther = 0;
        int i;

        create_hash_table(20, &table);

        TEST_CHECK(table.hashes != NULL);
        TEST_CHECK(table.entries != NULL);
        TEST_CHECK(table.capacity == 20);
        TEST_CHECK(table.n == 0);

        hash_table_set("potato", 3, &table); // hash 356168476
        hash_table_set("potbSo", 3, &table); // hash 356168476 (identical)

        hash_table_set("monkey", 8, &table); // hash 238557080
        hash_table_set("monkfX", 8, &table); // hash 238557080 (identical)

        // TEST_CHECK(table.n == 4);

        for (i = 0; i < table.capacity; ++i) {
            hash_table_entry_t n = table.entries[i];
            if (n.value == 3) {
                foundFirst++;
            } else if (n.value == 8) {
                foundSecond++;
            } else {
                foundOther++;
            }
        }

        // TEST_CHECK(foundFirst == 2);
        // TEST_CHECK(foundSecond == 2);
        // TEST_CHECK(foundOther == 16);

        destroy_hash_table(&table);
    }
}

void test_hash_table_exists(void)
{
    // It should be possible to test for the presence of items in the hash table.
    hash_table_t table;
    create_hash_table(20, &table);

    hash_table_set("potato", 3, &table);
    hash_table_set("monkey", 8, &table);

    TEST_CHECK(hash_table_exists("potato", &table));
    TEST_CHECK(hash_table_exists("monkey", &table));
    TEST_CHECK(!hash_table_exists("radio", &table));

    destroy_hash_table(&table);
}

void test_hash_table_get(void)
{
    // It should be possible to retrieve item values from the hash table.
    hash_table_t table;
    create_hash_table(20, &table);

    hash_table_set("potato", 3, &table);
    hash_table_set("monkey", 8, &table);

    TEST_CHECK(hash_table_get("potato", &table) == 3);
    TEST_CHECK(hash_table_get("monkey", &table) == 8);

    // TODO: the value returned by hash_table_get can be from a null pointer.
    // A default sentinel value could be returned instead e.g. -(2^30)
    // TEST_CHECK(hash_table_get("radio", &table) == -(2 << 30));

    destroy_hash_table(&table);
}

void test_hash_table_maybe_grow(void)
{
    // It should be possible for the table to grow, preserving all previous values
    hash_table_t table;
    create_hash_table(10, &table);

    hash_table_set("Pottery_clay0", 0, &table);
    hash_table_set("Dark_smoked_gla0", 1, &table);
    hash_table_set("Pottery_clay1", 2, &table);
    hash_table_set("Dark_smoked_gla1", 3, &table);
    hash_table_set("Metallic_Varni0", 4, &table);
    hash_table_set("Body0", 5, &table);
    hash_table_set("Pottery_clay2", 6, &table);
    hash_table_set("850matri0", 7, &table);
    hash_table_set("850matri1", 8, &table);
    hash_table_set("Pottery_clay3", 9, &table);
    hash_table_set("Pottery_clay4", 10, &table);

    TEST_CHECK(table.capacity > 10);
    TEST_CHECK(table.n == 11);

    TEST_CHECK(hash_table_get("Pottery_clay0", &table) == 0);
    TEST_CHECK(hash_table_get("Dark_smoked_gla0", &table) == 1);
    TEST_CHECK(hash_table_get("Pottery_clay1", &table) == 2);
    TEST_CHECK(hash_table_get("Dark_smoked_gla1", &table) == 3);
    TEST_CHECK(hash_table_get("Metallic_Varni0", &table) == 4);
    TEST_CHECK(hash_table_get("Body0", &table) == 5);
    TEST_CHECK(hash_table_get("Pottery_clay2", &table) == 6);
    TEST_CHECK(hash_table_get("850matri0", &table) == 7);
    TEST_CHECK(hash_table_get("850matri1", &table) == 8);
    TEST_CHECK(hash_table_get("Pottery_clay3", &table) == 9);
    TEST_CHECK(hash_table_get("Pottery_clay4", &table) == 10);

    destroy_hash_table(&table);
}
