#include "ini.h"



#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define INITIAL_ALLOCATED_PAIRS 32
#define INITIAL_ALLOCATED_SECTIONS 8



INIData_t *ini_parse_file(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file) return NULL;

    INIData_t *data = malloc(sizeof(INIData_t));
    assert(data);

    data->section_count = 0;
    data->section_allocation = INITIAL_ALLOCATED_SECTIONS;
    data->sections = malloc(sizeof(INIPair_t) * data->section_allocation);

    char line[INI_MAX_LINE_SIZE];
    INISection_t *current_section = NULL;
    while (fgets(line, INI_MAX_LINE_SIZE, file))
    {
        if (ini_is_blank_line(line)) continue;

        INISection_t dest_section;
        if (ini_is_section(line, &dest_section))
        {
            INISection_t *existing_section = ini_has_section(data, dest_section.name);
            if (existing_section)
            {
                current_section = existing_section;
                continue;
            }
            current_section = ini_add_section(data, dest_section.name);
            continue;
        }

        INIPair_t pair;
        if (ini_is_pair(line, &pair))
        {
            if (!current_section) return NULL;
            ini_add_pair_to_section(current_section, pair);
        }
        else return NULL;
    }

    fclose(file);
    return data;
}



void ini_write_file(INIData_t *data, const char *filename)
{
    assert(data);
    assert(filename);

    FILE *file = fopen(filename, "w");
    assert(file);

    for (int i = 0; i < data->section_count; i++)
    {
        INISection_t *section = &data->sections[i];
        fputs("\n[", file);
        fputs(section->name, file);
        fputs("]\n", file);
        for (int j = 0; j < section->pair_count; j++)
        {
            INIPair_t *pair = &section->pairs[j];
            fputs(pair->key, file);
            fputs("=", file);
            fputs(pair->value, file);
            fputs("\n", file);
        }
    }

    fclose(file);
}



INISection_t *ini_has_section(const INIData_t *data, const char *section)
{
    assert(data);
    assert(section);
    assert(data->sections || data->section_count == 0);
    static INISection_t *cached = NULL;
    if (cached)
        if (strcmp(section, cached->name) == 0)
            return cached;
    for (int i = 0; i < data->section_count; i++)
        if (strcmp(section, data->sections[i].name) == 0)
        {
            cached = &data->sections[i];
            return cached;
        }
    return NULL;
}



void ini_section_init(const char *name, INISection_t *section)
{
    assert(section);
    memset(section->name, 0, INI_MAX_STRING_SIZE);
    strncpy(section->name, name, INI_MAX_STRING_SIZE - 1);
    section->pair_count = 0;
    section->pairs = malloc(sizeof(INIPair_t) * INITIAL_ALLOCATED_PAIRS);
    section->pair_allocation = INITIAL_ALLOCATED_PAIRS;
}



INISection_t *ini_add_section(INIData_t *data, const char *name)
{
    if (ini_has_section(data, name)) return NULL;

    if (data->section_count >= data->section_allocation)
    {
        data->section_allocation *= 2;
        data->sections = realloc(data->sections, sizeof(INIPair_t) * data->section_allocation);
        assert(data->sections);
    }
    INISection_t *section = &data->sections[data->section_count++];
    ini_section_init(name, section);
    return section;
}



INIPair_t *ini_add_pair(const INIData_t *data, const char *section, INIPair_t pair)
{
    INISection_t *existing_section = ini_has_section(data, section);
    if (!existing_section) return NULL;
    return ini_add_pair_to_section(existing_section, pair);
}



INIPair_t *ini_add_pair_to_section(INISection_t *section, INIPair_t pair)
{
    assert(section);

    if (section->pair_count >= section->pair_allocation)
    {
        section->pair_allocation *= 2;
        section->pairs = realloc(section->pairs, sizeof(INIPair_t) * section->pair_allocation);
        assert(section->pairs);
    }

    INIPair_t *new_pair = &section->pairs[section->pair_count++];
    *new_pair = pair;
    return new_pair;
}



const char *ini_get_value(const INIData_t *data, const char *section, const char *key)
{
    assert(data);
    assert(data->sections);
    assert(section);
    assert(key);

    INISection_t *found_section = NULL;
    for (int i = 0; i < data->section_count; i++)
    {
        if (strcmp(data->sections[i].name, section) == 0)
        {
            found_section = &data->sections[i];
            break;
        }
    }

    if (!found_section) return NULL;

    for (int i = 0; i < found_section->pair_count; i++)
    {
        if (strcmp(found_section->pairs[i].key, key) == 0)
            return found_section->pairs[i].value;
    }

    return NULL;
}



void ini_free(INIData_t *data)
{
    if (!data) return;
    if (data->sections)
    {
        for (int i = 0; i < data->section_count; i++)
            if (data->sections[i].pairs)
                free(data->sections[i].pairs);
        free(data->sections);
    }
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