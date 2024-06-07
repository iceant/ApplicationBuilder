#ifndef INCLUDED_SM_HTTPCLIENT_H
#define INCLUDED_SM_HTTPCLIENT_H

////////////////////////////////////////////////////////////////////////////////
////

#ifndef INCLUDED_SDK_SOCKET_H
#include <sdk_socket.h>
#endif /*INCLUDED_SDK_SOCKET_H*/

#ifndef INCLUDED_OPENSSL_CRYPTO_H
#define INCLUDED_OPENSSL_CRYPTO_H
#include <openssl/crypto.h>
#endif /*INCLUDED_OPENSSL_CRYPTO_H*/

#ifndef INCLUDED_OPENSSL_X509_H
#define INCLUDED_OPENSSL_X509_H
#include <openssl/x509.h>
#endif /*INCLUDED_OPENSSL_X509_H*/

#ifndef INCLUDED_OPENSSL_PEM_H
#define INCLUDED_OPENSSL_PEM_H
#include <openssl/pem.h>
#endif /*INCLUDED_OPENSSL_PEM_H*/

#ifndef INCLUDED_OPENSSL_SSL_H
#define INCLUDED_OPENSSL_SSL_H
#include <openssl/ssl.h>
#endif /*INCLUDED_OPENSSL_SSL_H*/

#ifndef INCLUDED_OPENSSL_ERR_H
#define INCLUDED_OPENSSL_ERR_H
#include <openssl/err.h>
#endif /*INCLUDED_OPENSSL_ERR_H*/

////////////////////////////////////////////////////////////////////////////////
////
typedef struct sm_httpclient_s {
    SSL_CTX * ssl_ctx;
}sm_httpclient_t;


////////////////////////////////////////////////////////////////////////////////
////

int sm_httpclient_init(sm_httpclient_t* httpclient);



#endif /* INCLUDED_SM_HTTPCLIENT_H */
