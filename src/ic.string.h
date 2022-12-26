#ifndef IC_STRING_H_INCLUDED
#define IC_STRING_H_INCLUDED

#ifndef IC_STR_BLOCK
#define IC_STR_BLOCK	512
#endif

namespace ic {
	class string {
	public:
		string();
		~string();
		string(char *bu, unsigned int le, unsigned int si);
		string(char c);
		string(char *ca, unsigned int leng);
		string(char *ca);
		string(bool con);
		string(int inte);
		string(const char* cca);
		unsigned int length();
		char* c_str();
		void* ptr();
		operator char*();
		operator void*();
		operator const char*();
		bool operator!=(const char* str);
		bool operator==(const char* str);
		bool start_with(const char* cca);
		bool start_with(char* ca);
		bool start_with(string& str);
		bool start_with(char* ca, unsigned int leng);
		bool end_with(const char* cca);
		bool end_with(char* ca);
		bool end_with(string& str);
		bool end_with(char* ca, unsigned int leng);
		void remove(unsigned int pos);
		void remove(unsigned int pos, unsigned int leng);
		string substring(unsigned int pos, unsigned int leng);
		int find(char c, unsigned int pos = 0);
		int find(char *ca, unsigned int pos = 0);
		int find(const char *cca, unsigned int pos = 0);
		int find(char *ca, unsigned int leng, unsigned int pos);
		int to_int(unsigned int pos = 0);

		char char_at(unsigned int pos = 0);
		void set_char(char c, unsigned int pos = 0);
		void add(char c);
		void add(char* ca, unsigned int leng);
		void add(char* ca);
		unsigned int size();
		void resize(unsigned int s); // set the length, not the size

		bool equal(char *ca, unsigned int leng);
		bool equal(char *ca);
		bool equal(const char *ca, unsigned int leng);
		bool equal(const char *ca);
		string& operator+=(char c);
		string& operator+=(char *ca);
		string& operator+=(const char *cca);
		string& operator+=(string &str);
		string& operator=(const string& str);
		string operator+(const string& str) const;
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
