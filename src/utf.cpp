#include "utf.h"

size_t ic::wcs_to_utf8(const wchar_t *str, char *dst, size_t max, size_t *pos, size_t len) {
	const wchar_t *ss = str, *se = ss + len, *fa = ss;
	wchar_t c;
	char *ds = dst, *de = ds + max;
	char l = len != (size_t)-1; // length mode or null-terminated mode
	while((l && str < se) || (!l && *str)) {
		if(dst >= de) goto err;
		fa = str;
		c = *str;
		if(c < 0x0080) *dst++ = (char)c;
		else if(c < 0x0800) {
			if((dst + 2) >= de) goto err;
			*dst++ = 0xC0 | (char)(c >> 6);
			*dst++ = 0x80 | (char)(c & 0x3F);
		}
		else if(c < 0x10000) {
			if((dst + 3) >= de) goto err;
			*dst++ = 0xE0 | (char)(c >> 12);
			*dst++ = 0x80 | (char)((c >> 6) & 0x3F);
			*dst++ = 0x80 | (char)(c & 0x3F);
		}
		else {
			if((dst + 4) >= de) goto err;
			*dst++ = 0xF0 | (char)(c >> 18);
			*dst++ = 0x80 | (char)((c >> 12) & 0x3F);
			*dst++ = 0x80 | (char)((c >> 6) & 0x3F);
			*dst++ = 0x80 | (char)(c & 0x3F);
		}
		str++;
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
	char c;
	wchar_t *ds = dst, *de = ds + max;
	char l = len != (size_t)-1; // length mode or null-terminated mode
	unsigned char d = 0, e = 0;
	while((l && str < se) || (!l && *str)) {
		if(dst >= de) goto err;
		fa = str;
		c = *str;
		if((c & 0xf8) == 0xf0) {
			if(l && str + 3 >= se) goto err;
			if(
				(str[d = 1] & 0xC0) == 0x80 &&
				(str[d = 2] & 0xC0) == 0x80 &&
				(str[d = 3] & 0xC0) == 0x80
			) {
				*dst++ = (str[3] & 0x3F) | (str[2] << 6 & 0xFC0) | (str[1] << 12 & 0x3F000) | (c << 18 & 0x1C0000);
				str += 4;
				continue;
			}
		}
		else if((c & 0xf0) == 0xe0) {
			if(l && str + 2 >= se) goto err;
			if(
				(str[d = 1] & 0xC0) == 0x80 &&
				(str[d = 2] & 0xC0) == 0x80
			) {
				*dst++ = (str[2] & 0x3F) | (str[1] << 6 & 0xFC0) | (c << 12 & 0xf000);
				str += 3;
				continue;
			}
		}
		else if((c & 0xe0) == 0xc0) {
			if(l && str + 1 >= se) goto err;
			if((str[d = 1] & 0xC0) == 0x80) {
				*dst++ = (*++str & 0x3F) | (c << 6 & 0x7c0);
				str++;
				continue;
			}
		}
		else if((c & 0x80) == 0) {
			*dst++ = *str++;
			continue;
		}
		if(f == ic::replace) *dst++ = 0xfffd;
		else if(f == ic::copy) *dst++ = c; // not finished
		else if((e = f == ic::stop)) goto err;
		str += d;
	}
	if(dst < de) *dst = 0;
	if(pos != NULL) *pos = str - ss;
	return dst - ds;
	err:
	errno = e ? EILSEQ : ENOBUFS;
	if(pos != NULL) *pos = fa - ss;
	return dst - ds;
}
size_t ic::wcs_to_utf16(const wchar_t *str, char16_t *dst, size_t max, size_t *pos, size_t len) {
	const wchar_t *ss = str, *se = ss + len, *fa = ss;
	wchar_t c;
	char16_t *ds = dst, *de = ds + max;
	char l = len != (size_t)-1; // length mode or null-terminated mode
	while((l && str < se) || (!l && *str)) {
		if(dst >= de) goto err;
		fa = str;
		c = *str;
		if(c < 0x10000) *dst++ = c; // I won't block U+D800 to U+DFFF but they are reserved as surrogates; so this could provide corrupted output
		else {
			if((dst + 2) >= de) goto err;
			*dst++ = 0xD800 | ((c - 0x10000) >> 10);
			*dst++ = 0xDC00 | (c & 0x03FF);
		}
		str++;
	}
	if(dst < de) *dst = 0;
	if(pos != NULL) *pos = str - ss;
	return dst - ds;
	err:
	errno = ENOBUFS;
	if(pos != NULL) *pos = fa - ss;
	return dst - ds;
}
size_t ic::utf16_to_wcs(const char16_t *str, wchar_t *dst, size_t max, size_t *pos, enum utf_fix f, size_t len) {
	const char16_t *ss = str, *se = ss + len, *fa = ss;
	char16_t c;
	wchar_t *ds = dst, *de = ds + max;
	char l = len != (size_t)-1; // length mode or null-terminated mode
	while((l && str < se) || (!l && *str)) {
		if(dst >= de) goto err;
		fa = str;
		c = *str++;
		if((c & 0xF800) != 0xD800) {
			*dst++ = c;
			continue;
		}
		if((!l || (l && str < se)) && (*str & 0xFC00) == 0xDC00) {
			*dst++ = ((c << 10 & 0xFFC00) | (*str++ & 0x03FF)) + 0x10000;
			continue;
		}
		if(f == ic::replace) *dst++ = 0xfffd;
		else if(f == ic::copy) *dst++ = c;
		else if(f == ic::stop) goto err;
	}
	if(dst < de) *dst = 0;
	if(pos != NULL) *pos = str - ss;
	return dst - ds;
	err:
	if(pos != NULL) *pos = fa - ss;
	return dst - ds;
}
size_t ic::utf8_to_utf16(const char *str, char16_t *dst, size_t max, size_t *pos, enum utf_fix f, size_t len) {
	const char *ss = str, *se = ss + len, *fa = ss;
	char c;
	char16_t *ds = dst, *de = ds + max;
	char l = len != (size_t)-1; // length mode or null-terminated mode
	unsigned char d = 0, e = 0;
	wchar_t w = 0;
	while((l && str < se) || (!l && *str)) {
		if(dst >= de) goto err;
		fa = str;
		c = *str;
		if((c & 0xf8) == 0xf0) {
			if(l && str + 3 >= se) goto err;
			if(
				(str[d = 1] & 0xC0) == 0x80 &&
				(str[d = 2] & 0xC0) == 0x80 &&
				(str[d = 3] & 0xC0) == 0x80
			) {
				w = (str[3] & 0x3F) | (str[2] << 6 & 0xFC0) | (str[1] << 12 & 0x3F000) | (c << 18 & 0x1C0000);
				str += 4;
				goto push_chr;
			}
		}
		else if((c & 0xf0) == 0xe0) {
			if(l && str + 2 >= se) goto err;
			if(
				(str[d = 1] & 0xC0) == 0x80 &&
				(str[d = 2] & 0xC0) == 0x80
			) {
				w = (str[2] & 0x3F) | (str[1] << 6 & 0xFC0) | (c << 12 & 0xf000);
				str += 3;
				goto push_chr;
			}
		}
		else if((c & 0xe0) == 0xc0) {
			if(l && str + 1 >= se) goto err;
			if((str[d = 1] & 0xC0) == 0x80) {
				w = (*++str & 0x3F) | (c << 6 & 0x7c0);
				str++;
				goto push_chr;
			}
		}
		else if((c & 0x80) == 0) {
			w = *str++;
			goto push_chr;
		}
		if(f == ic::replace) w = 0xfffd;
		else if(f == ic::copy) w = c; // not finished
		else if((e = f == ic::stop)) goto err;
		str += d;

		push_chr:
		if(w < 0x10000) *dst++ = w; // I won't block U+D800 to U+DFFF but they are reserved as surrogates; so this could provide corrupted output
		else {
			if((dst + 2) >= de) goto err;
			*dst++ = 0xD800 | ((w - 0x10000) >> 10);
			*dst++ = 0xDC00 | (w & 0x03FF);
		}
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
	char16_t c;
	char *ds = dst, *de = ds + max;
	char l = len != (size_t)-1; // length mode or null-terminated mode
	wchar_t w = 0;
	unsigned char e = 0;
	while((l && str < se) || (!l && *str)) {
		if(dst >= de) goto err;
		fa = str;
		c = *str++;
		if((c & 0xF800) != 0xD800) {
			w = c;
			goto push_chr;
		}
		if((!l || (l && str < se)) && (*str & 0xFC00) == 0xDC00) {
			w = ((c << 10 & 0xFFC00) | (*str++ & 0x03FF)) + 0x10000;
			goto push_chr;
		}
		if(f == ic::replace) w = 0xfffd;
		else if(f == ic::copy) w = c;
		else if((e = f == ic::stop)) goto err;
		push_chr:
		if(w < 0x0080) *dst++ = (char)c;
		else if(w < 0x0800) {
			if((dst + 2) >= de) goto err;
			*dst++ = 0xC0 | (char)(w >> 6);
			*dst++ = 0x80 | (char)(w & 0x3F);
		}
		else if(w < 0x10000) {
			if((dst + 3) >= de) goto err;
			*dst++ = 0xE0 | (char)(w >> 12);
			*dst++ = 0x80 | (char)((w >> 6) & 0x3F);
			*dst++ = 0x80 | (char)(w & 0x3F);
		}
		else {
			if((dst + 4) >= de) goto err;
			*dst++ = 0xF0 | (char)(w >> 18);
			*dst++ = 0x80 | (char)((w >> 12) & 0x3F);
			*dst++ = 0x80 | (char)((w >> 6) & 0x3F);
			*dst++ = 0x80 | (char)(w & 0x3F);
		}
	}
	if(dst < de) *dst = 0;
	if(pos != NULL) *pos = str - ss;
	return dst - ds;
	err:
	errno = e ? EILSEQ : ENOBUFS;
	if(pos != NULL) *pos = fa - ss;
	return dst - ds;
}
