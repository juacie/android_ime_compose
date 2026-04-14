// HTTP_Imp.cpp: implementation of the CHTTP_Imp_Linux class.

#include "stdafx.h"
#include "HTTP_Imp_Linux.h"


#if defined(__LINUX)

#include <pthread.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <cstdio>
#include <ctime>


#include "Data_Type.h"
#include "Data_Host.h"
#include "ClientMeta.h"
#include "Timer_Linux.h"


//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
static void LogMsg(char *sMsg)
{
/*
     #if defined(__DEBUG) || defined(_DEBUG)
     printf(sMsg);
     #elif defined(__TPOEM)
     #else
     printf(sMsg);
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
static void LogMsg2(char *sMsg)
{
/*
     #if defined(__DEBUG) || defined(_DEBUG)
     printf(sMsg);
     #elif defined(__TPOEM)
     #else
     printf(sMsg);
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
CHTTP_Imp_Linux::CHTTP_Imp_Linux()
{
/*
    std::time_t
    now =std::time(0);

    std::strftime(sDayCurr, 42, "%Y%m%d", localtime(&now));

    strcpy(sLogPath, "/tmp");
*/
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
CHTTP_Imp_Linux::~CHTTP_Imp_Linux()
{
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void CHTTP_Imp_Linux::saveData()
{
/*
     char
     sLogFile[MAX_PATH_LEN] ={0};
     sprintf(sLogFile, "%s/klg.dat", sLogPath);

     FILE
     *fp =fopen(sLogFile, "w");

     if ( fp !=NULL )
     {
          param.iSucc =1;
          putw((param.iSucc ==1? 1: 0), fp);

          std::time_t
          now =std::time(0);

          char sData[40];
          std::strftime(sData, 42, "%Y%m%d", localtime(&now));

          fputs(sData, fp);

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
INT  CHTTP_Imp_Linux::loadData()
{
/*
     char sDayNow[40]  ={0};
     char sDayLast[40] ={0};

     std::time_t
     now =std::time(0);

     std::strftime(sDayNow, 42, "%Y%m%d", localtime(&now));

     char
     sLogFile[MAX_PATH_LEN] ={0};
     sprintf(sLogFile, "%s/klg.dat", sLogPath);

     FILE
     *fp =fopen(sLogFile, "r");

     if ( fp !=NULL )
     {
          int
          iSucc =getw(fp);

          fgets(sDayLast, 40, fp);
          fclose(fp);

          param.iSucc =(iSucc !=1) ? 0 :1;

          // Hisense always base on (iSucc) flag
          #if !defined(__CH_HISENSE)

          if ( strcmp(sDayLast, sDayNow ) !=0 )
          {
               param.iSucc =0;
               return 1;
          }
          return 0;

          #else

          if ( param.iSucc ==0 && strcmp(sDayLast, sDayNow ) !=0 )
          {
               param.iSucc =0;
               return 1;
          }
          return 0;

          #endif
     }
     else
     {
         // Hisense always base on date (02, 12, 22)
         #if !defined(__CH_HISENSE)
         if ( strcmp(sDayNow, sDayCurr ) !=0 )
         {
              param.iSucc =0;

              strcpy(sDayCurr, sDayNow);
              return 1;
         }
         #else

         char sDay[2] ={0};

         if ( strlen(sDayNow) >=8 )
         {
              strncpy(sDay, sDayNow +6, 2);
         }

         if ( param.iSucc ==0 && (strncmp(sDay, "02", 2) ==0 || strncmp(sDay, "17", 2) ==0) )
         {
              param.iSucc =0;
              return 1;
         }

         #endif
         return 0;
     }
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
void CHTTP_Imp_Linux::removeData()
{
/*     char
     sLogFile[MAX_PATH_LEN] ={0};
     sprintf(sLogFile, "%s/klg.dat", sLogPath);

     std::remove(sLogFile);
*/
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
#if defined(__GPP)
/*
std::string iso_8859_1_to_utf8(std::string str)
{
     string 
     strOut;

     for ( std::string::iterator it =str.begin( ); it !=str.end(); ++it)
     {
           uint8_t ch =*it;
           if ( ch < 0x80 )
           {
                strOut.push_back(ch);
           }
           else
           {
                strOut.push_back(0xc0 | ch >> 6);
                strOut.push_back(0x80 | (ch & 0x3f));
           }
     }
     return strOut;
}
*/
#endif


//-----------------------------------------------------------------------------
//
// Return Value : 
// Parameters   : 
// Remarks      : 
//
//-----------------------------------------------------------------------------
bool IsConnected()
{
/*
     struct
     addrinfo hints;
     memset(&hints, 0, sizeof hints);
     
     hints.ai_family   =AF_UNSPEC; // AF_INET or AF_INET6 to force version
     hints.ai_socktype =SOCK_STREAM;
     
     struct addrinfo
     *res, *p;
     
     int
     status =getaddrinfo(HOST_NAME_OEM, "80", &hints, &res);
     
     if ( status !=0)
     {
          return false;
     }
     
     char
     ipstr[INET6_ADDRSTRLEN];
     
     for (  p =res; p !=NULL; p =p->ai_next )
     {
           void *addr;
           char *ipver;
           
           // get the pointer to the address itself,
           // different fields in IPv4 and IPv6:
           if ( p->ai_family ==AF_INET ) // IPv4
           {
                struct sockaddr_in
                *ipv4 =(struct sockaddr_in *)p->ai_addr;
                
                addr =&(ipv4->sin_addr);
                ipver =(char*) "IPv4";
           }
           else // IPv6
           {
                struct sockaddr_in6 *
                ipv6 =(struct sockaddr_in6 *)p->ai_addr;
                
                addr =&(ipv6->sin6_addr);
                ipver =(char*) "IPv6";
           }
           
           // convert the IP to a string and print it:
           //inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
           
           //printf("  %s: %s\n", ipver, ipstr);
     }
     
     bool
     conn =false;
     
     int
     sockfd =socket(res->ai_family, res->ai_socktype, res->ai_protocol);
     
     if ( connect(sockfd, res->ai_addr, res->ai_addrlen) ==-1 )
     {
          conn =false;
     }
     else
     {
          conn =true;
     }
     close(sockfd);
     
     freeaddrinfo(res); // free the linked list
     
     return conn;
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
INT  CHTTP_Imp_Linux::onPost(LPCSTR duid, LPCBYTE sData, INT iLen)
{
/*
     if ( IsConnected() ==false )
     {
          return 0;
     }

     char
     response[MAXLINE];
     
     // create the socket
     int
     sockfd =socket(AF_INET, SOCK_STREAM, 0);
     
     if ( sockfd <0 )
     {
          LogMsg((char *)"ERROR opening socket....\r\n");
          return 0;
     }
     

     // lookup the ip address
     LPSTR
     mstrHost =NULL;

     if ( bDebug ==FALSE )
     {
          mstrHost =HOST_NAME_OEM;
     }
     else
     {
          #if defined(__DEBUG) || defined(_DEBUG)
          mstrHost =HOST_NAME_DEV;
          #elif defined(__TPOEM)
          mstrHost =HOST_NAME_OEM;
          #else
          mstrHost =HOST_NAME_DEV;
          #endif
     }

     struct hostent *
     server =gethostbyname(mstrHost);
     
     if ( server ==NULL )
     {
          close(sockfd);
          LogMsg((char *)"ERROR, no such host\r\n");
          return 0;
     }
     LogMsg((char *)mstrHost);
     
     char sPort[120];
     sprintf(sPort, "port=%d\r\n", PORT);
     LogMsg((char *)sPort);
     
     // fill in the structure
     struct sockaddr_in
     serv_addr;
     
     memset(&serv_addr, 0, sizeof(serv_addr));
     
     serv_addr.sin_family =AF_INET;
     serv_addr.sin_port   =htons(PORT);
     
     memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
     
     // connect the socket
     if ( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <0 )
     {
          close(sockfd);
          LogMsg((char *)"ERROR connecting\r\n");
          return 0;
     }
     
     //------------------------------------------------------------------------
     char sLen[120];
     sprintf(sLen, "Content-length: %d\r\n", iLen);
     LogMsg((char *)sLen);
     
     char sHost[120];
     sprintf(sHost, "Host: %s\r\n", mstrHost);
     LogMsg((char *)sHost);
     
     char sPostAPI[512];
     sprintf(sPostAPI, "POST /api.php?type=meta&duid=%s HTTP/1.0\r\n", duid);
     LogMsg((char *)sPostAPI);
     
     //------------------------------------------------------------------------
     ///LogMsg((char *)"\r\n------------------------------------------------------------------------\r\n");
     
     char sendline[MAXLINE];
     
     strcpy(sendline, sPostAPI);
     strcat(sendline, sHost);
     strcat(sendline, "Connection: keep-Alive\r\n");
     strcat(sendline, "Accept-Language: en-US,en;q=0.8\r\n");
     //strcat(sendline, "Accept-Encoding: gzip,deflate,sdch\r\n");
     strcat(sendline, "Content-Type: application/octet-stream; charset=UTF-8\r\n");
     strcat(sendline, sLen);
     strcat(sendline, "\r\n");
     
     write(sockfd, sendline, strlen(sendline));
     
     LogMsg((char *)sendline);
     
     //------------------------------------------------------------------------
     // send the request
     int
     total =iLen;
     
     int
     sent =0;
     
     char
     cData[MAXLINE] ={0};
          
     do
     {
          sprintf(cData, "wr sz %d\r\n", total -sent);
          LogMsg2(cData);
          
          int
          bytes =write(sockfd, sData +sent, total -sent);
          
          sprintf(cData, "get sz %d\r\n", bytes);
          LogMsg2(cData);
          
          if ( bytes < 0 )
          {
               LogMsg((char *)"ERROR writing message to socket");
          }
          if ( bytes ==0 )
          {
               break;
          }
          
          sent +=bytes;
          
     } while ( sent <total );
     
     // receive the response
     memset(response, 0, sizeof(response));
     
     total =sizeof(response) -1;
     
     int
     send_Ok  =0,
     received =0;
     
     LogMsg2((char *)"Get Response....\r\n");
     
     do
     {
          int
          bytes =read(sockfd, response +received, total -received);
          
          if ( bytes <0 )
          {
               LogMsg((char *)"ERROR reading response from socket");
          }
          if ( bytes ==0 )
               break;
          
          received +=bytes;
          
          LogMsg2((char *)response);
          
          if ( strstr(response, "send ok:") !='\0' )
          {
               send_Ok =1;
               LogMsg((char *)"Succes !\r\n");
               break;
          }
          else
          {
               LogMsg((char *)response);
          }
          
     } while ( received <total );
     
     LogMsg((char *)"Get Response....Done\r\n");
     
     if ( received ==total )
     {
          LogMsg((char *)"ERROR storing complete response from socket");
     }
     
     // close the socket
     close(sockfd);
     
     // process response
     sprintf(cData, "Response:\n%s\n", response);

     LogMsg2((char *)sData);

     return send_Ok;
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
void *thread_Fcn ( void *lParam)
{
/*
     pthread_detach(pthread_self());

     THREAD_PARAM_UI
     *param =(THREAD_PARAM_UI*)lParam;
     
     if ( param !='\0')
     {
          CTimer_Linux
          me;

          me.attach(param);
          me.start();

          #if defined(__GPP)
          try
          {
               INT
               succ =param->pThis->onPost(param->duid, param->sData, param->iLen);

               if ( succ ==0 )
               { 
                    param->tryN++;
               }
               else
               {
                    param->iSucc++;

                    param->pThis->saveData();
               }
               param->action =0;
          }
          catch ( exception ex )
          {
               param->action =0;
          }
          #else

          param->pThis->onPost(param->duid, param->sData, param->iLen);

          #endif
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
void CHTTP_Imp_Linux::onPostThread(LPCSTR duid, LPCBYTE sData, INT iLen, BOOL always)
{
/*
     if ( canPost(always) ==FALSE )
     {
          return;
     }

     param.duid  =duid;
     param.sData =sData;
     param.iLen  =iLen;
     param.pThis =this;

     pthread_t
     aThread =NULL;
     
     int
     rc1 =pthread_create(&aThread, NULL, thread_Fcn, (void *)&param);
     
     if ( rc1 !=0 )
     {
          #if defined(__MUST_CALL)

          LogMsg("ERROR thread create!");
          
          onPost(duid, sData, iLen);

          #endif

          param.action =0;

          return;
     }

     param.aThread =aThread;
     
     #if defined(__MUST_CALL)

     rc1 =pthread_join(aThread, NULL);

     param.action =0;

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
#endif
