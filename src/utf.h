#ifndef IC_UTF_H
#define IC_UTF_H 1
#include <stddef.h>
#ifndef __cplusplus
#include <uchar.h>
#endif

#ifdef __cplusplus
// cannot use "replace" when "using namespace std" because of "std::replace"
namespace ic {
	enum utf8fix : unsigned char {
		replace,            // standard method
		copy,               // use the broken value, (not finished and abandoned)
		skip,               // skip broken
		stop                // stop decoding
	};
}

[[deprecated("Use wcs_to_utf8() instead.")]]
size_t wcstoutf8(wchar_t *str, char *dst, size_t max, size_t *pos = NULL, size_t len = (size_t)-1);
[[deprecated("Use utf8_to_wcs() instead.")]]
size_t utf8towcs(char *str, wchar_t *dst, size_t max, size_t *pos = NULL, ic::utf8fix f = ic::replace, size_t len = (size_t)-1);

#endif



enum utf_fix {
	chr_replace = 0,      // standard method
	chr_copy = 1,         // use the broken value, (not finished and abandoned)
	chr_skip = 2,         // skip broken
	chr_stop = 3          // stop decoding
};

#define __ONLY_CPP(x)
#ifdef __cplusplus
#undef __ONLY_CPP
#define __ONLY_CPP(x) x
extern "C" {
#endif
size_t wcs_to_utf8(wchar_t *str, char *dst, size_t max, size_t *pos __ONLY_CPP(=NULL), size_t len __ONLY_CPP(=(size_t)-1));
size_t utf8_to_wcs(char *str, wchar_t *dst, size_t max, size_t *pos __ONLY_CPP(=NULL), enum utf_fix f __ONLY_CPP(=chr_replace), size_t len __ONLY_CPP(=(size_t)-1));

size_t wcs_to_utf16(wchar_t *str, char16_t *dst, size_t max, size_t *pos __ONLY_CPP(=NULL), size_t len __ONLY_CPP(=(size_t)-1));
size_t utf16_to_wcs(char16_t *str, wchar_t *dst, size_t max, size_t *pos __ONLY_CPP(=NULL), enum utf_fix f __ONLY_CPP(=chr_replace), size_t len __ONLY_CPP(=(size_t)-1));

#ifdef __cplusplus
}
#endif
#endif
