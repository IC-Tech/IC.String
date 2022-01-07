#include <cstdlib>
#include <cstring>
#include <cstdio>
#include "ic.string.h"

namespace IC {
	String::String(char *ca) : String(ca, strlen(ca)) {}
	String::String(const char* cca) : String((char*)cca) {}
	void String::add(char* ca) { return add(ca, strlen(ca)); }
	void String::insert(unsigned int pos, char *ca) { return insert(pos, ca, strlen(ca)); }
	int String::indexOf(char *ca, unsigned int pos) { return indexOf(ca, strlen(ca), pos); }
	int String::indexOf(const char *cca, unsigned int pos) { return indexOf((char*)cca, strlen(cca), pos); }
	void String::push_back(char c) { return add(c); }
	void String::insert(unsigned int pos, const char *ca) { return insert(pos, (char*)ca); }
	String::operator char*() { return c_str(); }
	String::operator void*() { return ptr(); }
	String::operator const char*() { return (const char*)c_str(); }
	bool String::operator!=(const char* str) { return !equal(str); }
	bool String::operator==(const char* str) { return equal(str); }
	void *String::ptr() { return (void*)buff; }
	bool String::startWith(const char* cca) { return startWith((char*)cca, strlen(cca)); }
	bool String::startWith(char* ca) { return startWith(ca, strlen(ca)); }
	bool String::startWith(String& str) { return startWith(str.buff, str.len); }
	bool String::endWith(const char* cca) { return endWith((char*)cca, strlen(cca)); }
	bool String::endWith(char* ca) { return endWith(ca, strlen(ca)); }
	bool String::endWith(String& str) { return endWith(str.buff, str.len); }
	#define e___ buff[len] = 0;

	String::String() {
		len = 0;
		buff = (char*)malloc(siz = block);
		e___
	}
	String::~String() {
		siz = len = 0;
		free(buff);
	}
	String::String(char *bu, unsigned int le, unsigned int si) {
		buff = bu;
		len = le;
		siz = si;
	}
	String::String(char* ca, unsigned int leng) {
		len = leng;
		buff = (char*)malloc(siz = len + block);
		memcpy(buff, ca, len);
		e___
	}
	String::String(bool con) {
		buff = (char*)malloc(siz = 5 + block);
		if(con == true) memcpy(buff, "true", len = 4);
		else memcpy(buff, "false", len = 5);
		e___
	}
	String::String(char c) {
		buff = (char*)malloc(siz = block);
		buff[0] = c;
		buff[len = 1] = 0;
	}
	String::String(int inte) {
		buff = (char*)malloc(siz = block);
		len = sprintf(buff, "%d", inte);
		e___
	}
	unsigned int String::length() {
		return len;
	}
	char* String::c_str() {
		//buff[len] = 0;
		return buff;
	}
	void String::add(char c) {
		if(len >= siz) resize_(siz + 1 + block);
		buff[len++] = c;
		e___
	}
	bool String::startWith(char *ca, unsigned int leng) {
		if(len < leng) return false;
		return memcmp(buff, ca, leng) == 0;
	}
	bool String::endWith(char *ca, unsigned int leng) {
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
	}
	void String::remove(unsigned int pos, unsigned int leng) {
		if(!len || pos >= len) return;
		unsigned int e = pos + leng;
		if(e >= len) return remove(pos);
		memcpy(buff + pos, buff + e, len - e);
		len -= leng;
		e___
		#ifndef NO_IC_STR_REDUCE
		reduce();
		#endif
	}
	String String::substring(unsigned int pos, unsigned int leng) {
		if(!len || !leng || pos >= len) return String();
		unsigned int s = leng + block;
		char *b = (char *)malloc(s);
		if((pos + leng) > len) leng = len - pos;
		memcpy(b, buff + pos, leng);
		return String(b, leng, s);
	}
	int String::indexOf(char c, unsigned int pos) {
		while(pos < len) {
			if(buff[pos] == c) return pos;
			pos++;
		}
		return -1;
	}
	int String::toInt(unsigned int pos) {
		return atoi(buff + pos);
	}



	unsigned int String::size() {
		return siz;
	}
	void String::resize_(unsigned int s) {
		buff = (char*)realloc(buff, siz = s);
		if(len > s) len = s;
	}
	void String::self_substring(unsigned int pos, unsigned int leng) {
		if(!len || !leng || pos >= len) return;
		if((pos + leng) > len) len = leng = len - pos;
		else len = leng;
		memcpy(buff, buff + pos, leng);
		e___
		#ifndef NO_IC_STR_REDUCE
		reduce();
		#endif
	}
	void String::pop_back() {
		if(len) buff[--len] = 0;
		#ifndef NO_IC_STR_REDUCE
		reduce();
		#endif
	}
	void String::clear() {
		buff[len = 0] = 0;
		#ifndef NO_IC_STR_REDUCE
		reduce();
		#endif
	}
	void String::insert(unsigned int pos, char c) {
		if(!len || pos >= (len -1)) return add(c);
		if(len >= siz) resize_(siz + 1 + block);
		memcpy(buff + pos + 1, buff + pos, len - pos);
		buff[pos] = c;
		buff[len++] = 0;
	}
	void String::insert(unsigned int pos, char *ca, unsigned int leng) {
		if(!leng) return;
		if(!len || pos >= (len -1)) return add(ca);
		if((len + leng) >= siz) resize_(siz + leng + block);
		memcpy(buff + pos + leng, buff + pos, len - pos);
		memcpy(buff + pos, ca, leng);
		len += leng;
		e___
	}
	bool String::equal(char *ca, unsigned int leng) {
		if(leng != len) return true;
		return memcmp(ca, buff, len) == 0;
	}
	bool String::equal(char *ca) {
		return equal(ca, strlen(ca));
	}
	bool String::equal(const char *ca) {
		return equal((char*)ca);
	}
	bool String::equal(const char *ca, unsigned int leng) {
		return equal((char*)ca, leng);
	}
	#ifndef NO_IC_STR_REDUCE
	void String::reduce() {
		if(!auto_size_reduce) return;
		if(siz - len < (block * 2)) return;
		buff = (char*)realloc(buff, siz = len + block * 2);
	}
	#endif
	void String::resize(unsigned int s) {
		if(siz <= s) resize_(s + block);
		buff[len = s] = 0;
		#ifndef NO_IC_STR_REDUCE
		reduce();
		#endif
	}
	char String::charAt(unsigned int pos) {
		return len <= pos ? 0 : buff[pos];
	}
	void String::setChar(char c, unsigned int pos) {
		if(len > pos) buff[pos] = c;
	}

	void String::add(char* ca, unsigned int leng) {
		if(!leng) return;
		if((len + leng) >= siz) resize_(siz + leng + block);
		memcpy(buff + len, ca, leng);
		len += leng;
		e___
	}
	String& String::operator+=(char c) {
		add(c);
		return *this;
	}
	String& String::operator+=(char *ca) {
		add(ca);
		return *this;
	}
	String& String::operator+=(const char *cca) {
		add((char*)cca);
		return *this;
	}
	String& String::operator+=(String &str) {
		add(str.buff, str.len);
		return *this;
	}
	String String::operator+(const String& str) const {
		unsigned int l = len + str.len, s;
		char *b = (char *)malloc(s = l + IC_STR_BLOCK);
		memcpy(b, buff, len);
		memcpy(b + len, str.buff, str.len);
		return String(b, l, s);
	}
	String& String::operator=(const String& str) {
		bool t;
		if((t = str.len >= siz)) resize_(str.len + block);
		memcpy(buff, str.buff, len = str.len);
		#ifndef NO_IC_STR_REDUCE
		if(!t) reduce();
		#endif
		return *this;
	}
	int String::indexOf(char *ca, unsigned int leng, unsigned int pos) {
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
}
