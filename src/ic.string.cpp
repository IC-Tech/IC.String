#include <cstdlib>
#include <cstring>
#include <wchar.h>
#include <cstdio>
#include "ic.string.h"
#include "utf8.h"

namespace IC {
	String::String(wchar_t *ca) : String(ca, wcslen(ca)) {}
	String::String(char c): String(wchar_t(c)) {}
	String::String(const wchar_t* cca) : String((wchar_t*)cca) {}
	String::String(char *ca) : String(ca, strlen(ca)) {}
	void String::add(char c) { return add(wchar_t(c)); }
	void String::add(char* ca) { return add(ca, strlen(ca)); }
	void String::add(wchar_t* ca) { return add(ca, wcslen(ca)); }
	void String::insert(unsigned int pos, wchar_t *ca) { return insert(pos, ca, wcslen(ca)); }
	int String::indexOf(wchar_t *ca, unsigned int pos) { return indexOf(ca, wcslen(ca), pos); }
	int String::indexOf(const wchar_t *cca, unsigned int pos) { return indexOf((wchar_t*)cca, wcslen(cca), pos); }
	void String::push_back(wchar_t c) { return add(c); }
	void String::insert(unsigned int pos, const wchar_t *ca) { return insert(pos, (wchar_t*)ca); }
	String::operator char*() { return c_str(); }
	String::operator wchar_t*() { return wc_str(); }
	String::operator void*() { return ptr(); }
	String::operator const wchar_t*() { return (const wchar_t*)wc_str(); }
	bool String::operator!=(const wchar_t* str) { return !equal(str); }
	bool String::operator==(const wchar_t* str) { return equal(str); }
	void *String::ptr() { return (void*)buff; }
	bool String::startWith(const wchar_t* cca) { return startWith((wchar_t*)cca, wcslen(cca)); }
	bool String::startWith(wchar_t* ca) { return startWith(ca, wcslen(ca)); }
	bool String::startWith(String& str) { return startWith(str.buff, str.len); }
	bool String::endWith(const wchar_t* cca) { return endWith((wchar_t*)cca, wcslen(cca)); }
	bool String::endWith(wchar_t* ca) { return endWith(ca, wcslen(ca)); }
	bool String::endWith(String& str) { return endWith(str.buff, str.len); }
	#define e___ buff[len] = 0;
	#define _malloc(x) malloc((x) * sizeof(wchar_t))
	#define _realloc(x, y) realloc((void *)(x), (y) * sizeof(wchar_t))
	//void *_malloc(unsigned int x) { return malloc(x * sizeof(wchar_t)); }
	//void *_realloc(wchar_t *x, unsigned int y) { return realloc((void *)x, y * sizeof(wchar_t)); }

	String::String() {
		len = 0;
		buff = (wchar_t*)_malloc(siz = block);
		e___
	}
	String::~String() {
		siz = len = 0;
		free(buff);
	}
	String::String(wchar_t *bu, unsigned int le, unsigned int si) {
		buff = bu;
		len = le;
		siz = si;
	}
	String::String(wchar_t* ca, unsigned int leng) {
		len = leng;
		buff = (wchar_t*)_malloc(siz = len + block);
		wmemcpy(buff, ca, len);
		e___
	}
	String::String(bool con) {
		buff = (wchar_t*)_malloc(siz = 5 + block);
		if(con == true) wmemcpy(buff, L"true", len = 4);
		else wmemcpy(buff, L"false", len = 5);
		e___
	}
	String::String(wchar_t c) {
		buff = (wchar_t*)_malloc(siz = block);
		buff[0] = c;
		buff[len = 1] = 0;
	}
	String::String(int inte) {
		buff = (wchar_t*)_malloc(siz = block);
		len = swprintf(buff, siz, L"%d", inte);
		e___
	}
	String::String(char *ca, unsigned int leng): String() {
		add(ca, leng);
	}
	unsigned int String::length() {
		return len;
	}
	wchar_t* String::wc_str() {
#ifdef NO_IC_STR_SAME_OUT
		wchar_t* ptr = (wchar_t*)_malloc(len + 1);
		wmemcpy(ptr, buff, len);
		ptr[len] = 0;
		return ptr;
#else
		return buff;
#endif
	}
	bool String::startWith(wchar_t *ca, unsigned int leng) {
		if(len < leng) return false;
		return wmemcmp(buff, ca, leng) == 0;
	}
	bool String::endWith(wchar_t *ca, unsigned int leng) {
		if(len < leng) return false;
		for(int i=leng - 1, s = len - leng; i >= 0; i--) if(buff[s + i] != ca[i]) return false;
		return true;
	}
	void String::remove(unsigned int pos) {
		if(pos >= len) return;
		buff[len = pos] = 0;
#ifndef NO_IC_STR_REDUCE
		reduce();
#endif
#ifndef NO_IC_STR_UTF8_CACHE
		buff_mod = true;
#endif
	}
	void String::remove(unsigned int pos, unsigned int leng) {
		if(!len || pos >= len) return;
		unsigned int e = pos + leng;
		if(e >= len) return remove(pos);
		wmemcpy(buff + pos, buff + e, len - e);
		len -= leng;
		e___
#ifndef NO_IC_STR_REDUCE
		reduce();
#endif
#ifndef NO_IC_STR_UTF8_CACHE
		buff_mod = true;
#endif
	}
	String String::substring(unsigned int pos, unsigned int leng) {
		if(!len || !leng || pos >= len) return String();
		unsigned int s = leng + block;
		wchar_t *b = (wchar_t *)_malloc(s);
		if((pos + leng) > len) leng = len - pos;
		wmemcpy(b, buff + pos, leng);
		return String(b, leng, s);
	}
	int String::indexOf(wchar_t c, unsigned int pos) {
		while(pos < len) {
			if(buff[pos] == c) return pos;
			pos++;
		}
		return -1;
	}
	int String::toInt(unsigned int pos) {
		wchar_t *end;
		return wcstol(buff + pos, &end, 10);
	}



	unsigned int String::size() {
		return siz;
	}
	void String::resize_(unsigned int s) {
		buff = (wchar_t*)_realloc(buff, siz = s);
		if(len > s) len = s;
	}
	void String::self_substring(unsigned int pos, unsigned int leng) {
		if(!len || !leng || pos >= len) return;
		if((pos + leng) > len) len = leng = len - pos;
		else len = leng;
		wmemcpy(buff, buff + pos, leng);
		e___
#ifndef NO_IC_STR_REDUCE
		reduce();
#endif
#ifndef NO_IC_STR_UTF8_CACHE
		buff_mod = true;
#endif
	}
	void String::pop_back() {
		if(len) buff[--len] = 0;
#ifndef NO_IC_STR_REDUCE
		reduce();
#endif
#ifndef NO_IC_STR_UTF8_CACHE
		buff_mod = true;
#endif
	}
	void String::clear() {
		buff[len = 0] = 0;
#ifndef NO_IC_STR_REDUCE
		reduce();
#endif
#ifndef NO_IC_STR_UTF8_CACHE
		buff_mod = true;
#endif
	}
	void String::insert(unsigned int pos, wchar_t c) {
		if(!len || pos >= (len -1)) return add(c);
		if(len >= siz) resize_(siz + 1 + block);
		wmemcpy(buff + pos + 1, buff + pos, len - pos);
		buff[pos] = c;
		buff[len++] = 0;
#ifndef NO_IC_STR_UTF8_CACHE
		buff_mod = true;
#endif
	}
	void String::insert(unsigned int pos, wchar_t *ca, unsigned int leng) {
		if(!leng) return;
		if(!len || pos >= (len -1)) return add(ca);
		if((len + leng) >= siz) resize_(siz + leng + block);
		wmemcpy(buff + pos + leng, buff + pos, len - pos);
		wmemcpy(buff + pos, ca, leng);
		len += leng;
		e___
#ifndef NO_IC_STR_UTF8_CACHE
		buff_mod = true;
#endif
	}
	bool String::equal(wchar_t *ca, unsigned int leng) {
		if(leng != len) return true;
		return wmemcmp(ca, buff, len) == 0;
	}
	bool String::equal(wchar_t *ca) {
		return equal(ca, wcslen(ca));
	}
	bool String::equal(const wchar_t *ca) {
		return equal((wchar_t*)ca);
	}
	bool String::equal(const wchar_t *ca, unsigned int leng) {
		return equal((wchar_t*)ca, leng);
	}
#ifndef NO_IC_STR_REDUCE
	void String::reduce() {
		if(!auto_size_reduce) return;
		if(siz - len < (block * 2)) return;
		buff = (wchar_t*)_realloc(buff, siz = len + block * 2);
	}
#endif
	void String::resize(unsigned int s) {
		if(siz <= s) resize_(s + block);
#ifndef NO_IC_STR_UTF8_CACHE
		if(len > s) buff_mod = true;
#endif
		buff[len = s] = 0;
#ifndef NO_IC_STR_REDUCE
		reduce();
#endif
	}
	wchar_t String::charAt(unsigned int pos) {
		return len <= pos ? 0 : buff[pos];
	}
	void String::setChar(wchar_t c, unsigned int pos) {
		if(len <= pos) return;
		buff[pos] = c;
#ifndef NO_IC_STR_UTF8_CACHE
		buff_mod = true;
#endif
	}

	void String::add(wchar_t c) {
		if(len >= siz) resize_(siz + 1 + block);
		buff[len++] = c;
		e___
#ifndef NO_IC_STR_UTF8_CACHE
		buff_mod = true;
#endif
	}
	void String::add(wchar_t* ca, unsigned int leng) {
		if(!leng) return;
		if((len + leng) >= siz) resize_(siz + leng + block);
		wmemcpy(buff + len, ca, leng);
		len += leng;
		e___
#ifndef NO_IC_STR_UTF8_CACHE
		buff_mod = true;
#endif
	}
	void String::add(char *ca, unsigned int leng) {
		size_t a = len, b = 0, c = 0;
		while(1) {
			b += c = utf8towcs(ca + b, buff, siz, &a, replace, leng - b);
			if(!c) {
				if(b >= leng) break;
				if((siz - a) > block)
					break; // unknown error
				resize_(siz + block);
			}
		}
		len = a;
		e___
#ifndef NO_IC_STR_UTF8_CACHE
		buff_mod = true;
#endif
	}
	String& String::operator+=(char c) {
		add(c);
		return *this;
	}
	String& String::operator+=(char *ca) {
		add(ca);
		return *this;
	}
	String& String::operator+=(const char *ca) {
		add((char*)ca);
		return *this;
	}
	String& String::operator+=(wchar_t c) {
		add(c);
		return *this;
	}
	String& String::operator+=(wchar_t *ca) {
		add(ca);
		return *this;
	}
	String& String::operator+=(const wchar_t *cca) {
		add((wchar_t*)cca);
		return *this;
	}
	String& String::operator+=(String &str) {
		add(str.buff, str.len);
		return *this;
	}
	String String::operator+(const String& str) const {
		unsigned int l = len + str.len, s;
		wchar_t *b = (wchar_t *)_malloc(s = l + IC_STR_BLOCK);
		wmemcpy(b, buff, len);
		wmemcpy(b + len, str.buff, str.len);
		return String(b, l, s);
	}
	String& String::operator=(const String& str) {
		bool t;
		if((t = str.len >= siz)) resize_(str.len + block);
		wmemcpy(buff, str.buff, len = str.len);
#ifndef NO_IC_STR_REDUCE
		if(!t) reduce();
#endif
#ifndef NO_IC_STR_UTF8_CACHE
		buff_mod = true;
#endif
		return *this;
	}
	int String::indexOf(wchar_t *ca, unsigned int leng, unsigned int pos) {
		if(!len || len < leng) return -1;
		if(len == leng) return equal(ca, leng);
		unsigned int p = pos-1, i = 0;
		while(++p < len) {
			if(buff[p] != ca[0]) continue;
			for(i = 1; i < leng; i++) {
				if(buff[p + i] != ca[i]) {
					i = 0;
					break;
				}
			}
			if(i) return p;
		}
		return -1;
	}

#ifdef NO_IC_STR_UTF8_CACHE
	char* String::c_str(unsigned int *leng) {
		unsigned int utf_siz = block;
		char *utf_buff = (char*)malloc(utf_siz);
#else
	char* String::c_str(unsigned int *leng, bool force) {
#endif
		if(!utf_siz) utf_buff = (char*)malloc(utf_siz = block);
		size_t a = 0, b = 0, c = 0;
#ifndef NO_IC_STR_UTF8_CACHE
		if(force || buff_mod) {
#endif
			while (1) {
				c += b = wcstoutf8(buff + c, utf_buff, utf_siz, &a, len - c);
				if(!b) {
					if(c >= len) break;
					if((utf_siz - a) > block)
						break; // unknown error
					utf_buff = (char*)realloc(utf_buff, utf_siz = utf_siz + block);
				}
			}
			if(auto_utf_size_reduce && (utf_siz - a) > (block * 2)) utf_buff = (char*)realloc(utf_buff, utf_siz = a + block * 2);
#ifndef NO_IC_STR_UTF8_CACHE
			buff_mod = false;
			utf_len = a;
		}
		else a = utf_len;
#endif
		utf_buff[a] = 0;
		if(leng != nullptr) *leng = a;
#ifdef NO_IC_STR_SAME_OUT
		char *ptr = (char*)malloc(a + 1);
		memcpy(ptr, utf_buff, a + 1);
		return ptr;
#else
		return utf_buff;
#endif
	}
}
