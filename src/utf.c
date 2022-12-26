#include "utf.h"

size_t wcs_to_utf8(wchar_t *str, char *dst, size_t max, size_t *pos, size_t len) {
	size_t _p = 0;
	if(pos == NULL) pos = &_p;
	size_t sp = 0, dp = *pos;
	wchar_t c;
	char l = len != (size_t)-1; // length mode or null-terminated mode
	while(1) {
		if(l && sp >= len) break;
		if(!l && !str[sp]) break;
		if(dp >= max) break;
		c = str[sp];
		if(c < 0x0080) dst[dp++] = (char)c;
		else if(c < 0x0800) {
			if((dp + 2) >= max) break;
			dst[dp++] = 0xC0 | (char)(c >> 6);
			dst[dp++] = 0x80 | (char)(c & 0x3F);
		}
		else if(c < 0x10000) {
			if((dp + 3) >= max) break;
			dst[dp++] = 0xE0 | (char)(c >> 12);
			dst[dp++] = 0x80 | (char)((c >> 6) & 0x3F);
			dst[dp++] = 0x80 | (char)(c & 0x3F);
		}
		else {
			if((dp + 4) >= max) break;
			dst[dp++] = 0xF0 | (char)(c >> 18);
			dst[dp++] = 0x80 | (char)((c >> 12) & 0x3F);
			dst[dp++] = 0x80 | (char)((c >> 6) & 0x3F);
			dst[dp++] = 0x80 | (char)(c & 0x3F);
		}
		sp++;
	}
	*pos = dp;
	return sp;
}
size_t utf8_to_wcs(char *str, wchar_t *dst, size_t max, size_t *pos, enum utf_fix f, size_t len) {
	size_t _p = 0;
	if(pos == NULL) pos = &_p;
	size_t sp = 0, dp = *pos;
	char c, d;
	char l = len != (size_t)-1; // length mode or null-terminated mode
	while(1) {
		if(l && sp >= len) break;
		if(!l && !str[sp]) break;
		if(dp >= max) break;
		c = str[sp];
		if((c & 0xf8) == 0xf0) {
			if(
				(str[sp + (d = 1)] & 0xC0) == 0x80 &&
				(str[sp + (d = 2)] & 0xC0) == 0x80 &&
				(str[sp + (d = 3)] & 0xC0) == 0x80
			) {
				dst[dp++] = (str[sp + 3] & 0x3F) | (str[sp + 2] << 6 & 0xFC0) | (str[sp + 1] << 12 & 0x3F000) | (str[sp] << 18 & 0x1C0000);
				sp += 4;
				continue;
			}
		}
		else if((c & 0xf0) == 0xe0) {
			if(
				(str[sp + (d = 1)] & 0xC0) == 0x80 &&
				(str[sp + (d = 2)] & 0xC0) == 0x80
			) {
				dst[dp++] = (str[sp + 2] & 0x3F) | (str[sp + 1] << 6 & 0xFC0) | (str[sp] << 12 & 0xf000);
				sp += 3;
				continue;
			}
		}
		else if((c & 0xe0) == 0xc0) {
			if((str[sp + 1] & 0xC0) == 0x80) {
				dst[dp++] = (str[sp + 1] & 0x3F) | (str[sp] << 6 & 0x7c0);
				sp += 2;
				continue;
			}
			else d = 1;
		}
		else if((c & 0x80) == 0) {
			dst[dp++] = str[sp++];
			continue;
		}
		else d = 1;
		if(f == chr_replace) dst[dp++] = 0xfffd;
		else if(f == chr_copy) {
			dst[dp++] = str[sp];
			// not finished
		}
		else if(f == chr_stop) break;
		sp += d;
	}
	*pos = dp;
	return sp;
}
size_t wcs_to_utf16(wchar_t *str, char16_t *dst, size_t max, size_t *pos, size_t len) {
	wchar_t c;
	wchar_t *ss = str, *se = ss + len;
	char16_t *ds = dst, *de = ds + max;
	char l = len != (size_t)-1; // length mode or null-terminated mode
	while(((l && str < se) || (!l && *str)) && (dst < de)) {
		c = *str;
		if(c < 0x10000) *dst++ = c; // I won't block U+D800 to U+DFFF but they are reserved as surrogates; so this could provide corrupted output
		else {
			if((dst + 2) >= de) break;
			*dst++ = 0xD800 | ((c - 0x10000) >> 10);
			*dst++ = 0xDC00 | (c & 0x03FF);
		}
		str++;
	}
	if(!l) *dst = 0;
	if(pos != NULL) *pos = dst - ds;
	return str - ss;
}
size_t utf16_to_wcs(char16_t *str, wchar_t *dst, size_t max, size_t *pos, enum utf_fix f, size_t len) {
	char16_t c;
	char16_t *ss = str, *se = ss + len;
	wchar_t *ds = dst, *de = ds + max;
	char l = len != (size_t)-1; // length mode or null-terminated mode
	while(((l && str < se) || (!l && *str)) && (dst < de)) {
		c = *str++;
		if((c & 0xF800) != 0xD800) {
			*dst++ = c;
			continue;
		}
		if(str < se && (*str & 0xFC00) == 0xDC00) {
			*dst++ = ((c << 10 & 0xFFC00) | (*str++ & 0x03FF)) + 0x10000;
			continue;
		}
		if(f == chr_replace) *dst++ = 0xfffd;
		else if(f == chr_copy) *dst++ = c;
		else if(f == chr_stop) break;
	}
	if(!l) *dst = 0;
	if(pos != NULL) *pos = dst - ds;
	return str - ss;
}
