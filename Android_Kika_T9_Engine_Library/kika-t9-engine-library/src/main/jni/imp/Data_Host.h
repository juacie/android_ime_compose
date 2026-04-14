// Data_Host.h: interface for the CData_Host class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATA_HOST_H__BDACB4DA_BB17_4FD4_AE9C_C672589ABE37__INCLUDED_)
#define AFX_DATA_HOST_H__BDACB4DA_BB17_4FD4_AE9C_C672589ABE37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define KERNEL_VER           "IQQIV2.2.20150820.1.ADNW"
#define KERNEL_VCODE         "IQQIV2.2.20170914.1.ADNW"

#define PORT                 80

#define HOST_NAME_OEM        (char*)"tp-oem.kika-backend.com"
#define HOST_NAME_DEV        (char*)"dc-dev.kika-backend.com"

#if defined(__SMTV)

 #define APPKEY              (char*)"809d13bf7af35690621b2bb270cbfe1b"

 #if defined(__CH_TCL)
   #define APPCH             (char*)"LX-TCL0-0000"
 #elif defined(__CH_HISENSE)
   #define APPCH             (char*)"LX-HISE-0000"
 #elif defined(__CH_SHIJIU)
   #define APPCH             (char*)"LX-SJTV-0000"
 #else
   #define APPCH             (char*)"LX-TEST-0000"
 #endif

#elif defined(__FPHONE)

 #define APPKEY              (char*)"00e7fa1cda4fed1af3febfbdf3a6e9ad"

 #if defined(__CH_KAIOS)
   #define APPCH             (char*)"LX-KAI0-0000"
 #else
   #define APPCH             (char*)"LX-TEST-0000"
 #endif

#else // other device

 #define APPKEY              (char*)"f3b7ba23c87a572717eed1ff7be0b9ca"

 #if defined(__CH_DELTA)
   #define APPCH             (char*)"LX-DLTA-0000"
 #elif defined(__CH_PSONIC)
   #define APPCH             (char*)"LX-PAIC-0000"
 #elif defined(__CH_ELEAD)
   #define APPCH             (char*)"LX-ELED-0000"
 #else
   #define APPCH             (char*)"LX-TEST-0000"
 #endif

#endif


#endif // !defined(AFX_DATA_HOST_H__BDACB4DA_BB17_4FD4_AE9C_C672589ABE37__INCLUDED_)
