/*
|| @file IC.String.cpp
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
#include "IC.String.h"
namespace IC
{
    String::String()
    {
        buff = new char[0];
        len = 0;
        endit();
    }
    String::String(char* ca)
    {
        len = strlen(ca);
        buff = new char[len];
        for(int i=0; i<len; i++)
        {
            buff[i] = ca[i];
        }
        endit();
    }
    String::String(bool con)
    {
        if(con == true)
        {
            buff = (char*)("true");
            len = 4;
            endit();
        }
        else
        {
            buff = (char*)("false");
            len = 5;
            endit();
        }
    }
    String::String(char c)
    {
        buff = new char[2];
        buff[0] = c;
        len = 1;
        endit();
    }
    String::String(int inte)
    {
        if(inte >= 0 && inte < 10)
        {
            String((char)(inte + 48));
        }
        else if(inte > 9)
        {
            int clen = 0;
            bool stp = false;
            while(!stp)
            {
                if(by10(clen) > inte)
                {
                    stp = true;
                }
                clen++;
            }
            int* vals = new int[clen];
            char* Cvals = new char[clen];
            for(int i=0; i<clen-1; i++)
            {
                vals[i] = inte / (by10(clen - i - 2));
                inte = inte - (vals[i] * (by10(clen - i - 2)));
                Cvals[i] = vals[i] + 48;
            }
            Cvals[clen-1] = '\0';
            len = clen - 1;
            buff = Cvals;
        }
        else if(inte < 0 && inte > -10)
        {
            char* vals = new char[3];
            vals[0] = '-';
            vals[1] = (char)((inte / -1) + 48);
            vals[2] = '\0';
            buff = vals;
            len = 2;
        }
        else if(inte < -9)
        {
            inte = inte * -1;
            int clen = 0;
            bool stp = false;
            while(!stp)
            {
                if(by10(clen) > inte)
                {
                    stp = true;
                }
                clen++;
            }
            int* vals = new int[clen];
            char* Cvals = new char[clen + 1];
            Cvals[0] = '-';
            for(int i=0; i<clen-1; i++)
            {
                vals[i] = inte / (by10(clen - i - 2));
                inte = inte - (vals[i] * (by10(clen - i - 2)));
                Cvals[i + 1] = vals[i] + 48;
            }
            Cvals[clen] = '\0';
            len = clen;
            buff = Cvals;
        }
        endit();
    }
    int String::by10(int co)
    {
        int val = 1;
        for(int i=0; i<co; i++)
        {
            val *= 10;
        }
        return val;
    }
    String::String(const char* cca)
    {
        char* ca = (char*)cca;
        len = strlen(ca);
        buff = new char[len];
        for(int i=0; i<len; i++)
        {
            buff[i] = ca[i];
        }
        endit();
    }
    int String::strlen(char* ca)
    {
        int len = 0;
        while(true)
        {
            if(ca[len] == '\0')
            {
                return len;
            }
            len++;
        }
        return -1;
    }
    int String::length()
    {
        return len;
    }
    char* String::c_str()
    {
        return buff;
    }
    void String::Add(char c)
    {
        char* temp = new char[len + 1];
        for(int i=0; i<len; i++)
        {
            temp[i] = buff[i];
        }
        temp[len] = c;
        buff = temp;
        len++;
    }
    void String::Add(char* ca)
    {
        int calen = strlen(ca);
        char* temp = new char[len + calen];
        for(int i=0; i<len; i++)
        {
            temp[i] = buff[i];
        }
        for(int i=0; i<calen; i++)
        {
            temp[len + i] = ca[i];
        }
        buff = temp;
        len += calen;
    }
    String& String::operator+=(const String& str)
    {
        Add(str.buff);
        endit();
        return *this;
    }
    bool String::operator!=(const char* str)
    {
        char* str1 = (char*)str;
        int len1 = strlen(str1);
        if(len1 != len)
            return true;
        for(int i=0; i<len; i++)
        {
            if(str1[i] != buff[i])
                return true;
        }
        return false;
    }
    bool String::operator==(const char* str)
    {
        char* str1 = (char*)str;
        int len1 = strlen(str1);
        if(len1 != len)
            return false;
        for(int i=0; i<len; i++)
        {
            if(str1[i] != buff[i])
                return false;
        }
        return true;
    }
    bool String::startWith(const String& str)
    {
        if(len < str.len)
            return false;
        for(int i=0; i<str.len; i++)
        {
            if(str.buff[i] != buff[i])
                return false;
        }
        return true;
    }
    bool String::endWith(const String& str)
    {
        if(len < str.len)
            return false;
        for(int i=0; i<str.len; i++)
        {
            if(str.buff[i] != buff[(len - str.len) + i])
                return false;
        }
        return true;
    }
    void String::remove(int pos)
    {
        if(pos < len)
        {
            int len1 = len - (len - pos);
            char* temp = new char[len1];
            for(int i=0; i<pos; i++)
            {
                temp[i] = buff[i];
            }
            buff = temp;
            len = len1;
            endit();
        }
    }
    void String::endit()
    {
        if(buff[len] != '\0')
            buff[len] = '\0';
    }
    void String::remove(int pos,int leng)
    {

        if((pos + leng) < len)
        {
            char* temp = new char[len - leng];
            for(int i=0; i<pos; i++)
            {
                temp[i] = buff[i];
            }
            for(int i=pos; i<(len - leng); i++)
            {
                temp[i] = buff[i + leng];
            }
            buff = temp;
            len = len - leng;
            endit();
        }
    }
    void String::substring(int pos,int leng)
    {
        if((pos + leng) < len)
        {
            char* temp = new char[leng];
            for(int i=0; i<leng; i++)
            {
                temp[i] = buff[pos + i];
            }
            buff = temp;
            len = leng;
            endit();
        }
    }
    int String::indexOf(char c)
    {
        for(int i=0 ;i<len; i++)
        {
            if(buff[i] == c)
                return i;
        }
        return -1;
    }
    int String::toInt(int pos)
    {
        char* str = buff;
        String Nstr = "0123456789";
        int val = 0;
        int len1 = strlen(str);
        for(int i=pos; i<len1; i++)
        {
            if(Nstr.indexOf(str[i]) == -1)
            {
                len1 = i;
                i = len1 + 1;
            }
        }
        for(int i=pos; i<len1; i++)
        {
            int val1 = ReadNumber(str,i);
            if(val1 == -1)
                return 0;
            val += (val1 * by10(len1 - i - 1));
        }
        return val;
    }
    int String::ReadNumber(char* ca,int pos)
    {
        String Nstr = "0123456789";
        return Nstr.indexOf(ca[pos]);
    }
    int String::indexOf(char *ca)
    {
        int len1 = strlen(ca);
        if(len1 < 1 || len < len1)
        {
            return -1;
        }
        for(int i=0; i<len ; i++)
        {
            if(buff[i] == ca[0])
            {
                int coun = 0;
                for(int i1 =0;i1<len1; i1++)
                {
                    if(buff[i + i1] != ca[i1])
                        i1 = len1;
                    else
                        coun++;
                }
                if(coun == len1)
                    return i;
            }
        }
        return -1;
    }
}
