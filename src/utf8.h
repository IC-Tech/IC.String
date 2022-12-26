#include <stdio.h>
#include <stddef.h>
#ifndef IC_UTF8_H
#define IC_UTF8_H 1
enum utf8fix : unsigned char {
	replace, // standard method
	copy, // use the broken value, (not finished, ignored)
	skip, // skip broken
	stop // stop decoding
};
size_t wcstoutf8(wchar_t *str, char *dst, size_t max, size_t *pos, size_t len = (size_t)-1);
size_t utf8towcs(char *str, wchar_t *dst, size_t max, size_t *pos, utf8fix f = replace, size_t len = (size_t)-1);
#endif
