/*
 * strhelpe implementation file
 * Author: Sun Junwen
 * Version: 1.2.1
 * Provides converting from tstring, string and wstring to each other
 * And provides string's utf8 converting.
 * Provides triming function to string and wstring.
 * Provides replacing function to string and wstring.
 * Provides newline fixing to string.
 */
#include "stdafx.h"

#include <string>
#include <algorithm>

#include "Windows.h"

#include "strhelper.h"

/*
 * 将 ascii 编码的 wstring 转换为 utf8 编码的 string
 */
std::string sunjwbase::wstrtostrutf8(const std::wstring& wstr)
{
    // Convert a Unicode string to an ASCII string
	size_t strLen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), NULL, 0, NULL, NULL);
    std::string strTo;
    char *szTo = new char[strLen + 1];
    szTo[strLen] = '\0';
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, szTo, (int)strLen, NULL, NULL);
    strTo = szTo;
    delete[] szTo;
    return strTo;
}

/*
 * 将 utf8 编码的 string 转换为 wstring
 * 如果需要把 utf8 编码的 string 转换为本地编码的 string
 * 再调用一次 wstrtostr
 */
std::wstring sunjwbase::strtowstrutf8(const std::string& str)
{
    // Convert an ASCII string to a Unicode String
    std::wstring wstrTo;
    wchar_t *wszTo = new wchar_t[str.length() + 1];
    wszTo[str.size()] = L'\0';
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wszTo, (int)str.length());
    wstrTo = wszTo;
    delete[] wszTo;
    return wstrTo;
}

std::string sunjwbase::wstrtostr(const std::wstring& wstr)
{
    // Convert a Unicode string to an ASCII string
	size_t strLen = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), (int)wstr.length(), NULL, 0, NULL, NULL);
    std::string strTo;
    char *szTo = new char[strLen + 1];
    szTo[strLen] = '\0';
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, szTo, (int)strLen, NULL, NULL);
    strTo = szTo;
    delete[] szTo;
    return strTo;
}

std::wstring sunjwbase::strtowstr(const std::string& str)
{
    // Convert an ASCII string to a Unicode String
    std::wstring wstrTo;
    wchar_t *wszTo = new wchar_t[str.length() + 1];
    wszTo[str.size()] = L'\0';
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wszTo, (int)str.length());
    wstrTo = wszTo;
    delete[] wszTo;
    return wstrTo;
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

