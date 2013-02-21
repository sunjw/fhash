/*
 * strhelpe header file
 * Author: Sun Junwen
 * Version: 1.2.2
 * Provides converting from tstring, string and wstring to each other
 * And provides string's utf8 converting.
 * Provides triming function to string and wstring.
 * Provides replacing function to string and wstring.
 * Provides newline fixing to string.
 */
#ifndef _STR_HELPER_H_
#define _STR_HELPER_H_
#include <string>

namespace sunjwbase
{
	// define the tstring
#if defined(UNICODE) || defined(_UNICODE)
	typedef std::wstring tstring;
#else
	typedef std::string tstring;
#endif

	// converting part
	/*
	 * 将 ascii 编码的 wstring 转换为 utf8 编码的 string
	 */
	std::string wstrtostrutf8(const std::wstring& wstr);
	/*
	 * 将 utf8 编码的 string 转换为 wstring
	 * 如果需要把 utf8 编码的 string 转换为本地编码的 string
	 * 再调用一次 wstrtostr
	 */
	std::wstring strtowstrutf8(const std::string& str);
	std::string wstrtostr(const std::wstring& wstr);
	std::wstring strtowstr(const std::string& str);

	// 将本地编码的 string 转换成 utf8 编码的 string
	inline std::string utf8conv(std::string& strAscii)
	{ return wstrtostrutf8(strtowstr(strAscii)); }
	// 将 utf8 编码的 string 转换成本地编码的 string
	inline std::string asciiconv(std::string& strUtf8)
	{ return wstrtostr(strtowstrutf8(strUtf8)); }

	inline std::string tstrtostr(const tstring& tstr)
	{
#if defined(UNICODE) || defined(_UNICODE)
		return wstrtostr(tstr);
#else
		return tstr;
#endif
	}

	inline std::wstring tstrtowstr(const tstring& tstr)
	{
#if defined(UNICODE) || defined(_UNICODE)
		return tstr;
#else
		return strtowstr(tstr);
#endif
	}

	inline sunjwbase::tstring strtotstr(const std::string& str)
	{
#if defined(UNICODE) || defined(_UNICODE)
		return strtowstr(str);
#else
		return str;
#endif
	}

	inline sunjwbase::tstring wstrtotstr(const std::wstring& wstr)
	{
#if defined(UNICODE) || defined(_UNICODE)
		return wstr;
#else
		return wstrtostr(wstr);
#endif
	}
	// converting part

	// triming part
	const std::string SPACES(" \t\r\n");
	const std::wstring WSPACES(L" \t\r\n");

	// string version
	std::string strtrim_right(const std::string& s, const std::string& spaces = SPACES);
	std::string strtrim_left(const std::string& s, const std::string& spaces = SPACES);
	inline std::string strtrim(const std::string& s, const std::string& spaces = SPACES)
	{ return strtrim_left(strtrim_right(s, spaces), spaces); }

	// wstring version
	std::wstring strtrim_right(const std::wstring& s, const std::wstring& spaces = WSPACES);
	std::wstring strtrim_left(const std::wstring& s, const std::wstring& spaces = WSPACES);
	inline std::wstring strtrim(const std::wstring& s, const std::wstring& spaces = WSPACES)
	{ return strtrim_left(strtrim_right(s, spaces), spaces); }
	// triming part

	// replacing part
	std::string strreplace(const std::string& base, const std::string& src, const std::string& des);
	std::wstring strreplace(const std::wstring& base, const std::wstring& src, const std::wstring& des);
	// replacing part

	// fixing
	std::string fixnewline(const std::string& str);
	// fixing

	// upper and lower
	std::string str_upper(const std::string& str);
	std::string str_lower(const std::string& str);
}


#endif
