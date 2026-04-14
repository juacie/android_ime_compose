/* 
 * File:   BufferHandler.h
 * Author: Lawrence
 *
 * Created on June 28, 2016, 2:05 AM
 */

#ifndef BUFFERHANDLER_H
#define BUFFERHANDLER_H

#include <openssl/bio.h>
#include <string>

typedef enum BUFFER_FORMAT_TYPE 
{
    BUFFER_FORMAT_UNKNOWN = 0,
    BUFFER_FORMAT_IPV4 = 1,
    BUFFER_FORMAT_IPV6 = 2,
} BUFFER_FORMAT_TYPE;

//extern int DEBUG;

class BufferHandler {
public:
    BufferHandler();
    ~BufferHandler();
   
    int decodeData(unsigned char *bkey, unsigned char *source, int source_len, unsigned char *dest, int dest_len);

    int encodeData(unsigned char *bkey, int bkey_len, unsigned char *source, int source_len, unsigned char **dest, int &dest_len);
    int encodeData2(unsigned char **bkey, int &bkey_len, unsigned char *source, int source_len, unsigned char **dest, int &dest_len);

private:
    RSA *publicRsa;
    RSA *privateRsa;
    BIO *publicMem, *privateMem;
  
    bool initRSA(bool isPublic);
    void deinit(int isPublic);

    int private_decrypt(unsigned char * enc_data,int data_len, unsigned char *decrypted);
    int public_encrypt(unsigned char * org_data, int data_len, unsigned char *encrypted);
    
    void printLastError();
};


#endif /* BUFFERHANDLER_H */

