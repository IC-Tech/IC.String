#include <cstdlib>
#include <cstring>
#include <cstdio>
#include "ic.string.h"

namespace ic {
	string::string(char *ca) : string(ca, strlen(ca)) {}
	string::string(const char* cca) : string((char*)cca) {}
	void string::add(char* ca) { return add(ca, strlen(ca)); }
	void string::insert(unsigned int pos, char *ca) { return insert(pos, ca, strlen(ca)); }
	int string::find(char *ca, unsigned int pos) { return find(ca, strlen(ca), pos); }
	int string::find(const char *cca, unsigned int pos) { return find((char*)cca, strlen(cca), pos); }
	void string::push_back(char c) { return add(c); }
	void string::insert(unsigned int pos, const char *ca) { return insert(pos, (char*)ca); }
	string::operator char*() { return c_str(); }
	string::operator void*() { return ptr(); }
	string::operator const char*() { return (const char*)c_str(); }
	bool string::operator!=(const char* str) { return !equal(str); }
	bool string::operator==(const char* str) { return equal(str); }
	void *string::ptr() { return (void*)buff; }
	bool string::start_with(const char* cca) { return start_with((char*)cca, strlen(cca)); }
	bool string::start_with(char* ca) { return start_with(ca, strlen(ca)); }
	bool string::start_with(string& str) { return start_with(str.buff, str.len); }
	bool string::end_with(const char* cca) { return end_with((char*)cca, strlen(cca)); }
	bool string::end_with(char* ca) { return end_with(ca, strlen(ca)); }
	bool string::end_with(string& str) { return end_with(str.buff, str.len); }
	#define e___ buff[len] = 0;

	string::string() {
		len = 0;
		buff = (char*)malloc(siz = block);
		e___
	}
	string::~string() {
		siz = len = 0;
		free(buff);
	}
	string::string(char *bu, unsigned int le, unsigned int si) {
		buff = bu;
		len = le;
		siz = si;
	}
	string::string(char* ca, unsigned int leng) {
		len = leng;
		buff = (char*)malloc(siz = len + block);
		memcpy(buff, ca, len);
		e___
	}
	string::string(bool con) {
		buff = (char*)malloc(siz = 5 + block);
		if(con == true) memcpy(buff, "true", len = 4);
		else memcpy(buff, "false", len = 5);
		e___
	}
	string::string(char c) {
		buff = (char*)malloc(siz = block);
		buff[0] = c;
		buff[len = 1] = 0;
	}
	string::string(int inte) {
		buff = (char*)malloc(siz = block);
		len = sprintf(buff, "%d", inte);
		e___
	}
	unsigned int string::length() {
		return len;
	}
	char* string::c_str() {
		//buff[len] = 0;
		return buff;
	}
	void string::add(char c) {
		if(len >= siz) resize_(siz + 1 + block);
		buff[len++] = c;
		e___
	}
	bool string::start_with(char *ca, unsigned int leng) {
		if(len < leng) return false;
		return memcmp(buff, ca, leng) == 0;
	}
	bool string::end_with(char *ca, unsigned int leng) {
		if(len < leng) return false;
		for(int i=leng - 1, s = len - leng; i >= 0; i--) if(buff[s + i] != ca[i]) return false;
		return true;
	}
	void string::remove(unsigned int pos) {
		if(pos >= len) return;
		buff[len = pos] = 0;
		#ifndef NO_IC_STR_REDUCE
		reduce();
		#endif
	}
	void string::remove(unsigned int pos, unsigned int leng) {
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
	string string::substring(unsigned int pos, unsigned int leng) {
		if(!len || !leng || pos >= len) return string();
		unsigned int s = leng + block;
		char *b = (char *)malloc(s);
		if((pos + leng) > len) leng = len - pos;
		memcpy(b, buff + pos, leng);
		return string(b, leng, s);
	}
	int string::find(char c, unsigned int pos) {
		while(pos < len) {
			if(buff[pos] == c) return pos;
			pos++;
		}
		return -1;
	}
	int string::to_int(unsigned int pos) {
		return atoi(buff + pos);
	}



	unsigned int string::size() {
		return siz;
	}
	void string::resize_(unsigned int s) {
		buff = (char*)realloc(buff, siz = s);
		if(len > s) len = s;
	}
	void string::self_substring(unsigned int pos, unsigned int leng) {
		if(!len || !leng || pos >= len) return;
		if((pos + leng) > len) len = leng = len - pos;
		else len = leng;
		memcpy(buff, buff + pos, leng);
		e___
		#ifndef NO_IC_STR_REDUCE
		reduce();
		#endif
	}
	void string::pop_back() {
		if(len) buff[--len] = 0;
		#ifndef NO_IC_STR_REDUCE
		reduce();
		#endif
	}
	void string::clear() {
		buff[len = 0] = 0;
		#ifndef NO_IC_STR_REDUCE
		reduce();
		#endif
	}
	void string::insert(unsigned int pos, char c) {
		if(!len || pos >= (len -1)) return add(c);
		if(len >= siz) resize_(siz + 1 + block);
		memcpy(buff + pos + 1, buff + pos, len - pos);
		buff[pos] = c;
		buff[len++] = 0;
	}
	void string::insert(unsigned int pos, char *ca, unsigned int leng) {
		if(!leng) return;
		if(!len || pos >= (len -1)) return add(ca);
		if((len + leng) >= siz) resize_(siz + leng + block);
		memcpy(buff + pos + leng, buff + pos, len - pos);
		memcpy(buff + pos, ca, leng);
		len += leng;
		e___
	}
	bool string::equal(char *ca, unsigned int leng) {
		if(leng != len) return true;
		return memcmp(ca, buff, len) == 0;
	}
	bool string::equal(char *ca) {
		return equal(ca, strlen(ca));
	}
	bool string::equal(const char *ca) {
		return equal((char*)ca);
	}
	bool string::equal(const char *ca, unsigned int leng) {
		return equal((char*)ca, leng);
	}
	#ifndef NO_IC_STR_REDUCE
	void string::reduce() {
		if(!auto_size_reduce) return;
		if(siz - len < (block * 2)) return;
		buff = (char*)realloc(buff, siz = len + block * 2);
	}
	#endif
	void string::resize(unsigned int s) {
		if(siz <= s) resize_(s + block);
		buff[len = s] = 0;
		#ifndef NO_IC_STR_REDUCE
		reduce();
		#endif
	}
	char string::char_at(unsigned int pos) {
		return len <= pos ? 0 : buff[pos];
	}
	void string::set_char(char c, unsigned int pos) {
		if(len > pos) buff[pos] = c;
	}

	void string::add(char* ca, unsigned int leng) {
		if(!leng) return;
		if((len + leng) >= siz) resize_(siz + leng + block);
		memcpy(buff + len, ca, leng);
		len += leng;
		e___
	}
	string& string::operator+=(char c) {
		add(c);
		return *this;
	}
	string& string::operator+=(char *ca) {
		add(ca);
		return *this;
	}
	string& string::operator+=(const char *cca) {
		add((char*)cca);
		return *this;
	}
	string& string::operator+=(string &str) {
		add(str.buff, str.len);
		return *this;
	}
	string string::operator+(const string& str) const {
		unsigned int l = len + str.len, s;
		char *b = (char *)malloc(s = l + IC_STR_BLOCK);
		memcpy(b, buff, len);
		memcpy(b + len, str.buff, str.len);
		return string(b, l, s);
	}
	string& string::operator=(const string& str) {
		bool t;
		if((t = str.len >= siz)) resize_(str.len + block);
		memcpy(buff, str.buff, len = str.len);
		#ifndef NO_IC_STR_REDUCE
		if(!t) reduce();
		#endif
		return *this;
	}
	int string::find(char *ca, unsigned int leng, unsigned int pos) {
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
