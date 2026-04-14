/*
* File:   BufferHandler.cpp
* Author: Lawrence
*
* Created on June 28, 2016, 2:05 AM
*/

#include "stdafx.h"
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/blowfish.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include "BufferHandler.h"
#include "Data_Decode.h"

#ifdef __linux__
 #include <endian.h>
#elif __APPLE__
 #include <libkern/OSByteOrder.h>
 #define be32toh(x) OSSwapBigToHostInt32(x)
 #define le32toh(x) OSSwapLittleToHostInt32(x)
#endif

#define __DEBUG 1

#define WORD_TRACE_SDK_VERSION  "1.0.0.0"
#define COMPRESS_FACTOR 12

#define move_short(buff) (buff += sizeof(short))
#define move_int(buff) (buff += sizeof(int))

#define OFFSET_TS 1
#define OFFSET_IP 8
#define OFFSET_V4 15
#define OFFSET_V6 39
#define OFFSET_LEN 4

const char privateKey[] =
"-----BEGIN PRIVATE KEY-----\n" \
"MIICdgIBADANBgkqhkiG9w0BAQEFAASCAmAwggJcAgEAAoGBAOScNXn9osHPzZpO\n" \
"43ahMQFAwZkn1pnbOUdGYDSQ9qrNSIFHf9I/+OPXzQ06t5u8bw6AVFsV1zPZEymj\n" \
"Xhq4ipM3kGw/0pDdOm6msiVh34RKPC4MGByuL26Ur9vNLRi062604/tOE5vFYXlw\n" \
"cA2Vkt1WVLW+nxwbKurlqhE7cT9FAgMBAAECgYBCTEL9896Fknwv6fzxZGHP2TXL\n" \
"8s+7ouYDk5dG51uBwTLruw6IrODDQsxGr+ablZQngnP0TmswsB1sj+I0oZwd9RzO\n" \
"a2CyO0l78MOJ329FEdu0e2tGwxrpYQrp3DYjmpEiZvwlR85ShA1MyFQ+9HYIRNoR\n" \
"kUULp0rJjyopYbK6QQJBAP4cLemdApPM90U3t91fnM/tAzT+B7P8Mwj9oZ9my5UC\n" \
"WvyPQtflpM305zK8u4NDpDRMn3Q4wOY71NQitBVl5V0CQQDmT3pvVACCAMdHMWNS\n" \
"UvahEFdhyVEZiM+TSf0X35KJXbx2pYiXiQOD1PmcRNqddlsCluJjavNMjanvwE4l\n" \
"QvsJAkBZkB8SlveAlp6Us/XHPdrsYlJeDCc05fAq/BJKN5YLuB/PosLhi5xRxz7x\n" \
"4BmLQ30Nve/5q8M5ot75wh2M6TQ1AkEApi/Pjym0PBrHsS2IF81ZvZsHEr6r0JsR\n" \
"/Zw0Dtf66zhCV/LWf1IUwToa65IblefjgBjUPxzbdu/6IdBIE0cj0QJAQFg7pP0N\n" \
"QRy56otf9osibp9oGU3Vg/RKLut1CMbT0Jz0lOcis3FDReNtnT2E2H+zoP4DF9yd\n" \
"w+ynBaXQ4aNESA==\n" \
"-----END PRIVATE KEY-----\n";

const char publicKey[] =
"-----BEGIN PUBLIC KEY-----\n"\
"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDknDV5/aLBz82aTuN2oTEBQMGZ\n" \
"J9aZ2zlHRmA0kPaqzUiBR3/SP/jj180NOrebvG8OgFRbFdcz2RMpo14auIqTN5Bs\n" \
"P9KQ3TpuprIlYd+ESjwuDBgcri9ulK/bzS0YtOtutOP7ThObxWF5cHANlZLdVlS1\n" \
"vp8cGyrq5aoRO3E/RQIDAQAB\n" \
"-----END PUBLIC KEY-----\n";

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
BufferHandler::BufferHandler()
{
/*
     privateRsa = NULL;
     publicRsa = NULL;
     
     initRSA(false);
     initRSA(true);
*/
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
BufferHandler::~BufferHandler()
{
//     deinit(false);
//     deinit(true);
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
bool BufferHandler::initRSA(bool isPublic)
{
/*
     bool inited = false;
     
     if ( isPublic )
     {
          publicMem = BIO_new_mem_buf(publicKey, -1);
          
          if ( publicMem == NULL )
          {
               printf("Failed to create key publicMem");
               return false;
          }
     }
     else
     {
          privateMem = BIO_new_mem_buf(privateKey, -1);
          
          if ( privateMem == NULL )
          {
               printf("Failed to create key privateMem");
               return false;
          }
     }
     
     inited = true;
     
     if ( isPublic )
     {
          publicRsa = PEM_read_bio_RSA_PUBKEY(publicMem, NULL, NULL, NULL);
          
          if ( publicRsa == NULL )
          {
               printf("Failed to create RSA");
               inited = false;
          }
     }
     else
     {
          privateRsa = PEM_read_bio_RSAPrivateKey(privateMem, NULL, NULL, NULL);
          
          if ( privateRsa == NULL )
          {
               printf("Failed to create RSA");
               inited = false;
          }
     }
     return inited;
*/
return false;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void BufferHandler::deinit(int isPublic)
{
/*
     if ( isPublic )
     {
          BIO_free(publicMem);
     }
     else
     {
          BIO_free(privateMem);
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
int  BufferHandler::private_decrypt(unsigned char * enc_data, int data_len, unsigned char *decrypted)
{
/*
     int ret = -1;
     
     if ( privateRsa != NULL )
     {
          ret = RSA_private_decrypt(data_len, enc_data, decrypted, privateRsa, RSA_PKCS1_PADDING);
          
          if ( ret == -1 )
          {
               printLastError();
          }
     }
     return ret;
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
int  BufferHandler::public_encrypt(unsigned char * org_data, int data_len, unsigned char *encrypted)
{
/*
     int ret = -1;
     
     if ( publicRsa != NULL )
     {
          ret = RSA_public_encrypt(data_len, org_data, encrypted, publicRsa, RSA_PKCS1_PADDING);
          
          if ( ret == -1 )
          {
               printLastError();
          }
     }
     return ret;

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
void BufferHandler::printLastError()
{
/*
     char * err = (char *) new char[130];;
     
     ERR_load_crypto_strings();
     
     ERR_error_string(ERR_get_error(), err);
     
     printf("ERROR: %s\n", err);
     
     delete err;
*/
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
unsigned char *symmetric_decrypt(unsigned char * data, int input_len, const unsigned char *key, int *output_len)
{
/*
     unsigned
     char iv[8];
     
     EVP_CIPHER_CTX
     *ctx = EVP_CIPHER_CTX_new();
     
     EVP_CIPHER_CTX_init(ctx);
     
     EVP_DecryptInit(ctx, EVP_bf_ecb(), key, iv);
     
     int
     max_buffer_size = input_len + EVP_CIPHER_CTX_block_size(ctx);
     
     unsigned char *
     tmp_buffer = new unsigned char[max_buffer_size];
     
     memset(tmp_buffer, 0, max_buffer_size);
     
     int final_len = 0;
     
     if ( EVP_DecryptUpdate(ctx, tmp_buffer, output_len, data, input_len ) != 1 )
     {
          return '\0';
     }
     if ( EVP_DecryptFinal(ctx, tmp_buffer + *output_len, &final_len ) != 1 )
     {
          return '\0';
     }
     *output_len += final_len;
     
     EVP_CIPHER_CTX_cleanup(ctx);
     
     EVP_CIPHER_CTX_free(ctx);
     
     return tmp_buffer;
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
unsigned char *symmetric_encrypt(unsigned char * data, int input_len, const unsigned char *key, int *output_len)
{
/*
     unsigned
     char iv[8];
     
     EVP_CIPHER_CTX
     *ctx = EVP_CIPHER_CTX_new();
     
     EVP_CIPHER_CTX_init(ctx);
     
     EVP_EncryptInit(ctx, EVP_bf_ecb(), key, iv);
     
     int
     max_buffer_size = input_len + EVP_CIPHER_CTX_block_size(ctx);
     
     unsigned char *
     tmp_buffer = new unsigned char[max_buffer_size];
     
     memset(tmp_buffer, 0, max_buffer_size);
     
     int final_len = 0;
     
     if ( EVP_EncryptUpdate(ctx, tmp_buffer, output_len, data, input_len ) != 1 )
     {
          return '\0';
     }
     if ( EVP_EncryptFinal(ctx, tmp_buffer + *output_len, &final_len ) != 1 )
     {
          return '\0';
     }
     *output_len += final_len;
     
     EVP_CIPHER_CTX_cleanup(ctx);
     
     EVP_CIPHER_CTX_free(ctx);
     
     return tmp_buffer;
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
int  BufferHandler::decodeData(unsigned char *bkey, unsigned char *source, int source_len, unsigned char *dest, int dest_len)
{
/*
     int
     size =128;
     
     int
     written_len = 0;
     
     unsigned char *
     key_buffer = new unsigned char[size + 1];
     
     memset(key_buffer, 0, size + 1);
     memcpy(key_buffer, bkey, size);
     
     // decrypt key
     int
     rsa_size = RSA_size(privateRsa);
     
     unsigned char *
     decrypted_blowfish_key = new unsigned char[rsa_size];
     
     memset(decrypted_blowfish_key, 0, rsa_size);

     int
     result = private_decrypt(key_buffer, size, decrypted_blowfish_key);
     
     if ( decrypted_blowfish_key[0] =='\0' )
     {
          return -1;
     }

     decrypted_blowfish_key[result] ='\0';
     
     CDepack_Data
     data(source, source_len);
     
     while ( data.getLen() )
     {
             unsigned char
             *tmp_buffer = symmetric_decrypt((unsigned char *)data.getMem(), data.getLen(), decrypted_blowfish_key, &written_len);
             
             if ( tmp_buffer =='\0' )
             {
                  delete[] key_buffer;
                  delete[] tmp_buffer;
                  delete[] decrypted_blowfish_key;
                  
                  return '\0';
             }

             tmp_buffer[written_len] ='\0';

             delete[] tmp_buffer;
             
             data.parseEx(source, data.getLessLen());
     }
     
     delete[] key_buffer;
     delete[] decrypted_blowfish_key;
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
long genrndnum(long lower_limit, long upper_limit)
{
     srand(time('\0'));
     
     long rdnum =0;
     
     rdnum = lower_limit + (rand() % (int)(upper_limit - lower_limit + 1));
     
     return rdnum;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  BufferHandler::encodeData(
     unsigned char *bkey, int rsa_len, unsigned char *source, int source_len, unsigned char **dest, int &dest_len)
{
/*
     int
     written_len = 0;
     
     *dest =symmetric_encrypt((unsigned char *)source, source_len, bkey, &written_len);
     
     if ( *dest =='\0' )
     {
          return NULL;
     }
     
     dest_len =written_len;
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
int  BufferHandler::encodeData2(
     unsigned char **rsa_bkey, int &rsa_len, unsigned char *source, int source_len, unsigned char **dest, int &dest_len)
{
/*
     rsa_len = RSA_size(publicRsa);
     
     //------------------------------------------------------------------------
     unsigned char *
     bkey = new unsigned char[rsa_len];
     
     //sprintf((char *)bkey, "%d%", genrndnum(100000000, 999999999));
     //sprintf((char *)bkey, "%d%d", genrndnum(2148, 8192), genrndnum(1124, 4196));
     sprintf((char *)bkey, "123123Afd");

     int
     kLen = strlen((char*)bkey);

     bkey[kLen] ='\0';

     //------------------------------------------------------------------------
     *rsa_bkey = new unsigned char[rsa_len];
     
     memset(*rsa_bkey, 0, rsa_len);
     
     int
     result = public_encrypt(bkey, kLen, *rsa_bkey);
     
     //*rsa_bkey[result] ='\0';
     
     //------------------------------------------------------------------------
     *dest =symmetric_encrypt((unsigned char *)source, source_len, bkey, &dest_len);
     
     if ( *dest == NULL )
     {
          delete[] bkey;
          
          return -1;
     }
     
     delete[] bkey;
*/
     return 0;
}
