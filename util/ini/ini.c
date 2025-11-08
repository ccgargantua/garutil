#include "ini.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_ALLOCATED_PAIRS 32

INIData_t *ini_parse_file(const char *filename)
{
    FILE *file = fopen(filename, "r");
    assert(file);

    INIData_t *data = malloc(sizeof(INIData_t));
    assert(data);
    int allocated_pairs = INITIAL_ALLOCATED_PAIRS;
    data->pair_count = 0;
    data->pairs = malloc(sizeof(INIPair_t) * allocated_pairs);

    char line[INI_MAX_LINE_SIZE];
    INISection_t section;
    INISection_t current_section = {INI_GLOBAL_SECTION};
    while (fgets(line, INI_MAX_LINE_SIZE, file))
    {
        if (ini_is_blank_line(line)) continue;
        if (ini_is_section(line, &section))
        {
            current_section = section;
            continue;
        }
        INIPair_t pair;
        pair.section = current_section;
        if (ini_is_pair(line, &pair))
        {
            if (data->pair_count >= allocated_pairs)
            {
                allocated_pairs *= 2;
                data->pairs = realloc(data->pairs, sizeof(INIPair_t) * allocated_pairs);
                assert(data->pairs);
            }
            data->pairs[data->pair_count++] = pair;
        }
        else assert(false && "Parsing fail.");
    }

    fclose(file);
    return data;
}

const char *ini_get_value(const INIData_t *data, const char *section, const char *key)
{
    assert(data);
    assert(data->pairs);
    assert(data->pair_count > 0);
    assert(section);
    assert(key);

    for (int i = 0; i < data->pair_count; i++)
    {
        const char *current_section = data->pairs[i].section.name;
        const char *current_key = data->pairs[i].key;
        if (strcmp(current_section, section) == 0
            && strcmp(current_key, key) == 0)
            return data->pairs[i].value;
    }
    return NULL;
}

void ini_free(INIData_t *data)
{
    if (!data) return;
    if (data->pairs) free(data->pairs);
    free(data);
}

// Assumes line is null-terminated.
bool ini_is_pair(const char *line, INIPair_t *pair)
{
    assert(line);
    const char *c = line;

    char *dest_c = NULL;
    if (pair) dest_c = pair->key;

    // Spaces before key
    while (isspace(*c)) c++;

    // Key
    if (!isalpha(*c) && *c != '_') goto is_not_pair;
    while (isalnum(*c) || *c == '_')
    {
        if (dest_c)
        {
            assert(dest_c - pair->key < INI_MAX_STRING_SIZE && "Exceeded max string length.");
            *dest_c++ = *c;
        }
        c++;
    }
    if (dest_c) *dest_c = '\0';

    // Spaces between key and =
    while (isspace(*c)) c++;

    // =
    if (*c != '=') goto is_not_pair;
    c++;

    if (pair)
        dest_c = pair->value;

    // Spaces before value
    while (isspace(*c)) c++;

    // value
    while (!isspace(*c) && *c != '\0')
    {
        if (dest_c)
        {
            assert(dest_c - pair->value < INI_MAX_STRING_SIZE && "Exceeded max string length.");
            *dest_c++ = *c;
        }
        c++;
    }
    if (dest_c) *dest_c = '\0';

    // Trailing spaces, comments, null terminator
    while (isspace(*c)) c++;
    if (*c != '#' && *c != ';' && *c != '\0') goto is_not_pair;

    return true;

    is_not_pair:
    if (pair)
    {
        pair->key[0] = '\0';
        pair->value[0] = '\0';
    }
    return false;
}

// Assumes line is null-terminated.
bool ini_is_blank_line(const char *line)
{
    while (isspace(*line)) line++;
    if (*line == '\0' || *line == ';' || *line == '#') return true;
    return false;
}

// Assumes line is null-terminated.
bool ini_is_section(const char *line, INISection_t *section)
{
    assert(line);
    const char *c = line;

    char *dest_c = NULL;
    if (section)
        dest_c = section->name;

    // Leading spaces
    while (isspace(*c)) c++;

    // Begin section name
    if (*c++ != '[') goto is_not_section;
    while (isspace(*c)) c++;

    // Section name
    if (!isalpha(*c) && *c != '_') goto is_not_section;
    while (isalnum(*c) || *c == '_')
    {
        if (dest_c)
        {
            assert(dest_c - section->name < INI_MAX_STRING_SIZE && "Exceeded max string length.");
            *dest_c++ = *c;
        }
        c++;
    }
    if (dest_c) *dest_c = '\0';

    while (isspace(*c)) c++;
    if (*c != ']') goto is_not_section;
    c++;

    while (isspace(*c)) c++;
    if (*c != '#' && *c != ';' && *c != '\0') goto is_not_section;

    return true;

    is_not_section:
    if (section)
        section->name[0] = '\0';
    return false;
}