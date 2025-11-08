#include "rktest.h"
#include "ini/ini.h"

TEST(ini_tests, blank_lines)
{
    const char empty_line[] = "";
    const char space_line[] = "   ";
    const char new_line[] = "\n";
    const char null_line[] = "\0";
    const char comment_line[] = "# comment here!";
    const char alt_comment_line[] = "; and here!";

    ASSERT_TRUE(ini_is_blank_line(empty_line));
    ASSERT_TRUE(ini_is_blank_line(space_line));
    ASSERT_TRUE(ini_is_blank_line(new_line));
    ASSERT_TRUE(ini_is_blank_line(null_line));
    ASSERT_TRUE(ini_is_blank_line(comment_line));
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
    const char line_comment[] = "key=value # comment";
    const char line_alt_comment[] = "key=value ; comment";

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

    ASSERT_TRUE(ini_is_pair(line_alt_comment, &pair));
    ASSERT_STREQ(pair.key, "key");
    ASSERT_STREQ(pair.value, "value");
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

    ASSERT_FALSE(ini_is_pair(line_invalid_key, NULL));
    ASSERT_FALSE(ini_is_pair(line_invalid_value, NULL));
    ASSERT_FALSE(ini_is_pair(line_invalid_value_spaces, NULL));
    ASSERT_FALSE(ini_is_pair(line_half, NULL));
    ASSERT_FALSE(ini_is_pair(line_empty, NULL));
    ASSERT_FALSE(ini_is_pair(line_null, NULL));
    ASSERT_FALSE(ini_is_pair(line_comment, NULL));
}

TEST(ini_tests, sections)
{
    const char line[] = "[section]";
    const char line_spaces[] = " [ section ] ";
    const char line_comment[] = "[section] # comment here";
    const char line_alt_comment[] = "[section] ; and here";

    INISection_t section;
    ASSERT_TRUE(ini_is_section(line, &section));
    ASSERT_STREQ(section.name, "section");

    ASSERT_TRUE(ini_is_section(line_spaces, &section));
    ASSERT_STREQ(section.name, "section");

    ASSERT_TRUE(ini_is_section(line_comment, &section));
    ASSERT_STREQ(section.name, "section");

    ASSERT_TRUE(ini_is_section(line_alt_comment, &section));
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