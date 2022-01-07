#ifndef IC_STRING_H_INCLUDED
#define IC_STRING_H_INCLUDED

#ifndef IC_STR_BLOCK
#define IC_STR_BLOCK	512
#endif

namespace IC {
	class String {
	public:
		String();
		~String();
		String(char *bu, unsigned int le, unsigned int si);
		String(char c);
		String(char *ca, unsigned int leng);
		String(char *ca);
		String(bool con);
		String(int inte);
		String(const char* cca);
		unsigned int length();
		char* c_str();
		void* ptr();
		operator char*();
		operator void*();
		operator const char*();
		bool operator!=(const char* str);
		bool operator==(const char* str);
		bool startWith(const char* cca);
		bool startWith(char* ca);
		bool startWith(String& str);
		bool startWith(char* ca, unsigned int leng);
		bool endWith(const char* cca);
		bool endWith(char* ca);
		bool endWith(String& str);
		bool endWith(char* ca, unsigned int leng);
		void remove(unsigned int pos);
		void remove(unsigned int pos, unsigned int leng);
		String substring(unsigned int pos, unsigned int leng);
		int indexOf(char c, unsigned int pos = 0);
		int indexOf(char *ca, unsigned int pos = 0);
		int indexOf(const char *cca, unsigned int pos = 0);
		int indexOf(char *ca, unsigned int leng, unsigned int pos);
		int toInt(unsigned int pos = 0);

		char charAt(unsigned int pos = 0);
		void setChar(char c, unsigned int pos = 0);
		void add(char c);
		void add(char* ca, unsigned int leng);
		void add(char* ca);
		unsigned int size();
		void resize(unsigned int s); // set the length, not the size

		bool equal(char *ca, unsigned int leng);
		bool equal(char *ca);
		bool equal(const char *ca, unsigned int leng);
		bool equal(const char *ca);
		String& operator+=(char c);
		String& operator+=(char *ca);
		String& operator+=(const char *cca);
		String& operator+=(String &str);
		String& operator=(const String& str);
		String operator+(const String& str) const;
		void self_substring(unsigned int pos, unsigned int leng);
		void pop_back();
		void push_back(char c);
		void clear();
		void insert(unsigned int pos, char c);
		void insert(unsigned int pos, char *ca, unsigned int leng);
		void insert(unsigned int pos, char *ca);
		void insert(unsigned int pos, const char *ca);

		// internal functions, put to protected if wanted
		void resize_(unsigned int s);
#ifndef NO_IC_STR_REDUCE
		bool auto_size_reduce = true;
		void reduce();
#endif
	protected:
		char* buff;
		unsigned int len, siz;
		const unsigned short block = IC_STR_BLOCK;
	};
}
#endif // IC_STRING_H_INCLUDED
