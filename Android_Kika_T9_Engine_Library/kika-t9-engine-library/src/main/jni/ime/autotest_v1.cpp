// autotest_v1.cpp: implementation of the autotest_v1 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "autotest_v1.h"

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : Construction/Destruction
//
//-----------------------------------------------------------------------------
autotest_v1::autotest_v1()
{
    lang.name.empty();
    lang.count =-1;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
autotest_v1::~autotest_v1()
{
     
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
std::vector<std::wstring> autotest_v1::split(const std::string& text, const std::string& delims)
{
     std::vector<std::wstring>
     tokens;
     
     int
     start =text.find_first_not_of(delims), end =0;
     
     while ( (end =text.find_first_of(delims, start)) !=std::string::npos )
     {
             wstring
             ws =utf8_to_utf16(text.substr(start, end -start));

             tokens.push_back(ws);
             
             start =text.find_first_not_of(delims, end);
     }
     if ( start !=std::string::npos )
     {
          wstring
          ws =utf8_to_utf16(text.substr(start));

          tokens.push_back(ws);
     }
     
     return tokens;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
std::vector<std::string> autotest_v1::splitGet(const std::string& text, const std::string& delims)
{
     std::vector<std::string>
     tokens;
     
     int
     start =text.find_first_not_of(delims), end =0;
     
     if ( start !=std::string::npos )
     {
          string 
          sValue = text.substr(start, text.size() -start);

          tokens.push_back(sValue);
     }
     
     return tokens;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
std::vector<std::string> autotest_v1::splitCH(const std::string& text, const char seperator)
{
     std::vector<std::string>
     output;
     
     std::string::size_type
     prev_pos = 0,
     pos = 0;
     
     while ( (pos =text.find(seperator, pos)) !=std::string::npos )
     {
             std::string
             substring( text.substr(prev_pos, pos-prev_pos) );
             
             output.push_back(substring);
             
             prev_pos = ++pos;
     }
     
     output.push_back(text.substr(prev_pos, pos-prev_pos)); // Last word
     
     return output;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void autotest_v1::action(const std::string& text, const std::string& delims)
{
     vector<string>::iterator
     ii;
     
     if ( text.c_str()[0] ==0x9 )
     {
          return;
     }
     if (text.find("ime=") !=std::string::npos) 
     {
         if ( lang.count !=-1 )
         {
              if ( lang.count ==0 )
              {
                   lang.count =20;
              }
              mKeys.push_back(lang);
              lang.keys.resize(0);
              lang.name.empty();
              lang.count =0;
         }
         std::vector<std::string>
         out =splitGet(text, "ime=");

         if ( out.size() >=1 )
         {
              ii =out.begin();
              ii->erase(std::remove(ii->begin(), ii->end(), '\t'), ii->end());

              lang.id =atoi((*ii).c_str());
         }
         return;
     }
     else if (text.find("lang=") !=std::string::npos) 
     {
         if ( lang.count !=-1 )
         {
              if ( lang.count ==0 )
              {
                   lang.count =20;
              }
         }
         std::vector<std::string>
         out =splitGet(text, "lang=");

         if ( out.size() >=1 )
         {
              ii =out.begin();
              ii->erase(std::remove(ii->begin(), ii->end(), '\t'), ii->end());

              lang.name =*ii;
         }
         return;
     }
     else if (text.find("count=") !=std::string::npos) 
     {
         std::vector<std::string>
         out =splitGet(text, "count=");

         if ( out.size() >=1 )
         {
              ii =out.begin();

              lang.count =atoi((*ii).c_str());
         }
         return;
     }
     if (text.find("Length") !=std::string::npos) 
     {
         return;
     }

     std::vector<std::wstring>
     out =split(text, delims);
     
     vector<wstring>::iterator
     wii;
     wii =out.begin();
     
     KEY_ANS
     kans;

     for ( int idx =0; wii !=out.end(); wii++, idx++ )
     {
           switch ( idx )
           {
                    case 0:
                         break;
                    case 1:
                         kans.key =*wii;
                         break;

                    default:
                         kans.ans.push_back(*wii);
                         break;
           }
     }
     lang.keys.push_back(kans);
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void autotest_v1::actionDone()
{
     if ( lang.count >0 )
     {
          mKeys.push_back(lang);
          lang.name.empty();
          lang.count =0;
     }
}
