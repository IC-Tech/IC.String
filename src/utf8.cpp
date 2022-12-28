#include "utf8.h"

size_t wcstoutf8(wchar_t *str, char *dst, size_t max, size_t *pos, size_t len) {
	size_t _p = 0;
	if(pos == NULL) pos = &_p;
	size_t sp = 0, dp = *pos;
	wchar_t c;
	bool l = len != (size_t)-1; // length mode or null-terminated mode
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
size_t utf8towcs(char *str, wchar_t *dst, size_t max, size_t *pos, ic::utf_fix f, size_t len) {
	size_t _p = 0;
	if(pos == NULL) pos = &_p;
	size_t sp = 0, dp = *pos;
	char c, d;
	bool l = len != (size_t)-1; // length mode or null-terminated mode
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
		if(f == ic::replace) dst[dp++] = 0xfffd;
		else if(f == ic::copy) {
			dst[dp++] = str[sp];
			// not finished
		}
		else if(f == ic::stop) break;
		sp += d;
	}
	*pos = dp;
	return sp;
}
