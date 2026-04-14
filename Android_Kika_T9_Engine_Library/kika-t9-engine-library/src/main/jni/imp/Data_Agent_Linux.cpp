// Data_Agent_Linux.cpp: implementation of the CData_Agent_Linux class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <algorithm>
#include "Data_Host.h"
#include "Data_Agent_Linux.h"


//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : Construction/Destruction
//
//-----------------------------------------------------------------------------
CData_Agent_Linux::CData_Agent_Linux()
{
     
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
CData_Agent_Linux::~CData_Agent_Linux()
{
     
}


//-----------------------------------------------------------------------------
//
// Return Value : 
// Parameters   : 
// Remarks      : 
//
//-----------------------------------------------------------------------------
#if defined(__LINUX)


#include <unistd.h>
#include <ctime>
#include <errno.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/sockios.h>
#include <sys/resource.h>
#include <sys/utsname.h>

#if defined(__XWND)
 #include <X11/Xlib.h>
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
INT  getScreenSize(INT *w, INT *h)
{
/*
     #if defined(__XWND)

     Display* 
     pdsp =XOpenDisplay( NULL );
     
     if ( !pdsp )
     {
          //fprintf(stderr, "Failed to open default display.\n");
          return -1;
     }
     
     Screen* 
     pscr =DefaultScreenOfDisplay( pdsp );
     
     if ( !pscr )
     {
          //fprintf(stderr, "Failed to obtain the default screen of given display.\n");
          return -2;
     }
     
     *w =pscr->width;
     *h =pscr->height;
     
     XCloseDisplay( pdsp );
 
     #else
*/
     *w =0;
     *h =0;

//     #endif

     return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
char* strlwrX(char* s)
{
     char*
     tmp = s;

     for ( ;*tmp; ++tmp )
     {
           *tmp = tolower((unsigned char) *tmp);
     }
     return s;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
const char* getMachineName()
{
     static struct utsname
     u;
     
     if ( uname( &u ) < 0 )
     {
          //assert(0);
          return "unknown";
     }
     
     return u.nodename;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
unsigned short getVolumeHash()
{
     /*
     // we don't have a 'volume serial number' like on windows. Lets hash the system name instead.
     unsigned char*
     sysname = (unsigned char*)getMachineName();
     
     unsigned short
     hash = 0;
     
     for ( unsigned int i = 0; sysname[i]; i++ )
     {
           hash += ( sysname[i] << (( i & 1 ) * 8 ));
     }
     return hash;
     */
     return 0;
}



//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
static void getCpuid( unsigned int* p, unsigned int ax )
{
     /*
     #if defined(__ASM)
     
     __asm __volatile
     (   
     "movl %%ebx, %%esi\n\t"
     "cpuid\n\t"
     "xchgl %%ebx, %%esi"
     : "=a" (p[0]), "=S" (p[1]),
     "=c" (p[2]), "=d" (p[3])
     : "0" (ax)
     );
     
     #endif
     */
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
unsigned short getCpuHash()
{
     unsigned
     int cpuinfo[4] = { 0, 0, 0, 0 };
     
     getCpuid( cpuinfo, 0 );
     
     unsigned short hash = 0;
     unsigned int* ptr = (&cpuinfo[0]);
     
     for ( unsigned int i = 0; i <4; i++ )
     {
           hash += (ptr[i] & 0xFFFF) + ( ptr[i] >> 16 );
     }
     return hash;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
static void smear( UINT* id )
{
     UINT
     mask[5] = { 0x4e25, 0xf4a1, 0x5437, 0xab41, 0x0000 };
     
     for ( long i = 0; i < 5; i++ )
     {
           for ( long j = i; j < 5; j++ )
           {
                 if ( i != j )
                 {
                      id[i] ^= id[j];
                 }
           }
     }
     
     for ( long i = 0; i < 5; i++ )
     {
           id[i] ^= mask[i];
     }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
static void unsmear( UINT* id )
{
     UINT
     mask[5] = { 0x4e25, 0xf4a1, 0x5437, 0xab41, 0x0000 };
     
     for ( long i = 0; i <5; i++ )
     {
           id[i] ^= mask[i];
     }
     
     for ( long i = 0; i <5; i++ )
     {
           for ( long j = 0; j <i; j++ )
           {
                 if ( i != j )
                 {
                      id[4-i] ^=id[4-j];
                 }
           }
     }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
UINT hashMacAddress( unsigned char* mac )
{
     UINT
     hash = 0;
     
     for ( long i =0; i <6; i++ )
     {
           hash += ( mac[i] << (( i & 1 ) * 8 ));
     }
     return hash;
}


//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void getMacHash( UINT& mac1, UINT& mac2 )
{
     /*
     mac1 = 0;
     mac2 = 0;
     
     int
     sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP );
     
     if ( sock < 0 )
     {
          return;
     }
     
     // enumerate all IP addresses of the system
     char
     ifconfbuf[ 128 * sizeof(struct ifreq)  ];
     
     memset( ifconfbuf, 0, sizeof( ifconfbuf ));
     
     struct ifconf
     conf;
     
     conf.ifc_buf = ifconfbuf;
     conf.ifc_len = sizeof( ifconfbuf );
     
     if ( ioctl( sock, SIOCGIFCONF, &conf ))
     {
          //assert(0);
          return;
     }
     
     // get MAC address
     bool foundMac1 = false;
     
     struct ifreq* ifr;
     
     for ( ifr = conf.ifc_req; (unsigned char*)ifr < (unsigned char*)conf.ifc_req + conf.ifc_len; ifr++ )
     {
           if ( ifr->ifr_addr.sa_data == (ifr +1)->ifr_addr.sa_data )
           {
                continue;  // duplicate, skip it
           }
           
           if ( ioctl( sock, SIOCGIFFLAGS, ifr ))
           {
                continue;  // failed to get flags, skip it
           }
           
           if ( ioctl( sock, SIOCGIFHWADDR, ifr ) == 0 )
           {
                if ( !foundMac1 )
                {
                     foundMac1 = true;
                     
                     mac1 = hashMacAddress( (unsigned char*)&(ifr->ifr_addr.sa_data));
                }
                else
                {
                     mac2 = hashMacAddress( (unsigned char*)&(ifr->ifr_addr.sa_data));
                     break;
                }
           }
     }
     close( sock );
     */
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
static UINT* computeSystemUniqueId()
{
     /*
     static UINT id[5];
     static bool computed = false;
     
     if ( computed )
     {
          return id;
     }
     
     // produce a number that uniquely identifies this system.
     id[0] = getCpuHash();
     id[1] = getVolumeHash();
     
     getMacHash( id[2], id[3] );
     
     // fifth block is some checkdigits
     // fifth block is some checkdigits
     
     id[4] = 0;
     
     for ( long i =0; i <4; i++ )
     {
           id[4] += id[i];
     }
     smear( id );
     
     computed = true;
     
     return id;
     */
     return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
#define MAX_UUID_LEN                   36
#define MAX_UUID_BUF                   256

void GetRandomGUID(LPAgent_Data_Init dui)
{
     /*
     char
     strUuid[MAX_UUID_BUF];

     FILE
     *fp =fopen("/tmp/klg.guid", "r");

     if ( fp !=NULL )
     {
         fgets(strUuid, MAX_UUID_BUF, fp);
         fclose(fp);

         int
         slen =strlen(strUuid);

         if ( strlen(strUuid) ==MAX_UUID_LEN )
         {
              dui->d_duid.set(strUuid);
              return;
          }
     }

     srand(time(NULL));

     sprintf(strUuid, 
             "%x%x%x%x%x%x%x%x",
             rand(), rand(),                 // Generates a 64-bit Hex number
             rand(),                         // Generates a 32-bit Hex number
             ((rand() & 0x0fff) | 0x4000),   // Generates a 32-bit Hex number of the form 4xxx (4 indicates the UUID version)
             rand() % 0x3fff + 0x8000,       // Generates a 32-bit Hex number in the range [0x8000, 0xbfff]
             rand(), rand(), rand());        // Generates a 96-bit Hex number

     if ( strlen(strUuid) >MAX_UUID_LEN )
     {
          strUuid[MAX_UUID_LEN] =NULL;
     }

     fp =fopen("/tmp/klg.guid", "w");

     if ( fp !=NULL )
     {
         fputs(strUuid, fp);

         dui->d_duid.set(strUuid);

         fclose(fp);
     }
     */
}

//-----------------------------------------------------------------------------
//
// Return Value : 
// Parameters   : 
// Remarks      : 
//
//-----------------------------------------------------------------------------
void GetStdoutFromCommand(LPAgent_Data_Init dui)
{
     /*
     try
     {
          string data;
          string data_last;
          
          string cmd ="ls /dev/disk/by-uuid";
          
          char buffer[MAX_UUID_BUF];
          
          cmd.append(" 2>&1");
          
          FILE *
          stream = popen(cmd.c_str(), "r");
          
          if ( stream )
          {
               while ( !feof(stream) )
               {
                       if ( fgets(buffer, MAX_UUID_BUF, stream ) != NULL )
                       {
                            data.clear();
                            data.append(buffer);
                            
                            data.erase(data.length() -1, 1);
                            
                            if ( data.length() >=MAX_UUID_LEN )
                            {
                                 data_last.append(buffer);
                                 break;
                            }
                       }
               }
               pclose(stream);
          }
          
          if ( data_last.length() >MAX_UUID_LEN )
          {
               data.clear();
               data.append(data_last.c_str());
               
               int
               ExLen =data.length() -MAX_UUID_LEN;
               
               data.erase(MAX_UUID_LEN, data.length() -ExLen);
          }

          LPCSTR
          sdata =data.c_str();

          LPSTR
          errFound =strstr(strlwr((LPSTR)sdata), "no such file");
          
          if ( errFound ==NULL )
          {
               dui->d_duid.set(sdata);
          }
     }
     catch ( exception ex )
     {
     }
     */
}

//-----------------------------------------------------------------------------
//
// Return Value : 
// Parameters   : 
// Remarks      : 
//
//-----------------------------------------------------------------------------
#if defined(__USE_MACH_ID)
void GetStdoutFromMachineID(LPAgent_Data_Init dui)
{
     /*
     try
     {
          FILE *
          fp =fopen("/etc/machine-id", "rb");
          
          if ( fp !='\0' )
          {
               char *arg = 0;
               size_t size = 0;
               
               while ( getdelim(&arg, &size, 0, fp) !=-1 )
               {
                       dui->d_duid.set(arg);
               }
               
               delete arg;
               fclose(fp);
          }
      }
      catch ( exception ex )
      {
      }
      */
}
#endif

//-----------------------------------------------------------------------------
//
// Return Value : 
// Parameters   : 
// Remarks      : 
//
//-----------------------------------------------------------------------------
void GetDUID(LPAgent_Data_Init dui)
{
     /*
     #if defined(__FIX_GUID)
     GetRandomGUID(dui);
     #endif

     if ( dui->d_duid.getLen() ==0 )
     {
          GetStdoutFromCommand(dui);

          if ( dui->d_duid.getLen() ==0 )
          {
               #if defined(__USE_MACH_ID)
               GetStdoutFromMachineID(dui);
               #endif

               if ( dui->d_duid.getLen() ==0 )
               {
                    char
                    sData[120];

                    sprintf(sData, "%d", (int)computeSystemUniqueId());
          
                    dui->d_duid.set(sData);
               }
          }
     }
     if ( dui->d_duid.getLen() !=0 )
     {
          string s =dui->d_duid.getMem();

          //replace( s.begin(), s.end(), '-', ' ' );
          char chars[] = "()-";

          for ( unsigned int i = 0; i < strlen(chars); ++i )
          {
                s.erase (std::remove(s.begin(), s.end(), chars[i]), s.end());
          }
          dui->d_duid.set(s.c_str());
     }
     */
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void CData_Agent_Linux::init()
{

/*
     char
     sData[120];
     
     //------------------------------------------------------------------------
     GetDUID(&dui);

     //------------------------------------------------------------------------
     struct utsname
     info;
     
     uname(&info);
     
     //------------------------------------------------------------------------
     sprintf(sData, "%d.%d.%d\n",__GNUC__,__GNUC_MINOR__,__GNUC_PATCHLEVEL__);
     
     dui.d_sdk_version.set(sData);
     
     //------------------------------------------------------------------------
     dui.d_language.set("");
     dui.d_appKey.set(APPKEY);
     
     dui.d_app_version.set(KERNEL_VER);
     dui.d_agt_version.set(KERNEL_VER);
     dui.d_os_version.set(info.release);
     
     dui.d_op.set(info.nodename);
     dui.d_pf.set(info.sysname);
     dui.d_ch.set(APPCH);
     
     //------------------------------------------------------------------------
     INT
     wndW, wndH;
     
     if ( getScreenSize(&wndW, &wndH) ==0 )
     {
          sprintf(sData, "%dx%d", wndW, wndH);
     }
     else
     {
          sprintf(sData, "000x000");
     }
     dui.d_resolution.set(sData);
  
     //------------------------------------------------------------------------
     #if defined(__GPP)

     time_t 
     now;
     time(&now);

     struct tm*
     tm_now =localtime(&now);
 
     time_t
     new_time =mktime(tm_now);

     sprintf(sData, "%ld", new_time);
     
     dui.d_ts.set(sData);
     dui.d_firstIns_ts.set(sData);

     #else

     dui.d_ts.set("");
     dui.d_firstIns_ts.set("");

     #endif
     
     //------------------------------------------------------------------------
     dui.d_firstIns_ap_ver.set(dui.d_app_version.getMem());
     
     //------------------------------------------------------------------------
     #if defined(__GPP)

     char
     *sLang =setlocale (LC_ALL, NULL);
     
     char
     *sLangEx =strstr(sLang, ".UTF-8");
     
     if ( sLangEx !='\0' )
     {
          sLangEx[0] ='\0';
     }
     dui.d_lang.set(sLang);

     #else

     dui.d_lang.set("");

     #endif

*/
}

//-----------------------------------------------------------------------------
//
// Return Value : 
// Parameters   : 
// Remarks      : 
//
//-----------------------------------------------------------------------------
#else

void CData_Agent_Linux::init()
{
x
}

#endif
