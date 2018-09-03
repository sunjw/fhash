/*
 * strhelper implementation file
 * Author: Sun Junwen
 * Version: 2.2
 * Provides converting from tstring, string and wstring to each other
 * And provides string's utf8 converting.
 * Provides triming function to string and wstring.
 * Provides replacing function to string and wstring.
 * Provides newline fixing to string.
 */
#include "stdafx.h"

#include <math.h>
#include <string>

#if defined (WIN32)
#include "Windows.h"
#endif

#if defined (__APPLE__) || defined (__unix)
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <locale.h>
#include <iconv.h>
#include <errno.h>
#include <iostream>
#include <locale>
#include <vector>
#endif

#include "strhelper.h"

namespace sunjwbase
{
#if defined (WIN32)
	// Windows convert
	static std::string _wstrtostr(const std::wstring& wstr, UINT codePage)
	{
		// Convert a wstring to a specified code page encoded string 
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
		// Convert a specified code page encoded string to a wstring 
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

#if defined (__APPLE__) || defined (__unix)
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
 * convert wstring to utf8 encoded string
 */
std::string sunjwbase::wstrtostrutf8(const std::wstring& wstr)
{
	// Convert a wstring to an utf8 encoded string
#if defined (WIN32)
	return _wstrtostr(wstr, CP_UTF8);
#endif
#if defined (__APPLE__) || defined (__unix)
	return striconv(wstrtostr(wstr), "UTF-8", "ASCII");
#endif
}

/*
 * convert utf8 encoded string to wstring
 * 1st, convert utf8 encoded string to native string
 * 2nd, call strtowstr
 */
std::wstring sunjwbase::strtowstrutf8(const std::string& str)
{
	// Convert an utf8 encoded string to a wstring
#if defined (WIN32)
	return _strtowstr(str, CP_UTF8);
#endif
#if defined (__APPLE__) || defined (__unix)
	return strtowstr(striconv(str, "ASCII", "UTF-8"));
#endif
}

std::string sunjwbase::wstrtostr(const std::wstring& wstr)
{
	// Convert a wstring to an string
#if defined (WIN32)
	return _wstrtostr(wstr, CP_ACP);
#endif
#if defined (__APPLE__) || defined (__unix)
	setlocale(LC_ALL, "zh_CN.UTF-8");
	size_t num_chars = wcstombs(NULL, wstr.c_str(), 0);
	char* char_buf = new char[num_chars + 1];
	wcstombs(char_buf, wstr.c_str(), num_chars);
	char_buf[num_chars] = '\0';
	std::string str(char_buf);
	delete[] char_buf;
	setlocale(LC_ALL, "C");
	
	return str;
#endif
}

std::wstring sunjwbase::strtowstr(const std::string& str)
{
	// Convert an string to a wtring
#if defined (WIN32)
	return _strtowstr(str, CP_ACP);
#endif
#if defined (__APPLE__) || defined (__unix)
	setlocale(LC_ALL, "zh_CN.UTF-8");
	size_t num_chars = mbstowcs(NULL, str.c_str(), 0);
	wchar_t* wct_buf = new wchar_t[num_chars + 1];
	mbstowcs(wct_buf, str.c_str(), num_chars);
	wct_buf[num_chars] = '\0';
	std::wstring wstr(wct_buf, num_chars);
	delete[] wct_buf;
	setlocale(LC_ALL, "C");
	
	return wstr;
#endif
}

std::string sunjwbase::asciiconvjson(std::string& strJsonUtf16)
{
	std::wstring wstrTemp;

	std::string::size_type i = 0;
	for (; i + 5 < strJsonUtf16.length(); ++i)
	{
		if (strJsonUtf16[i] == '\\' &&
			strJsonUtf16[i + 1] == 'u')
		{
			char n[4];
			n[0] = strJsonUtf16[i + 2];
			n[1] = strJsonUtf16[i + 3];
			n[2] = strJsonUtf16[i + 4];
			n[3] = strJsonUtf16[i + 5];
			
			i += 5;

			int utf16 = 0;
			for (int j = 0; j < 4; ++j)
			{
				int num = 0;
				if (n[j] > 'a')
					num = n[j] - 'a' + 10;
				else if (n[j] > '0')
					num = n[j] - '0';

				utf16 += (int)(num * pow(16.0, 3.0 - j));
			}

			wchar_t wch = utf16;
			wstrTemp += wch;
		}
	}

	return sunjwbase::wstrtostr(wstrTemp);
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
	// convert new line, /10->/13/10, /13/13/10->/13/10
	strRepairSrc += (char)10;
	strRepairDst += (char)13;
	strRepairDst += (char)10;
	ret = strreplace(str, strRepairSrc, strRepairDst);
	strRepairSrc = (char)13;
	strRepairSrc += (char)13;
	strRepairSrc += (char)10;
	ret = strreplace(ret, strRepairSrc, strRepairDst);
	// convert new line, /13->/13/10, /13/10/10->/13/10
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
#if defined (WIN32)
		int n = vsnprintf_s((char *)temp.c_str(), size, size, format, vl);
#else
		int n = vsnprintf((char *)temp.c_str(), size, format, vl);
#endif
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

bool sunjwbase::str_startwith(const std::string& str, const std::string& target)
{
	if(str.length() < target.length())
		return false;

	// Length is enough, let's check content.
	size_t i = 0;
	for(; i < target.length(); ++i)
	{
		if(str[i] != target[i])
			return false;
	}

	return (i == target.length());
}

bool sunjwbase::str_endwith(const std::string& str, const std::string& target)
{
	if(str.length() < target.length())
		return false;

	// Length is enough, let's check content.
	size_t str_len = str.length();
	size_t target_len = target.length();
	size_t i = 0;
	for(; i < target.length(); ++i)
	{
		if(str[str_len - target_len + i] != target[i])
			return false;
	}

	return (i == target.length());
}
