// HTTP_Imp_Wnd.h: interface for the CHTTP_Imp_Wnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTTP_IMP_WND_H__E0EB1544_E0E7_439C_92F6_3D9BBC88F795__INCLUDED_)
#define AFX_HTTP_IMP_WND_H__E0EB1544_E0E7_439C_92F6_3D9BBC88F795__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Data_Type.h"
#include "HTTP_Imp.h"


//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
class CHTTP_Imp_Wnd :public CHTTP_Imp
{
public:
      CHTTP_Imp_Wnd();
      virtual ~CHTTP_Imp_Wnd();
      
      INT  loadData();

      void saveData();

      void removeData();

      INT  onPost(LPCSTR duid, LPCBYTE LstrData, INT iLen);

      void onPostThread(LPCSTR duid, LPCBYTE LstrData, INT iLen, BOOL always =false) { }
};

#endif // !defined(AFX_HTTP_IMP_WND_H__E0EB1544_E0E7_439C_92F6_3D9BBC88F795__INCLUDED_)
