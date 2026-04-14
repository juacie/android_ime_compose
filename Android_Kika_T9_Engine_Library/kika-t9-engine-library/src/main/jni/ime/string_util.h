#if !defined(AFX_STR_UTILS_H__6631A387_285C_469B_9B73_AB78C12301F5__INCLUDED_)
#define AFX_STR_UTILS_H__6631A387_285C_469B_9B73_AB78C12301F5__INCLUDED_


#if defined(_MSC_VER) && _MSC_VER < 1300
 #pragma comment(linker, "/IGNORE:4786")
 #pragma warning(disable:4786)
#endif

#include <map>
#include <set>
#include <vector>
#include <string>
#include <clocale>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

using namespace std;

#if defined(_WIN32)
using namespace std::tr1;
#endif

std::wstring UTF8_to_wchar(const char * in);

std::string wchar_to_UTF8(const wchar_t * in);

std::string  WstringToString(const std::wstring str);

std::wstring utf8_to_utf16(const std::string& utf8);

wchar_t * _wcsu16_utf162wcs(unsigned short * utf16, wchar_t *wcs, int len);

#endif //AFX_STR_UTILS_H__6631A387_285C_469B_9B73_AB78C12301F5__INCLUDED_

