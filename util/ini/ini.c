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



void ini_write_file(const INIData_t *data, const char *filename)
{
    assert(data);
    assert(filename);

    FILE *file = fopen(filename, "w");
    assert(file);

    for (int i = 0; i < data->section_count; i++)
    {
        const INISection_t *section = &data->sections[i];
        fputs("\n[", file);
        fputs(section->name, file);
        fputs("]\n", file);
        for (int j = 0; j < section->pair_count; j++)
        {
            const INIPair_t *pair = &section->pairs[j];
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
    if (!data || !section || !data->sections) return NULL;
    static INISection_t *cached = NULL;
    if (cached)
        if (strncmp(section, cached->name, INI_MAX_STRING_SIZE) == 0)
            return cached;
    for (int i = 0; i < data->section_count; i++)
        if (strncmp(section, data->sections[i].name, INI_MAX_STRING_SIZE) == 0)
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
        INISection_t *re = realloc(data->sections, sizeof(INIPair_t) * data->section_allocation);
        if (!re) return NULL;
        data->sections = re;
    }
    INISection_t *section = &data->sections[data->section_count++];
    ini_section_init(name, section);
    return section;
}



INIPair_t *ini_add_pair(const INIData_t *data, const char *section, const INIPair_t pair)
{
    INISection_t *existing_section = ini_has_section(data, section);
    if (!existing_section) return NULL;
    return ini_add_pair_to_section(existing_section, pair);
}



INIPair_t *ini_add_pair_to_section(INISection_t *section, const INIPair_t pair)
{
    assert(section);

    if (section->pair_count >= section->pair_allocation)
    {
        section->pair_allocation *= 2;
        INIPair_t *re = realloc(section->pairs, sizeof(INIPair_t) * section->pair_allocation);
        if (!re)
            return NULL;
        section->pairs = re;
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
        if (strncmp(data->sections[i].name, section, INI_MAX_STRING_SIZE) == 0)
        {
            found_section = &data->sections[i];
            break;
        }
    }

    if (!found_section) return NULL;

    for (int i = 0; i < found_section->pair_count; i++)
    {
        if (strncmp(found_section->pairs[i].key, key, INI_MAX_STRING_SIZE) == 0)
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



static const char *skip_ignored_characters_(const char *c)
{
    while (isspace(*c)) c++;
    if (*c == ';' || *c == '#')
        while (*c != '\0') c++;
    return c;
}



// Assumes line is null-terminated.
bool ini_is_blank_line(const char *line)
{
    const char *c = skip_ignored_characters_(line);
    return *c == '\0';
}



static bool is_valid_section_starting_character_(const char c)
{
    return isalpha(c) || c == '_';
}



static bool is_valid_section_character_(const char c)
{
    return isalnum(c) || c == '_';
}



// Assumes line is null-terminated.
bool ini_is_section(const char *line, INISection_t *section)
{
    assert(line);
    const char *c = line;

    char *dest_c = NULL;
    if (section)
        dest_c = section->name;

    c = skip_ignored_characters_(c);
    if (*c++ != '[') goto is_not_section;
    c = skip_ignored_characters_(c);

    if (!is_valid_section_starting_character_(*c)) goto is_not_section;
    while (is_valid_section_character_(*c))
    {
        if (dest_c)
        {
            assert(dest_c - section->name < INI_MAX_STRING_SIZE && "Exceeded max string length.");
            if (dest_c - section->name >= INI_MAX_STRING_SIZE)
                return false;
            *dest_c++ = *c;
        }
        c++;
    }
    if (dest_c) *dest_c = '\0';

    c = skip_ignored_characters_(c);
    if (*c != ']') goto is_not_section;
    c++;

    c = skip_ignored_characters_(c);
    if (*c != '\0') goto is_not_section;

    return true;

    is_not_section:
    if (section)
        section->name[0] = '\0';
    return false;
}



static bool is_valid_key_starting_value_(const char c)
{
    return isalpha(c) || c == '_';
}



static bool is_valid_key_character_(const char c)
{
    return isalnum(c) || c == '_';
}



static bool is_valid_value_character_(const char c, const bool quoted)
{
    if isalnum(c) return true;
    const char valid_special[] = "_-+.,:\'()[]{}\\/";
    for (int i = 0; i < sizeof(valid_special); i++)
        if (c == valid_special[i]) return true;
    if (quoted && c == ' ') return true;
    return false;
}



// Assumes line is null-terminated.
bool ini_is_pair(const char *line, INIPair_t *pair)
{
    assert(line);
    const char *c = line;

    char *dest_c = NULL;
    if (pair) dest_c = pair->key;

    c = skip_ignored_characters_(c);

    // Key
    if (!is_valid_key_starting_value_(*c)) goto is_not_pair;
    while (is_valid_key_character_(*c))
    {
        if (dest_c)
        {
            if (dest_c - pair->key >= INI_MAX_STRING_SIZE) return false;
            *dest_c++ = *c;
        }
        c++;
    }
    if (dest_c) *dest_c = '\0';

    c = skip_ignored_characters_(c);
    if (*c != '=') goto is_not_pair;
    c++;
    c = skip_ignored_characters_(c);

    if (pair)
        dest_c = pair->value;


    // value
    bool quoted = false;
    if (*c == '"')
    {
        quoted = true;
        c++;
    }
    while (is_valid_value_character_(*c, quoted))
    {
        if (dest_c)
        {
            if (dest_c - pair->value >= INI_MAX_STRING_SIZE) return false;
            *dest_c++ = *c;
        }
        c++;
    }
    if (dest_c) *dest_c = '\0';

    if (quoted)
    {
        if (*c == '"') c++;
        else goto is_not_pair;
    }
    c = skip_ignored_characters_(c);
    if (*c != '\0') goto is_not_pair;

    return true;

    is_not_pair:
    if (pair)
    {
        pair->key[0] = '\0';
        pair->value[0] = '\0';
    }
    return false;
}
