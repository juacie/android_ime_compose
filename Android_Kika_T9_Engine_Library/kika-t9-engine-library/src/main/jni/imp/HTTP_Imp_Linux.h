// HTTP_Imp_Linux.h: interface for the CHTTP_Imp_Linux class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTTP_IMP_LINUX_H__BA0E960F_B002_454E_AB70_1C6FD6E9C0A1__INCLUDED_)
#define AFX_HTTP_IMP_LINUX_H__BA0E960F_B002_454E_AB70_1C6FD6E9C0A1__INCLUDED_

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
#define MAXLINE              4096


#define SEND_RQ(MSG)         write(sockfd, MSG, strlen(MSG));
#define SEND_RQ2(MSG)        send(sockfd, MSG, strlen(MSG), 0);


//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
class CHTTP_Imp_Linux :public CHTTP_Imp
{
public:
      CHTTP_Imp_Linux();
      virtual ~CHTTP_Imp_Linux();

      INT  loadData();

      void saveData();

      void removeData();
      
      INT  onPost(LPCSTR duid, LPCBYTE LstrData, INT iLen);

      void onPostThread(LPCSTR duid, LPCBYTE LstrData, INT iLen, BOOL always =false);
};

#endif // !defined(AFX_HTTP_IMP_LINUX_H__BA0E960F_B002_454E_AB70_1C6FD6E9C0A1__INCLUDED_)
