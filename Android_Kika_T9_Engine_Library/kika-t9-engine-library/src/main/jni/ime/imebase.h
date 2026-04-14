// imebase.h: interface for the imebase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMEBASE_H__766A3308_7E9F_45B0_8BB0_C92DFC4AC214__INCLUDED_)
#define AFX_IMEBASE_H__766A3308_7E9F_45B0_8BB0_C92DFC4AC214__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "phraseimp_v1.h"
#include "phraseimp_v2.h"
#include "phraseimp_v2_1.h"
#include "phraseimp_v2_1w.h"
#include "phraseimp_v2_2w.h"
#include "phraseimp_v3.h"

enum PHRS_IME_ID
{
     phrs_v1 =1,
     phrs_v2,
     phrs_v2_1,
     phrs_v2_1w,
     phrs_v2_2w,
     phrs_v3
};

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
class imebase :public phraseimp
{
private:
      phraseimp_v1           m_v1;
      phraseimp_v2           m_v2;
      phraseimp_v2_1         m_v2_1;
      phraseimp_v2_1w        m_v2_1w;
      phraseimp_v2_2w        m_v2_2w;
      phraseimp_v3           m_v3;
      phraseimp*             m_imeX;

public:
      imebase();
      virtual ~imebase();
      
      //-----------------------------------------------------------------------
      void attach(PHRS_IME_ID vID);

      void attach(imebase &_base);

      //-----------------------------------------------------------------------
      //
      //-----------------------------------------------------------------------
      bool isV1()
      {
           return m_imeX ==&m_v1;
      }
      bool isV2()
      {
           return m_imeX ==&m_v2;
      }
      bool isV2_1()
      {
           return m_imeX ==&m_v2_1;
      }
      bool isV2_1w()
      {
           return m_imeX ==&m_v2_1w;
      }
      bool isV2_2w()
      {
           return m_imeX ==&m_v2_2w;
      }
      bool isV2_base()
      {
           return m_imeX ==&m_v2 || 
                  m_imeX ==&m_v2_1 || 
                  m_imeX ==&m_v2_1w ||
                  m_imeX ==&m_v2_2w;
      }
      bool isV3()
      {
           return m_imeX ==&m_v3;
      }

      //-----------------------------------------------------------------------
      //
      //-----------------------------------------------------------------------
      phraseimp_v1* v1()
      {
           return &m_v1;
      }
      phraseimp_v2* v2()
      {
           return &m_v2;
      }
      phraseimp_v2_1* v2_1()
      {
           return &m_v2_1;
      }
      phraseimp_v2_1w* v2_1w()
      {
           return &m_v2_1w;
      }
      phraseimp_v2_2w* v2_2w()
      {
           return &m_v2_2w;
      }
      phraseimp_v3* v3()
      {
           return &m_v3;
      }

      //-----------------------------------------------------------------------
      //
      //-----------------------------------------------------------------------
      phraseimp* operator ->()
      {
           return m_imeX;
      }
      
      //-----------------------------------------------------------------------
      void exportDict(const char *dictName);

      bool loadDict(const char *dictName);
};

#endif // !defined(AFX_IMEBASE_H__766A3308_7E9F_45B0_8BB0_C92DFC4AC214__INCLUDED_)
