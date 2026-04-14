#include "stdafx.h"
#include <string.h>
#include "debug_util.h"
#include "string_util.h"


//char16_t to wchar_t
wchar_t * _wcsu16_utf162wcs(unsigned short * utf16, wchar_t *wcs, int len)
{
    if (sizeof(unsigned short) == sizeof(wchar_t))
    {
        memcpy(wcs, utf16, (len + 1) * sizeof(wchar_t));
        return wcs;
    }
    else
    {
        unsigned short 
        *end = utf16 + len;

        wchar_t * 
        ret = wcs;

        int       
        u32,
        u32low =0;

        while ( utf16 <end )
        {
                u32 =*utf16++;
            
                if ( u32>=0xD800 && u32<0xDC00 )
                {
                     u32low   =*utf16++;
                     u32     &=0x3FF;
                     u32low  &=0x3FF;
                     u32    <<=10;
                     u32     +=u32low;
                     u32     +=0x10000;
                }
                *wcs++ =u32;
        }
        *(wcs -1) =0;

        return ret;
    }
}

//-----------------------------------------------------------------------------
//
// Return Value : 
// Parameters   : 
// Remarks      : 
//
//-----------------------------------------------------------------------------
std::string wchar_to_UTF8(const wchar_t * in)
{
     std::string out;

     unsigned int codepoint = 0;
   
     for ( ; *in != 0; ++in )
     {
           if ( *in >= 0xd800 && *in <= 0xdbff )
                codepoint = ((*in - 0xd800) << 10) + 0x10000;
           else
           {
                if ( *in >= 0xdc00 && *in <= 0xdfff )
                     codepoint |= *in - 0xdc00;
                else
                     codepoint = *in;

                if ( codepoint <= 0x7f )
                     out.append(1, static_cast<char>(codepoint));

                else if  (codepoint <= 0x7ff )
                {
                     out.append(1, static_cast<char>(0xc0 | ((codepoint >> 6) & 0x1f)));
                     out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
                }
                else if ( codepoint <= 0xffff )
                {
                     out.append(1, static_cast<char>(0xe0 | ((codepoint >> 12) & 0x0f)));
                     out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
                     out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
                }
                else
                {
                     out.append(1, static_cast<char>(0xf0 | ((codepoint >> 18) & 0x07)));
                     out.append(1, static_cast<char>(0x80 | ((codepoint >> 12) & 0x3f)));
                     out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
                     out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
                }
                codepoint = 0;
           }
     }
     return out;
}

//-----------------------------------------------------------------------------
//
// Return Value : 
// Parameters   : 
// Remarks      : 
//
//-----------------------------------------------------------------------------
std::wstring UTF8_to_wchar(const char * in)
{
    std::wstring out;
    unsigned int codepoint;
    while (*in != 0)
    {
        unsigned char ch = static_cast<unsigned char>(*in);
        if (ch <= 0x7f)
            codepoint = ch;
        else if (ch <= 0xbf)
            codepoint = (codepoint << 6) | (ch & 0x3f);
        else if (ch <= 0xdf)
            codepoint = ch & 0x1f;
        else if (ch <= 0xef)
            codepoint = ch & 0x0f;
        else
            codepoint = ch & 0x07;
        ++in;
        if (((*in & 0xc0) != 0x80) && (codepoint <= 0x10ffff))
        {
            if (sizeof(wchar_t) > 2)
                out.append(1, static_cast<wchar_t>(codepoint));
            else if (codepoint > 0xffff)
            {
                out.append(1, static_cast<wchar_t>(0xd800 + (codepoint >> 10)));
                out.append(1, static_cast<wchar_t>(0xdc00 + (codepoint & 0x03ff)));
            }
            else if (codepoint < 0xd800 || codepoint >= 0xe000)
                out.append(1, static_cast<wchar_t>(codepoint));
        }
    }
    return out;
}

//-----------------------------------------------------------------------------
//
// Return Value : 
// Parameters   : 
// Remarks      : 
//
//-----------------------------------------------------------------------------
std::string WstringToString(const std::wstring str)
{
     unsigned 
     len = str.size() * 4;

     setlocale(LC_CTYPE, "");

     char 
     *p =new char[len];

     if ( p )
     {
          wcstombs(p,str.c_str(),len);

          std::string str1(p);
     
          delete[] p;

          return str1;
     }
     return "";
}

//-----------------------------------------------------------------------------
//
// Return Value : 
// Parameters   : 
// Remarks      : 
//
//-----------------------------------------------------------------------------
std::wstring utf8_to_utf16(const std::string& utf8)
{
     std::vector<unsigned long> unicode;
     size_t i = 0;

     while (i < utf8.size())
     {
             unsigned long uni;
             size_t todo;
             bool error = false;
             unsigned char ch = utf8[i++];
             if ( ch <= 0x7F )
             {
                  uni = ch;
                  todo = 0;
             }
             else if (ch <= 0xBF)
             {
                  {
                       //throw std::logic_error("not a UTF-8 string");
                       d_pintf("error :not a UTF-8 string\r\n");
                       return L"";
                  }
             }
             else if (ch <= 0xDF)
             {
                  uni = ch&0x1F;
                  todo = 1;
             }
             else if (ch <= 0xEF)
             {
                  uni = ch&0x0F;
                  todo = 2;
             }
             else if (ch <= 0xF7)
             {
                  uni = ch&0x07;
                  todo = 3;
             }
             else
             {
                  {
                       //throw std::logic_error("not a UTF-8 string");
                       #if defined(__DEBUG_MSG)
                       d_pintf("error :not a UTF-8 string\r\n");
                       #endif
                       return L"";
                  }
             }
             for ( size_t j = 0; j < todo; ++j )
             {
                   if ( i == utf8.size( ))
                   {
                        //throw std::logic_error("not a UTF-8 string");
                        d_pintf("error :not a UTF-8 string\r\n");
                        return L"";
                   }
                   unsigned char ch = utf8[i++];
                   if ( ch < 0x80 || ch > 0xBF )
                   {
                        //throw std::logic_error("not a UTF-8 string");
                        d_pintf("error :not a UTF-8 string\r\n");
                        return L"";
                   }
                   uni <<= 6;
                   uni += ch & 0x3F;
             }
             if ( uni >= 0xD800 && uni <= 0xDFFF )
             {
                  //throw std::logic_error("not a UTF-8 string");
                  d_pintf("error :not a UTF-8 string\r\n");
                  return L"";
             }
             if ( uni > 0x10FFFF )
             {
                  //throw std::logic_error("not a UTF-8 string");
                  d_pintf("error :not a UTF-8 string\r\n");
                  return L"";
             }
             unicode.push_back(uni);
     }
     std::wstring utf16;
     for ( size_t j = 0; j < unicode.size(); ++j )
     {
           unsigned long uni = unicode[j];
           if ( uni <= 0xFFFF )
           {
                utf16 += (wchar_t)uni;
           }
           else
           {
                uni -= 0x10000;
                utf16 += (wchar_t)((uni >> 10) + 0xD800);
                utf16 += (wchar_t)((uni & 0x3FF) + 0xDC00);
           }
     }
     return utf16;
}


