#include "utf.h"

#define _RET_OK         0    ///< function return without any errors
#define _RET_EOF        1    ///< reach the end of buffer
#define _RET_EILSEQ     2    ///< function meet corrupted character (only happens with ic::utf_fix::stop)
#define _RET_ENOBUFS    3    ///< no buffer space available to write

///@{
/**
 * @brief         core of UTF-8 decoder
 *
 * @param[in,out] str   the input string
 * @param         se    the end of input
 * @param[out]    w     read wide character
 * @param         f     fix mood
 * @param         l     length mood
 *
 * @return        error code
 * - _RET_OK
 * - _RET_EOF
 * - _RET_EILSEQ
 */
unsigned char _proto_utf8_dec(const char *&str, const char *se, wchar_t &w, enum ic::utf_fix f, bool l) {
	if((l && str >= se) || (!l && !*str)) return _RET_EOF;
	unsigned char d = 0;
	char c = *str;
	if((c & 0xf8) == 0xf0) {
		if(
			((l && str + 3 < se) || !l) &&
			(str[d = 1] & 0xC0) == 0x80 &&
			(str[d = 2] & 0xC0) == 0x80 &&
			(str[d = 3] & 0xC0) == 0x80
		) {
			w = (str[3] & 0x3F) | (str[2] << 6 & 0xFC0) | (str[1] << 12 & 0x3F000) | (c << 18 & 0x1C0000);
			str += 4;
			return _RET_OK;
		}
	}
	else if((c & 0xf0) == 0xe0) {
		if(
			((l && str + 2 < se) || !l) &&
			(str[d = 1] & 0xC0) == 0x80 &&
			(str[d = 2] & 0xC0) == 0x80
		) {
			w = (str[2] & 0x3F) | (str[1] << 6 & 0xFC0) | (c << 12 & 0xf000);
			str += 3;
			return _RET_OK;
		}
	}
	else if((c & 0xe0) == 0xc0) {
		if(
			((l && str + 1 < se) || !l) &&
			(str[d = 1] & 0xC0) == 0x80
		) {
			w = (*++str & 0x3F) | (c << 6 & 0x7c0);
			str++;
			return _RET_OK;
		}
	}
	else if((c & 0x80) == 0) {
		w = *str++;
		return _RET_OK;
	}
	if(f == ic::replace) w = 0xfffd;
	else if(f == ic::copy) w = c; // not finished
	else if(f == ic::stop) return _RET_EILSEQ;
	str += d;
	return _RET_OK;
}
/**
 * @brief       core of UTF-16 decoder
 * @copydetails _proto_utf8_dec()
 */
unsigned char _proto_utf16_dec(const char16_t *&str, const char16_t *se, wchar_t &w, enum ic::utf_fix f, bool l) {
	if((l && str >= se) || (!l && !*str)) return _RET_EOF;
	char16_t c = *str++;
	if((c & 0xF800) != 0xD800) {
		w = c;
		return _RET_OK;
	}
	if((!l || (l && str < se)) && (*str & 0xFC00) == 0xDC00) {
		w = ((c << 10 & 0xFFC00) | (*str++ & 0x03FF)) + 0x10000;
		return _RET_OK;
	}
	if(f == ic::replace) w = 0xfffd;
	else if(f == ic::copy) w = c;
	else if(f == ic::stop) return _RET_EILSEQ;
	return _RET_OK;
}
///@}

///@{
/**
 * @brief         core of UTF-8 encoder
 *
 * @param[in,out] dst   the destination
 * @param         de    the end of destination
 * @param         w     input character
 *
 * @return        error code
 * - _RET_OK
 * - _RET_ENOBUFS
 */
unsigned char _proto_utf8_enc(char *&dst, const char *de, wchar_t w) {
	if(dst >= de) return _RET_ENOBUFS;
	if(w < 0x0080) *dst++ = (char)w;
	else if(w < 0x0800) {
		if((dst + 1) >= de) return _RET_ENOBUFS;
		*dst++ = 0xC0 | (char)(w >> 6);
		*dst++ = 0x80 | (char)(w & 0x3F);
	}
	else if(w < 0x10000) {
		if((dst + 2) >= de) return _RET_ENOBUFS;
		*dst++ = 0xE0 | (char)(w >> 12);
		*dst++ = 0x80 | (char)((w >> 6) & 0x3F);
		*dst++ = 0x80 | (char)(w & 0x3F);
	}
	else {
		if((dst + 3) >= de) return _RET_ENOBUFS;
		*dst++ = 0xF0 | (char)(w >> 18);
		*dst++ = 0x80 | (char)((w >> 12) & 0x3F);
		*dst++ = 0x80 | (char)((w >> 6) & 0x3F);
		*dst++ = 0x80 | (char)(w & 0x3F);
	}
	return _RET_OK;
}
/**
 * @brief       core of UTF-16 encoder
 * @copydetails _proto_utf8_enc()
 */
unsigned char _proto_utf16_enc(char16_t *&dst, const char16_t *de, wchar_t w) {
	if(dst >= de) return _RET_ENOBUFS;
	if(w < 0x10000) *dst++ = w; // I won't block U+D800 to U+DFFF but they are reserved as surrogates; so this could provide corrupted output
	else {
		if((dst + 1) >= de) return _RET_ENOBUFS;
		*dst++ = 0xD800 | ((w - 0x10000) >> 10);
		*dst++ = 0xDC00 | (w & 0x03FF);
	}
	return _RET_OK;
}
///@}

size_t ic::wcs_to_utf8(const wchar_t *str, char *dst, size_t max, size_t *pos, size_t len) {
	const wchar_t *ss = str, *se = ss + len, *fa = ss;
	char *ds = dst, *de = ds + max;
	const bool l = len != (size_t)-1; // length mode or null-terminated mode
	unsigned char r;
	while((l && str < se) || (!l && *str)) {
		fa = str;
		if((r = _proto_utf8_enc(dst, de, *str++)) == _RET_ENOBUFS) goto err;
	}
	if(dst < de) *dst = 0;
	if(pos != NULL) *pos = str - ss;
	return dst - ds;
	err:
	errno = ENOBUFS;
	if(pos != NULL) *pos = fa - ss;
	return dst - ds;
}
size_t ic::wcs_to_utf16(const wchar_t *str, char16_t *dst, size_t max, size_t *pos, size_t len) {
	const wchar_t *ss = str, *se = ss + len, *fa = ss;
	char16_t *ds = dst, *de = ds + max;
	const bool l = len != (size_t)-1; // length mode or null-terminated mode
	unsigned char r;
	while((l && str < se) || (!l && *str)) {
		fa = str;
		if((r = _proto_utf16_enc(dst, de, *str++)) == _RET_ENOBUFS) goto err;
	}
	if(dst < de) *dst = 0;
	if(pos != NULL) *pos = str - ss;
	return dst - ds;
	err:
	errno = ENOBUFS;
	if(pos != NULL) *pos = fa - ss;
	return dst - ds;
}
size_t ic::utf8_to_wcs(const char *str, wchar_t *dst, size_t max, size_t *pos, enum utf_fix f, size_t len) {
	const char *ss = str, *se = ss + len, *fa = ss;
	wchar_t *ds = dst, *de = ds + max;
	const bool l = len != (size_t)-1; // length mode or null-terminated mode
	unsigned char e = 0, r;
	wchar_t w = 0;
	while((r = _proto_utf8_dec(str, se, w, f, l)) != _RET_EOF) {
		if(r != _RET_OK) {
			e = r == _RET_EILSEQ;
			goto err;
		}
		if(dst >= de) goto err;
		*dst++ = w;
		fa = str;
	}
	if(dst < de) *dst = 0;
	if(pos != NULL) *pos = str - ss;
	return dst - ds;
	err:
	errno = e ? EILSEQ : ENOBUFS;
	if(pos != NULL) *pos = fa - ss;
	return dst - ds;
}
size_t ic::utf16_to_wcs(const char16_t *str, wchar_t *dst, size_t max, size_t *pos, enum utf_fix f, size_t len) {
	const char16_t *ss = str, *se = ss + len, *fa = ss;
	wchar_t *ds = dst, *de = ds + max;
	const bool l = len != (size_t)-1; // length mode or null-terminated mode
	unsigned char e = 0, r;
	wchar_t w = 0;
	while((r = _proto_utf16_dec(str, se, w, f, l)) != _RET_EOF) {
		if(r != _RET_OK) {
			e = r == _RET_EILSEQ;
			goto err;
		}
		if(dst >= de) goto err;
		*dst++ = w;
		fa = str;
	}
	if(dst < de) *dst = 0;
	if(pos != NULL) *pos = str - ss;
	return dst - ds;
	err:
	errno = e ? EILSEQ : ENOBUFS;
	if(pos != NULL) *pos = fa - ss;
	return dst - ds;
}
size_t ic::utf8_to_utf16(const char *str, char16_t *dst, size_t max, size_t *pos, enum utf_fix f, size_t len) {
	const char *ss = str, *se = ss + len, *fa = ss;
	char16_t *ds = dst, *de = ds + max;
	const bool l = len != (size_t)-1; // length mode or null-terminated mode
	unsigned char e = 0, r;
	wchar_t w = 0;
	while((r = _proto_utf8_dec(str, se, w, f, l)) != _RET_EOF) {
		if(r != _RET_OK) {
			e = r == _RET_EILSEQ;
			goto err;
		}
		if((r = _proto_utf16_enc(dst, de, w)) == _RET_ENOBUFS) goto err;
		fa = str;
	}
	if(dst < de) *dst = 0;
	if(pos != NULL) *pos = str - ss;
	return dst - ds;
	err:
	errno = e ? EILSEQ : ENOBUFS;
	if(pos != NULL) *pos = fa - ss;
	return dst - ds;
}
size_t ic::utf16_to_utf8(const char16_t *str, char *dst, size_t max, size_t *pos, enum utf_fix f, size_t len) {
	const char16_t *ss = str, *se = ss + len, *fa = ss;
	char *ds = dst, *de = ds + max;
	const bool l = len != (size_t)-1; // length mode or null-terminated mode
	unsigned char e = 0, r;
	wchar_t w = 0;
	while((r = _proto_utf16_dec(str, se, w, f, l)) != _RET_EOF) {
		if(r != _RET_OK) {
			e = r == _RET_EILSEQ;
			goto err;
		}
		if((r = _proto_utf8_enc(dst, de, w)) == _RET_ENOBUFS) goto err;
		fa = str;
	}
	if(dst < de) *dst = 0;
	if(pos != NULL) *pos = str - ss;
	return dst - ds;
	err:
	errno = e ? EILSEQ : ENOBUFS;
	if(pos != NULL) *pos = fa - ss;
	return dst - ds;
}

size_t ic::utf8_len(const char *str, size_t len) {
	const char *se = str + len;
	const bool l = len != (size_t)-1; // length mode or null-terminated mode
	wchar_t w = 0;
	unsigned char r;
	size_t c = 0;
	while((r = _proto_utf8_dec(str, se, w, ic::replace, l)) != _RET_EOF) {
		if(r != _RET_OK) {
			errno = EILSEQ;
			return -1;
		}
		c++;
	}
	return c;
}
size_t ic::utf16_len(const char16_t *str, size_t len) {
	const char16_t *se = str + len;
	const bool l = len != (size_t)-1; // length mode or null-terminated mode
	wchar_t w = 0;
	unsigned char r;
	size_t c = 0;
	while((r = _proto_utf16_dec(str, se, w, ic::replace, l)) != _RET_EOF) {
		if(r != _RET_OK) {
			errno = EILSEQ;
			return -1;
		}
		c++;
	}
	return c;
}
wchar_t ic::utf8_get_char(const char *str, const char **nxt, enum utf_fix f, size_t len) {
	const char *se = str + len;
	const bool l = len != (size_t)-1; // length mode or null-terminated mode
	wchar_t w = 0;
	unsigned char r = _proto_utf8_dec(str, se, w, f, l);
	if(nxt != NULL) *nxt = str;
	if(r == _RET_OK) return w;
	errno = r == _RET_EOF ? ENODATA : EILSEQ;
	return 0;
}
wchar_t ic::utf16_get_char(const char16_t *str, const char16_t **nxt, enum utf_fix f, size_t len) {
	const char16_t *se = str + len;
	const bool l = len != (size_t)-1; // length mode or null-terminated mode
	wchar_t w = 0;
	unsigned char r = _proto_utf16_dec(str, se, w, f, l);
	if(nxt != NULL) *nxt = str;
	if(r == _RET_OK) return w;
	errno = r == _RET_EOF ? ENODATA : EILSEQ;
	return 0;
}
size_t ic::utf8_put_char(char *dst, size_t max, wchar_t c) {
	char *ds = dst, *de = ds + max;
	unsigned char r = _proto_utf8_enc(dst, de, c);
	if(r == _RET_ENOBUFS) {
		errno = ENOBUFS;
		return 0;
	}
	return dst - ds;
}
size_t ic::utf16_put_char(char16_t *dst, size_t max, wchar_t c) {
	char16_t *ds = dst, *de = ds + max;
	unsigned char r = _proto_utf16_enc(dst, de, c);
	if(r == _RET_ENOBUFS) {
		errno = ENOBUFS;
		return 0;
	}
	return dst - ds;
}
