#ifndef INI_H
#define INI_H

#include <stdbool.h>

#define INI_MAX_STRING_SIZE 256
#define INI_MAX_LINE_SIZE 1024
#define INI_GLOBAL_SECTION "__GLOBAL__"

typedef struct
{
    char name[INI_MAX_STRING_SIZE];
} INISection_t;

typedef struct
{
    INISection_t section;
    char key[INI_MAX_STRING_SIZE];
    char value[INI_MAX_STRING_SIZE];
} INIPair_t;

typedef struct
{
    INIPair_t *pairs;
    unsigned pair_count;
} INIData_t;

INIData_t *ini_parse_file(const char *filename);
const char *ini_get_value(const INIData_t *data, const char *section, const char *key);
void ini_free(INIData_t *data);
bool ini_is_blank_line(const char *line);
bool ini_is_pair(const char *line, INIPair_t *pair);
bool ini_is_section(const char *line, INISection_t *section);


#endif //INI_H
