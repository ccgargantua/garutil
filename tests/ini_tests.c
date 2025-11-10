#include "rktest.h"
#include "ini/ini.h"



TEST(ini_tests, blank_lines)
{
    const char empty_line[] = "";
    const char space_line[] = "   ";
    const char new_line[] = "\n";
    const char null_line[] = "\0";
    const char alt_comment_line[] = "; comment here!";

    ASSERT_TRUE(ini_is_blank_line(empty_line));
    ASSERT_TRUE(ini_is_blank_line(space_line));
    ASSERT_TRUE(ini_is_blank_line(new_line));
    ASSERT_TRUE(ini_is_blank_line(null_line));
    ASSERT_TRUE(ini_is_blank_line(alt_comment_line));
}



TEST(ini_tests, nonblank_line)
{
    const char line[] = " abcdefghijklm nopqrstuvwxyz ";
    ASSERT_FALSE(ini_is_blank_line(line));
}



TEST(ini_tests, pairs)
{
    const char line[] = "key=value";
    const char line_spaces[] = " key = value ";
    const char line_comment[] = "key=value ; comment";
    const char line_string[] = "key=\"this is a value\"";

    INIPair_t pair;

    ASSERT_TRUE(ini_is_pair(line, &pair));
    ASSERT_STREQ(pair.key, "key");
    ASSERT_STREQ(pair.value, "value");

    ASSERT_TRUE(ini_is_pair(line_spaces, &pair));
    ASSERT_STREQ(pair.key, "key");
    ASSERT_STREQ(pair.value, "value");

    ASSERT_TRUE(ini_is_pair(line_comment, &pair));
    ASSERT_STREQ(pair.key, "key");
    ASSERT_STREQ(pair.value, "value");

    ASSERT_TRUE(ini_is_pair(line_string, &pair));
    ASSERT_STREQ(pair.key, "key");
    ASSERT_STREQ(pair.value, "this is a value");
}



TEST(ini_tests, bad_pairs)
{
    const char line_invalid_key[] = "1key=value";
    const char line_invalid_value[] = "key=va lue";
    const char line_invalid_value_spaces[] = "kay = value space";
    const char line_half[] = "key";
    const char line_empty[] = "";
    const char line_null[] = "\0";
    const char line_comment[] = "#key=value";
    const char line_bad_string[] = "key=\"this is a # bad string\"";

    ASSERT_FALSE(ini_is_pair(line_invalid_key, NULL));
    ASSERT_FALSE(ini_is_pair(line_invalid_value, NULL));
    ASSERT_FALSE(ini_is_pair(line_invalid_value_spaces, NULL));
    ASSERT_FALSE(ini_is_pair(line_half, NULL));
    ASSERT_FALSE(ini_is_pair(line_empty, NULL));
    ASSERT_FALSE(ini_is_pair(line_null, NULL));
    ASSERT_FALSE(ini_is_pair(line_comment, NULL));
    ASSERT_FALSE(ini_is_pair(line_bad_string, NULL));
}



TEST(ini_tests, sections)
{
    const char line[] = "[section]";
    const char line_spaces[] = " [ section ] ";
    const char line_comment[] = "[section] ; comment here";

    INISection_t section;
    ASSERT_TRUE(ini_is_section(line, &section));
    ASSERT_STREQ(section.name, "section");

    ASSERT_TRUE(ini_is_section(line_spaces, &section));
    ASSERT_STREQ(section.name, "section");

    ASSERT_TRUE(ini_is_section(line_comment, &section));
    ASSERT_STREQ(section.name, "section");
}



TEST(ini_tests, bad_sections)
{
    const char line_invalid[] = "x[section]";
    const char line_invalid_double[] = "[section section]";
    const char line_empty[] = "";
    const char line_spaces[] = "  ";
    const char line_null[] = "\0";
    const char line_comment[] = "# comment here";

    ASSERT_FALSE(ini_is_section(line_invalid, NULL));
    ASSERT_FALSE(ini_is_section(line_invalid_double, NULL));
    ASSERT_FALSE(ini_is_section(line_empty, NULL));
    ASSERT_FALSE(ini_is_section(line_spaces, NULL));
    ASSERT_FALSE(ini_is_section(line_null, NULL));
    ASSERT_FALSE(ini_is_section(line_comment, NULL));
}



TEST(ini_tests, file_parsing)
{
    INIData_t *data = ini_parse_file("../tests/test.ini");
    ASSERT_TRUE(data != NULL);
    ASSERT_STREQ(ini_get_value(data, "section", "hello"), "world");
    ASSERT_STREQ(ini_get_value(data, "section", "hi"), "true");
    ASSERT_STREQ(ini_get_value(data, "section", "val"), "5");
    ASSERT_STREQ(ini_get_value(data, "another", "this_one"), "yeah");
    ini_free(data);
}



TEST(ini_tests, file_writing)
{
    INIData_t *data = ini_parse_file("../tests/test.ini");
    ini_write_file(data, "../tests/test_copy.ini");
    INIData_t *copy = ini_parse_file("../tests/test_copy.ini");

    ASSERT_EQ(data->section_count, copy->section_count);

    for (int i = 0; i < data->section_count; i++)
    {
        const INISection_t *section = &data->sections[i];
        for (int j = 0; j < section->pair_count; j++)
        {
            const char *key = section->pairs[j].key;
            const char *value = section->pairs[j].value;

            ASSERT_STREQ(value, ini_get_value(copy, section->name, key));

        }
    }

    ini_free(data);
    ini_free(copy);
}