/*
 * strhelper header file
 * Author: Sun Junwen
 * Version: 2.2
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
	// define the tstring only for WIN32 platform
#if defined(WIN32) && (defined(UNICODE) || defined(_UNICODE))
#define _UNICODE_HELPER // use _UNICODE_HELPER internally
	typedef std::wstring tstring;
#else
	typedef std::string tstring;
#endif
	
#if defined (WIN32)
	typedef __int64 INT64;
#endif
#if defined (__APPLE__) || defined (__unix)
	typedef long long INT64;
	typedef tstring::value_type TCHAR;
	typedef TCHAR _TCHAR;
#endif

	// converting part
	/*
	 * convert ascii encoded wstring to utf8 encoded string
	 */
	std::string wstrtostrutf8(const std::wstring& wstr);
	/*
	 * convert utf8 encoded string to wstring
	 * 1st, convert utf8 encoded string to native string
	 * 2nd, call strtowstr
	 */
	std::wstring strtowstrutf8(const std::string& str);
	
	std::string wstrtostr(const std::wstring& wstr);
	std::wstring strtowstr(const std::string& str);
	
#if defined (__APPLE__) || defined (__unix)
	std::string striconv(const std::string& input,
						 const std::string& to_code,
						 const std::string& from_code);
#endif

	// convert native encoded string to utf8 encoded string
	inline std::string utf8conv(std::string& strAscii)
	{
#if defined (WIN32)
		return wstrtostrutf8(strtowstr(strAscii));
#endif
#if defined (__APPLE__) || defined (__unix)
		// current only support GB
		return striconv(strAscii, "UTF-8", "GBK");
#endif
	}

	// convert utf8 encoded string to native encoded string
	inline std::string asciiconv(std::string& strUtf8)
	{
#if defined (WIN32)
		return wstrtostr(strtowstrutf8(strUtf8));
#endif
#if defined (__APPLE__) || defined (__unix)
		// current only support GB
		return striconv(strUtf8, "GBK", "UTF-8");
#endif
	}

	// convert JSON utf16 encoded string to native encoded string
	std::string asciiconvjson(std::string& strJsonUtf16);
	
	inline std::string tstrtostr(const tstring& tstr)
	{
#if defined(_UNICODE_HELPER)
		return wstrtostr(tstr);
#else
		return tstr;
#endif
	}
	
	inline std::wstring tstrtowstr(const tstring& tstr)
	{
#if defined(_UNICODE_HELPER)
		return tstr;
#else
		return strtowstr(tstr);
#endif
	}

	inline sunjwbase::tstring strtotstr(const std::string& str)
	{
#if defined(_UNICODE_HELPER)
		return strtowstr(str);
#else
		return str;
#endif
	}

	inline sunjwbase::tstring wstrtotstr(const std::wstring& wstr)
	{
#if defined(_UNICODE_HELPER)
		return wstr;
#else
		return wstrtostr(wstr);
#endif
	}

	inline sunjwbase::tstring strtotstrutf8(const std::string& str)
	{
#if defined(_UNICODE_HELPER)
		return strtowstrutf8(str);
#else
		return str;
#endif
	}

	inline std::string tstrtostrutf8(const sunjwbase::tstring& tstr)
	{
#if defined(_UNICODE_HELPER)
		return wstrtostrutf8(tstr);
#else
		return tstr;
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
		strequal_ci( const std::locale& loc ):m_loc(loc)
		{}
		
		bool operator()(charT ch1, charT ch2)
		{
			return std::toupper(ch1, m_loc) == std::toupper(ch2, m_loc);
		}
	private:
		const std::locale& m_loc;
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

	// start with & end with
	bool str_startwith(const std::string& str, const std::string& target);
	bool str_endwith(const std::string& str, const std::string& target);

}


#endif
