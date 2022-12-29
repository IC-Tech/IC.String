/**
 * @file utf.h
 *
 * @brief UTF conversion functions
 */

#ifndef FILE_IC_UTF_H
#define FILE_IC_UTF_H 1

#include <cstddef>
#include <algorithm>
#include <cerrno>
#include <stdexcept>

// cannot use "replace" when "using namespace std" because of "std::replace"
namespace ic {

/** @brief options for how should corrupted strings be handled */
enum utf_fix {
	replace = 0,      ///< standard method, replaces characters with U+FFFD
	copy = 1,         ///< use the corrupted character (not completed written but it's abandoned)
	skip = 2,         ///< ignore and skip broken characters
	stop = 3          ///< stop decoding immediately
};

/**
 * @brief convert wide character string into multi-byte UTF-8 string
 * @warning deprecated, Use wcs_to_utf8() instead.
 *
 * @param str       input wide character string
 * @param[out] dst  destination for UTF-8 string
 * @param max       maximum size of destination
 * @param[out] pos  position for start and will change with the number of characters used in output string
 * @param len       length of input string. provide -1 for use as a null-terminated string
 * @return processed input character count
 */
[[deprecated("Use wcs_to_utf8() instead.")]]
size_t wcstoutf8(wchar_t *str, char *dst, size_t max, size_t *pos = NULL, size_t len = (size_t)-1);
/**
 * @brief convert multi-byte UTF-8 string into wide character string
 * @warning deprecated, Use utf8_to_wcs() instead.
 *
 * @param str       input UTF-8 string
 * @param[out] dst  destination for wide character string
 * @param max       maximum size of destination
 * @param[out] pos  position for start and will change with the number of characters used in output string
 * @param fix       fix mood for corrupted characters
 * @param len       length of input string. provide -1 for use as a null-terminated string
 * @return processed input character count
 */
[[deprecated("Use utf8_to_wcs() instead.")]]
size_t utf8towcs(char *str, wchar_t *dst, size_t max, size_t *pos = NULL, utf_fix fix = replace, size_t len = (size_t)-1);





/**
 * @brief convert wide character string into multi-byte UTF-8 string
 *
 * @param str       input wide character string
 * @param[out] dst  destination for UTF-8 string
 * @param max       maximum size of destination
 * @param[out] pos  position of the input string when finished converting
 * @param len       length of input string. provide -1 for use as a null-terminated string
 * @return number of characters written to the destination
 */
size_t wcs_to_utf8(const wchar_t *str, char *dst, size_t max, size_t *pos = NULL, size_t len = (size_t)-1);

/**
 * @brief convert multi-byte UTF-8 string into wide character string
 *
 * @param str       input UTF-8 string
 * @param[out] dst  destination for wide character string
 * @param max       maximum size of destination
 * @param[out] pos  position of the input string when finished converting
 * @param fix       fix mood for corrupted characters
 * @param len       length of input string. provide -1 for use as a null-terminated string
 * @return processed input character count
 */
size_t utf8_to_wcs(const char *str, wchar_t *dst, size_t max, size_t *pos = NULL, enum utf_fix fix = replace, size_t len = (size_t)-1);



/**
 * @brief convert wide character string into multi-byte UTF-16 string
 *
 * @param str       input wide character string
 * @param[out] dst  destination for UTF-16 string
 * @param max       maximum size of destination
 * @param[out] pos  position of the input string when finished converting
 * @param len       length of input string. provide -1 for use as a null-terminated string
 * @return number of characters written to the destination
 */
size_t wcs_to_utf16(const wchar_t *str, char16_t *dst, size_t max, size_t *pos = NULL, size_t len = (size_t)-1);

/**
 * @brief convert multi-byte UTF-16 string into wide character string
 *
 * @param str       input UTF-16 string
 * @param[out] dst  destination for wide character string
 * @param max       maximum size of destination
 * @param[out] pos  position of the input string when finished converting
 * @param fix       fix mood for corrupted characters
 * @param len       length of input string. provide -1 for use as a null-terminated string
 * @return processed input character count
 */
size_t utf16_to_wcs(const char16_t *str, wchar_t *dst, size_t max, size_t *pos = NULL, enum utf_fix fix = replace, size_t len = (size_t)-1);



/**
 * @brief convert multi-byte UTF-8 string into multi-byte UTF-16 string
 *
 * @param str       input multi-byte UTF-8 string
 * @param[out] dst  destination for UTF-16 string
 * @param max       maximum size of destination
 * @param[out] pos  position of the input string when finished converting
 * @param fix       fix mood for corrupted characters
 * @param len       length of input string. provide -1 for use as a null-terminated string
 * @return number of characters written to the destination
 */
size_t utf8_to_utf16(const char *str, char16_t *dst, size_t max, size_t *pos = NULL, enum utf_fix fix = replace, size_t len = (size_t)-1);

/**
 * @brief convert multi-byte UTF-16 string into multi-byte UTF-8 string
 *
 * @param str       input UTF-16 string
 * @param[out] dst  destination for multi-byte UTF-8 string
 * @param max       maximum size of destination
 * @param[out] pos  position of the input string when finished converting
 * @param fix       fix mood for corrupted characters
 * @param len       length of input string. provide -1 for use as a null-terminated string
 * @return processed input character count
 */
size_t utf16_to_utf8(const char16_t *str, char *dst, size_t max, size_t *pos = NULL, enum utf_fix fix = replace, size_t len = (size_t)-1);



/**
 * @brief overloaded functions for converts one encoding to other
 *
 * @param str       input  string
 * @param[out] dst  output string
 * @param max       maximum size of output buffer
 * @param[out] pos  position of the input string when finished converting
 * @param len       length of input string. provide -1 for use as a null-terminated string
 * @return processed input character count
 */
///@{
inline size_t cvt_utf(const wchar_t *str, char16_t *dst, size_t max, size_t *pos, size_t len) { return wcs_to_utf16(str, dst, max, pos, len); }
inline size_t cvt_utf(const wchar_t *str, char *dst, size_t max, size_t *pos, size_t len) { return wcs_to_utf8(str, dst, max, pos, len); }
inline size_t cvt_utf(const char16_t *str, wchar_t *dst, size_t max, size_t *pos, size_t len) { return utf16_to_wcs(str, dst, max, pos, replace, len); }
inline size_t cvt_utf(const char16_t *str, char *dst, size_t max, size_t *pos, size_t len) { return utf16_to_utf8(str, dst, max, pos, replace, len); }
inline size_t cvt_utf(const char *str, wchar_t *dst, size_t max, size_t *pos, size_t len) { return utf8_to_wcs(str, dst, max, pos, replace, len); }
inline size_t cvt_utf(const char *str, char16_t *dst, size_t max, size_t *pos, size_t len) { return utf8_to_utf16(str, dst, max, pos, replace, len); }
///@}

/**
 * @brief converts one encoding to other
 *
 * @details
 * Usage: Use `ic::cvt_utf_simple<char, wchar_t>("test")` to get wide character string
 *
 * @note the output string should freed manually
 *
 * @param str           input string
 * @param len           length of the input
 * @param[out] s_out    length of the output
 * @param block_size    size of a memory allocation block
 * @tparam t_input      input format
 * @tparam t_output     output format
 * @exception std::runtime_error unable to convert the string.
 * @return encoded string
 */
template <typename t_input, typename t_output>
inline t_output *cvt_utf_simple(const t_input *str, size_t len = (size_t)-1, size_t *s_out=NULL, unsigned int block_size=2048) {
	size_t s = block_size;

	//t_output *dst = (t_output *)malloc(sizeof(t_output) * s);
	t_output *dst = new t_output[s];

	size_t t_src = 0, t_dst = 0, p_src = 0, l = len;
	while(1) {
		errno = 0;
		t_dst += cvt_utf(str + t_src, dst + t_dst, s - t_dst, &p_src, l);
		t_src += p_src;
		if(l != (size_t)-1) l -= p_src;

		if(errno == 0) {
			if(l != (size_t)-1 && l) throw new std::runtime_error("unknown error occurred");
			break;
		}
		if(errno != ENOBUFS) throw new std::runtime_error("unable to convert the string");

		// this is why I love C more
		//s += block_size;
		//dst = (t_output *)realloc(dst, sizeof(t_output) * s);

		t_output *new_dst = new t_output[s + block_size];
		std::copy(dst, dst + s, new_dst);
		s += block_size;
		delete[] dst;
		dst = new_dst;
	}
	if(s_out != NULL) *s_out = t_dst;
	return dst;
}

}
#endif
