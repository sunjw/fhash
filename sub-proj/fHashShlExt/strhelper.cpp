/*
 * strhelper implementation file
 * Author: Sun Junwen
 * Version: 1.3
 * Provides converting from tstring, string and wstring to each other
 * And provides string's utf8 converting.
 * Provides triming function to string and wstring.
 * Provides replacing function to string and wstring.
 * Provides newline fixing to string.
 */
#include "stdafx.h"

#include <string>

#if defined (WIN32)
#include "Windows.h"
#endif

#if defined (__APPLE__) || defined (UNIX)
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <clocale>
#include <locale>
#include <vector>
#include <iconv.h>
#include <errno.h>
#endif

#include "strhelper.h"

namespace sunjwbase
{
#if defined (WIN32)
    // Windows convert
	static std::string _wstrtostr(const std::wstring& wstr, UINT codePage)
	{
		// Convert a wstring to an string with specified code page
		size_t strLen = WideCharToMultiByte(codePage, 0, wstr.c_str(), (int)wstr.length(), NULL, 0, NULL, NULL);
		std::string strTo;
		char *szTo = new char[strLen + 1];
		szTo[strLen] = '\0';
		WideCharToMultiByte(codePage, 0, wstr.c_str(), -1, szTo, (int)strLen, NULL, NULL);
		strTo = szTo;
		delete[] szTo;
		return strTo;
	}
    
	static std::wstring _strtowstr(const std::string& str, UINT codePage)
	{
		// Convert an string to a wstring with specified code page
		size_t wstrLen = MultiByteToWideChar(codePage, 0, str.c_str(), -1, NULL, 0);
		std::wstring wstrTo;
		wchar_t *wszTo = new wchar_t[wstrLen + 1];
		wszTo[wstrLen] = L'\0';
		MultiByteToWideChar(codePage, 0, str.c_str(), -1, wszTo, (int)wstrLen);
		wstrTo = wszTo;
		delete[] wszTo;
		return wstrTo;
	}
#endif
}

#if defined (__APPLE__) || defined (UNIX)
std::string sunjwbase::striconv(const std::string& input,
                                const std::string& to_code,
                                const std::string& from_code)
{
    char* inptr = new char[input.size() + 1];
    size_t inleft = input.size();
    size_t outleft = inleft * 4 + 1; // should be large enough
    char* outptr = new char[outleft];
    bzero(outptr, outleft);
    
    strcpy(inptr, input.c_str());
    
    iconv_t cd; // conversion descriptor
    if ((cd = iconv_open(to_code.c_str(), from_code.c_str())) == (iconv_t) (-1))
    {
        iconv_close(cd); // failed clean
        return input;
    }
    
    char* in = inptr;
    char* out = outptr;
    outleft = iconv(cd, &in, &inleft, &out, &outleft);
    
    iconv_close(cd);
    
    std::string strRet(outptr);
    
    delete[] inptr;
    delete[] outptr;
    
    return strRet;
}
#endif

/*
 * 将 ascii 编码的 wstring 转换为 utf8 编码的 string
 */
std::string sunjwbase::wstrtostrutf8(const std::wstring& wstr)
{
    // Convert a Unicode wstring to an ASCII string
#if defined (WIN32)
    return _wstrtostr(wstr, CP_UTF8);
#endif
#if defined (__APPLE__) || defined (UNIX)
    return striconv(wstrtostr(wstr), "UTF-8", "ASCII");
#endif
}

/*
 * 将 utf8 编码的 string 转换为 wstring
 * 如果需要把 utf8 编码的 string 转换为本地编码的 string
 * 再调用一次 wstrtostr
 */
std::wstring sunjwbase::strtowstrutf8(const std::string& str)
{
    // Convert an ASCII string to a Unicode wstring
#if defined (WIN32)
    return _strtowstr(str, CP_UTF8);
#endif
#if defined (__APPLE__) || defined (UNIX)
    return strtowstr(striconv(str, "ASCII", "UTF-8"));
#endif
}

std::string sunjwbase::wstrtostr(const std::wstring& wstr)
{
    // Convert a wstring to an string
#if defined (WIN32)
    return _wstrtostr(wstr, CP_ACP);
#endif
#if defined (__APPLE__) || defined (UNIX)
	size_t num_chars = wcstombs(NULL, wstr.c_str(), 0);
	char* char_buf = new char[num_chars + 1];
	wcstombs(char_buf, wstr.c_str(), wstr.size());
	std::string str(char_buf);
	delete[] char_buf;
    
	return str;
#endif
}

std::wstring sunjwbase::strtowstr(const std::string& str)
{
    // Convert an string to a wtring
#if defined (WIN32)
    return _strtowstr(str, CP_ACP);
#endif
#if defined (__APPLE__) || defined (UNIX)
    size_t num_chars = mbstowcs(NULL, str.c_str(), 0);
	wchar_t* wct_buf = new wchar_t[num_chars + 1];
	mbstowcs(wct_buf, str.c_str(), str.size());
	std::wstring wstr(wct_buf, num_chars);
	delete[] wct_buf;
    
	return wstr;
#endif
}

std::string sunjwbase::strtrim_right(const std::string& s, const std::string& spaces)
{
	std::string d(s);
	std::string::size_type i(d.find_last_not_of(spaces));
	if(i == std::string::npos)
		return "";
	else
		return d.erase(d.find_last_not_of(spaces) + 1);
}  // end of trim_right

std::string sunjwbase::strtrim_left(const std::string& s, const std::string& spaces)
{
	std::string d(s);
	return d.erase(0, s.find_first_not_of(spaces));
}  // end of trim_left

std::wstring sunjwbase::strtrim_right(const std::wstring& s, const std::wstring& spaces)
{
	std::wstring d(s);
	std::wstring::size_type i(d.find_last_not_of(spaces));
	if(i == std::wstring::npos)
		return L"";
	else
		return d.erase(d.find_last_not_of(spaces) + 1);
}

std::wstring sunjwbase::strtrim_left(const std::wstring& s, const std::wstring& spaces)
{
	std::wstring d(s);
	return d.erase(0, s.find_first_not_of(spaces));
}

std::string sunjwbase::strreplace(const std::string& base, const std::string& src, const std::string& des)
{
	std::string ret(base);
	std::string::size_type pos = 0;
	std::string::size_type srcLen = src.size();
	std::string::size_type desLen = des.size();
	pos = ret.find(src, pos);
	while((pos != std::string::npos))
	{
		ret.replace(pos, srcLen, des);
		pos = ret.find(src, pos + desLen);
	}
    
	return ret;
}

std::wstring sunjwbase::strreplace(const std::wstring& base, const std::wstring& src, const std::wstring& des)
{
	std::wstring ret(base);
	std::wstring::size_type pos = 0;
	std::wstring::size_type srcLen = src.size();
	std::wstring::size_type desLen = des.size();
	pos = ret.find(src, pos);
	while((pos != std::wstring::npos))
	{
		ret.replace(pos, srcLen, des);
		pos = ret.find(src, pos + desLen);
	}
    
	return ret;
}

std::string sunjwbase::fixnewline(const std::string& str)
{
	std::string ret;
	std::string strRepairSrc(""), strRepairDst("");
	// 修理换行符, /10->/13/10, /13/13/10->/13/10
	strRepairSrc += (char)10;
	strRepairDst += (char)13;
	strRepairDst += (char)10;
	ret = strreplace(str, strRepairSrc, strRepairDst);
	strRepairSrc = (char)13;
	strRepairSrc += (char)13;
	strRepairSrc += (char)10;
	ret = strreplace(ret, strRepairSrc, strRepairDst);
	// 修理换行符, /13->/13/10, /13/10/10->/13/10
	strRepairSrc = "";
	strRepairDst = "";
	strRepairSrc += (char)13;
	strRepairDst += (char)13;
	strRepairDst += (char)10;
	ret = strreplace(ret, strRepairSrc, strRepairDst);
	strRepairSrc = (char)13;
	strRepairSrc += (char)10;
	strRepairSrc += (char)10;
	ret = strreplace(ret, strRepairSrc, strRepairDst);
	return ret;
}

std::string sunjwbase::str_upper(const std::string& str)
{
	std::string ret(str);
	std::transform(ret.begin(), ret.end(), ret.begin(), ::toupper);
	return ret;
}

std::string sunjwbase::str_lower(const std::string& str)
{
	std::string ret(str);
	std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
	return ret;
}

std::string sunjwbase::itostr(int num, int idx /* = 10 */)
{
    if(idx > 16)
        idx = 16;
    char temp[2] = {0};
    std::string ret;
    while(1)
    {
        int n = num % idx;
        if(n < 10)
            temp[0] = n + '0';
        else
            temp[0] = n - 10 + 'A';
        ret = temp + ret;
        num /= idx;
        if(num == 0)
            break;
    }
    
    return ret;
}

std::string sunjwbase::strappendformat(std::string& str, const char *format, ...)
{
    int size = 100;
    std::string temp;
    va_list vl;
    while (1) {
        temp.resize(size);
        va_start(vl, format);
        int n = vsnprintf((char *)temp.c_str(), size, format, vl);
        va_end(vl);
        if (n > -1 && n < size) {
            temp.resize(n);
            break;
        }
        if (n > -1)
            size = n + 1; // not large enough
        else
            size *= 2;
    }
    str.append(temp);
    
    return str;
}
