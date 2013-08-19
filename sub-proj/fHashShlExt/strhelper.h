/*
 * strhelper header file
 * Author: Sun Junwen
 * Version: 1.3
 * Provides converting from tstring, string and wstring to each other
 * And provides string's utf8 converting.
 * Provides triming function to string and wstring.
 * Provides replacing function to string and wstring.
 * Provides newline fixing to string.
 */
#ifndef _STR_HELPER_H_
#define _STR_HELPER_H_
#include <string>
#include <algorithm>
#include <locale>

namespace sunjwbase
{
	// define the tstring
#if defined(UNICODE) || defined(_UNICODE)
	typedef std::wstring tstring;
#else
	typedef std::string tstring;
#endif
    
#if defined (WIN32)
	typedef __int64 INT64;
#endif
#if defined (__APPLE__) || defined (UNIX)
    typedef long long INT64;
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
    
#if defined (__APPLE__) || defined (UNIX)
    std::string striconv(const std::string& input,
                         const std::string& to_code,
                         const std::string& from_code);
#endif
    
	// 将本地编码的 string 转换成 utf8 编码的 string
	inline std::string utf8conv(std::string& strAscii)
	{
#if defined (WIN32)
        return wstrtostrutf8(strtowstr(strAscii));
#endif
#if defined (__APPLE__) || defined (UNIX)
        // current only support GB
        return striconv(strAscii, "UTF-8", "GBK");
#endif
    }
	// 将 utf8 编码的 string 转换成本地编码的 string
	inline std::string asciiconv(std::string& strUtf8)
	{
#if defined (WIN32)
        return wstrtostr(strtowstrutf8(strUtf8));
#endif
#if defined (__APPLE__) || defined (UNIX)
        // current only support GB
        return striconv(strUtf8, "GBK", "UTF-8");
#endif
    }
    
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
    
	// searching part
	// case insensitive equal for ci_strfind
	// templated version of ci_equal so it could work with both char and wchar_t
	template<typename charT>
	struct strequal_ci
	{
		strequal_ci( const std::locale& loc ):loc_(loc)
		{}
        
		bool operator()(charT ch1, charT ch2)
		{
			return std::toupper(ch1, loc_) == std::toupper(ch2, loc_);
		}
	private:
		const std::locale& loc_;
	};
    
	// case insensitive search
	template<typename T>
	INT64 strfind_ci(const T& str, const T& substr, const std::locale& loc = std::locale())
	{
		typename T::const_iterator it = std::search(str.begin(), str.end(),
                                           substr.begin(), substr.end(),
                                           strequal_ci<typename T::value_type>(loc));
		if (it != str.end()) 
			return it - str.begin();
		else 
			return -1; // not found
	}
    
    // itostr
    std::string itostr(int num, int idx = 10);
    // format append for std::string
    std::string strappendformat(std::string& str, const char *format, ...);
}


#endif
