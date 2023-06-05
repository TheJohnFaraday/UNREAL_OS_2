#ifndef UTF8_PARSER
#define UTF8_PARSER
#include <stdint.h>

#define TABLE_ROWS 23
#define TABLE_COLS 3
typedef uint8_t *utf8_sequence;
int8_t utf8_index(const unsigned char *s, uint32_t len);
#endif
