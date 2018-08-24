/*
|| @file IC.String.h
|| @version 1.0
|| @author Imesh Chamara
|| @contact imesh1chamara@gmail.com, +94717401880
||
|| @description
|| | IC.String Library for control chars strings easily
|| #
||
|| @license
|| | This library is free software; you can redistribute it and/or
|| | modify it under the terms of the GNU Lesser General Public
|| | License as published by the Free Software Foundation; version
|| | 2.1 of the License.
|| |
|| | This library is distributed in the hope that it will be useful,
|| | but WITHOUT ANY WARRANTY; without even the implied warranty of
|| | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
|| | Lesser General Public License for more details.
|| |
|| | You should have received a copy of the GNU Lesser General Public
|| | License along with this library; if not, write to the Free Software
|| | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
|| #
||
*/
#ifndef IC_STRING_H_INCLUDED
#define IC_STRING_H_INCLUDED
namespace IC
{
    class String
    {
    public:
        String();
        String(char c);
        String(char* ca);
        String(bool con);
        String(int inte);
        String(const char* cca);
        int length();
        char* c_str();
        operator char*()                            { return c_str();}
        operator const char*()                      {return (const char*)c_str();}
        String& operator+=(const String& str);
        bool operator==(const char* str);
        bool operator!=(const char* str);
        bool startWith(const String& str);
        bool endWith(const String& str);
        void remove(int pos);
        void remove(int pos,int leng);
        void substring(int pos,int leng);
        int indexOf(char c);
        int indexOf(char *ca);
        int indexOf(const char *cc)                 {return indexOf((char*)cc);     }
        int toInt()                                 {return toInt(0);               }
        int toInt(int pos);
    private:
        static int strlen(char* ca);
        char* buff;
        int len;
        void Add(char c);
        void Add(char* ca);
        static int by10(int co);
        void endit();
        static int ReadNumber(char* ca,int pos);
    };
}
#endif // IC_STRING_H_INCLUDED
