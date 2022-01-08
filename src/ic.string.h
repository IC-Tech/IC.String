#ifndef IC_STRING_H_INCLUDED
#define IC_STRING_H_INCLUDED

#ifndef IC_STR_BLOCK
#define IC_STR_BLOCK	512 // normally sizeof wchar_t == 4 so, this will be IC_STR_BLOCK * 4
#endif


// build flags
//#define NO_IC_STR_SAME_OUT
//#define NO_IC_STR_UTF8_CACHE

namespace IC {
	class String {
	public:
		String();
		~String();
		String(wchar_t *bu, unsigned int le, unsigned int si);
		String(char c);
		String(wchar_t c);
		String(char *ca);
		String(char *ca, unsigned int leng);
		String(wchar_t *ca, unsigned int leng);
		String(wchar_t *ca);
		String(bool con);
		String(int inte);
		String(const wchar_t* cca);
		unsigned int length();
		wchar_t* wc_str();
#ifdef NO_IC_STR_UTF8_CACHE
		char* c_str(unsigned int *leng = nullptr);
#else
		char* c_str(unsigned int *leng = nullptr, bool force=false);
#endif
		void* ptr();
		operator char*();
		operator wchar_t*();
		operator void*();
		operator const wchar_t*();
		bool operator!=(const wchar_t* str);
		bool operator==(const wchar_t* str);
		bool startWith(const wchar_t* cca);
		bool startWith(wchar_t* ca);
		bool startWith(String& str);
		bool startWith(wchar_t* ca, unsigned int leng);
		bool endWith(const wchar_t* cca);
		bool endWith(wchar_t* ca);
		bool endWith(String& str);
		bool endWith(wchar_t* ca, unsigned int leng);
		void remove(unsigned int pos);
		void remove(unsigned int pos, unsigned int leng);
		String substring(unsigned int pos, unsigned int leng);
		int indexOf(wchar_t c, unsigned int pos = 0);
		int indexOf(wchar_t *ca, unsigned int pos = 0);
		int indexOf(const wchar_t *cca, unsigned int pos = 0);
		int indexOf(wchar_t *ca, unsigned int leng, unsigned int pos);
		int toInt(unsigned int pos = 0);

		wchar_t charAt(unsigned int pos = 0);
		void setChar(wchar_t c, unsigned int pos = 0);
		void add(char c);
		void add(wchar_t c);
		void add(char *ca);
		void add(wchar_t* ca);
		void add(char *ca, unsigned int leng);
		void add(wchar_t* ca, unsigned int leng);
		unsigned int size();
		void resize(unsigned int s); // set the length, not the size

		bool equal(wchar_t *ca, unsigned int leng);
		bool equal(wchar_t *ca);
		bool equal(const wchar_t *ca, unsigned int leng);
		bool equal(const wchar_t *ca);
		String& operator+=(char c);
		String& operator+=(char *ca);
		String& operator+=(const char *cca);
		String& operator+=(wchar_t c);
		String& operator+=(wchar_t *ca);
		String& operator+=(const wchar_t *cca);
		String& operator+=(String &str);
		String& operator=(const String& str);
		String operator+(const String& str) const;
		void self_substring(unsigned int pos, unsigned int leng);
		void pop_back();
		void push_back(wchar_t c);
		void clear();
		void insert(unsigned int pos, wchar_t c);
		void insert(unsigned int pos, wchar_t *ca, unsigned int leng);
		void insert(unsigned int pos, wchar_t *ca);
		void insert(unsigned int pos, const wchar_t *ca);

		// internal functions, put to protected if wanted
		void resize_(unsigned int s);
#ifndef NO_IC_STR_REDUCE
		bool auto_size_reduce = true;
		void reduce();
#endif
#ifndef NO_IC_STR_UTF8_CACHE
		bool auto_utf_size_reduce = true;
#endif
	protected:
		wchar_t* buff;
#ifndef NO_IC_STR_UTF8_CACHE
		char *utf_buff;
		unsigned int utf_len = 0, utf_siz = 0;
		bool buff_mod = true;
#endif
		unsigned int len, siz;
		const unsigned short block = IC_STR_BLOCK;
	};
}
#endif // IC_STRING_H_INCLUDED
