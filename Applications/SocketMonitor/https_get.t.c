
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

#include <assert.h>
#include "sdk_memory.h"
#include <sdk_str.h>

////////////////////////////////////////////////////////////////////////////////
////



#define TIMEOUT 5.0

void parse_url(char *url, char **hostname, char **port, char** path) {
    printf("URL: %s\n", url);
    
    char *p;
    p = strstr(url, "://");
    
    char *protocol = 0;
    if (p) {
        protocol = url;
        *p = 0;
        p += 3;
    } else {
        p = url;
    }
    
    if (protocol) {
        if (strcmp(protocol, "https")) {
            fprintf(stderr,
                    "Unknown protocol '%s'. Only 'https' is supported.\n",
                    protocol);
            exit(1);
        }
    }
    
    *hostname = p;
    while (*p && *p != ':' && *p != '/' && *p != '#') ++p;
    
    *port = "443";
    if (*p == ':') {
        *p++ = 0;
        *port = p;
    }
    while (*p && *p != '/' && *p != '#') ++p;
    
    *path = p;
    if (*p == '/') {
        *path = p + 1;
    }
    *p = 0;
    
    while (*p && *p != '#') ++p;
    if (*p == '#') *p = 0;
    
    printf("hostname: %s\n", *hostname);
    printf("port: %s\n", *port);
    printf("path: %s\n", *path);
}


void send_request(SSL *s, char *hostname, char *port, char *path) {
    char buffer[2048];
    
    sprintf(buffer, "GET /%s HTTP/1.1\r\n", path);
    sprintf(buffer + strlen(buffer), "Host: %s:%s\r\n", hostname, port);
    sprintf(buffer + strlen(buffer), "Connection: close\r\n");
    sprintf(buffer + strlen(buffer), "User-Agent: honpwc https_get 1.0\r\n");
    sprintf(buffer + strlen(buffer), "\r\n");
    
    SSL_write(s, buffer, strlen(buffer));
    printf("Sent Headers:\n%s", buffer);
}


SOCKET connect_to_host(char *hostname, char *port) {
    printf("Configuring remote address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo *peer_address;
    if (getaddrinfo(hostname, port, &hints, &peer_address)) {
        fprintf(stderr, "getaddrinfo() failed. (%d)\n", SDK_SOCKET_GET_SOCKET_ERRNO());
        exit(1);
    }
    
    printf("Remote address is: ");
    char address_buffer[100];
    char service_buffer[100];
    getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen,
                address_buffer, sizeof(address_buffer),
                service_buffer, sizeof(service_buffer),
                NI_NUMERICHOST);
    printf("%s %s\n", address_buffer, service_buffer);
    
    printf("Creating socket...\n");
    SOCKET server;
    server = socket(peer_address->ai_family,
                    peer_address->ai_socktype, peer_address->ai_protocol);
    if (!SDK_SOCKET_IS_VALID_SOCKET(server)) {
        fprintf(stderr, "socket() failed. (%d)\n", SDK_SOCKET_GET_SOCKET_ERRNO());
        exit(1);
    }
    
    printf("Connecting...\n");
    if (connect(server,
                peer_address->ai_addr, peer_address->ai_addrlen)) {
        fprintf(stderr, "connect() failed. (%d)\n", SDK_SOCKET_GET_SOCKET_ERRNO());
        exit(1);
    }
    freeaddrinfo(peer_address);
    
    printf("Connected.\n\n");
    
    return server;
}



int main(int argc, char *argv[]) {
    #if defined(__WIN32)
    system("chcp 65001");
    #endif
#if defined(_WIN32)
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        fprintf(stderr, "Failed to initialize.\n");
        return 1;
    }
#endif
    
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    
    SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx) {
        fprintf(stderr, "SSL_CTX_new() failed.\n");
        return 1;
    }
    
    if (argc < 2) {
        fprintf(stderr, "usage: web_get url\n");
        return 1;
    }
    char *url = argv[1];
    
    char *hostname, *port, *path;
    parse_url(url, &hostname, &port, &path);
    
    SOCKET server = connect_to_host(hostname, port);
    
    
    SSL *ssl = SSL_new(ctx);
    if (!ssl) {
        fprintf(stderr, "SSL_new() failed.\n");
        return 1;
    }
    
    if (!SSL_set_tlsext_host_name(ssl, hostname)) {
        fprintf(stderr, "SSL_set_tlsext_host_name() failed.\n");
        ERR_print_errors_fp(stderr);
        return 1;
    }
    
    SSL_set_fd(ssl, server);
    if (SSL_connect(ssl) == -1) {
        fprintf(stderr, "SSL_connect() failed.\n");
        ERR_print_errors_fp(stderr);
        return 1;
    }
    
    printf ("SSL/TLS using %s\n", SSL_get_cipher(ssl));
    
    
    X509 *cert = SSL_get_peer_certificate(ssl);
    if (!cert) {
        fprintf(stderr, "SSL_get_peer_certificate() failed.\n");
        return 1;
    }
    
    char *tmp;
    if ((tmp = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0))) {
        printf("subject: %s\n", tmp);
        OPENSSL_free(tmp);
    }
    
    if ((tmp = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0))) {
        printf("issuer: %s\n", tmp);
        OPENSSL_free(tmp);
    }
    
    X509_free(cert);
    
    send_request(ssl, hostname, port, path);
    
    const clock_t start_time = clock();

#define RESPONSE_SIZE (1024*1024*2)
    char* response = SDK_ALLOC(RESPONSE_SIZE+1);
    assert(response);
    char *p = response, *q;
    char *end = response + RESPONSE_SIZE;
    char *body = 0;
    
    enum {length, chunked, connection};
    int encoding = 0;
    int remaining = 0;
    
    while(1) {
        
        if ((clock() - start_time) / CLOCKS_PER_SEC > TIMEOUT) {
            fprintf(stderr, "timeout after %.2f seconds\n", TIMEOUT);
            return 1;
        }
        
        if (p == end) {
            fprintf(stderr, "out of buffer space\n");
            return 1;
        }
        
        fd_set reads;
        FD_ZERO(&reads);
        FD_SET(server, &reads);
        
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 200;
        
        if (select(server+1, &reads, 0, 0, &timeout) < 0) {
            fprintf(stderr, "select() failed. (%d)\n", SDK_SOCKET_GET_SOCKET_ERRNO());
            return 1;
        }
        
        if (FD_ISSET(server, &reads)) {
            int bytes_received = SSL_read(ssl, p, end - p);
            if (bytes_received < 1) {
                if (encoding == connection && body) {
                    printf("%.*s", (int)(end - body), body);
                }
                
                printf("\nConnection closed by peer.\n");
                break;
            }
            
//            printf("Received (%d bytes): '%.*s'",
//                    bytes_received, bytes_received, p);
            
            p += bytes_received;
            *p = 0;
            
            if (!body && (body = strstr(response, "\r\n\r\n"))) {
                *body = 0;
                body += 4;
                
                printf("Received Headers:\n%s\n", response);
                
                q = strstr(response, "\nContent-Length: ");
                if (q) {
                    encoding = length;
                    q = strchr(q, ' ');
                    q += 1;
                    remaining = strtol(q, 0, 10);
                    
                } else {
                    q = strstr(response, "\nTransfer-Encoding: chunked");
                    if (q) {
                        encoding = chunked;
                        remaining = 0;
                    } else {
                        encoding = connection;
                    }
                }
                printf("\nReceived Body:\n");
            }
            
            if (body) {
                if (encoding == length) {
                    if (p - body >= remaining) {
                        printf("%.*s", remaining, body);
                        break;
                    }
                } else if (encoding == chunked) {
                    do {
                        if (remaining == 0) {
                            if ((q = strstr(body, "\r\n"))) {
                                remaining = strtol(body, 0, 16);
                                if (!remaining) goto finish;
                                body = q + 2;
                            } else {
                                break;
                            }
                        }
                        if (remaining && p - body >= remaining) {
                            printf("%.*s", remaining, body);
                            body += remaining + 2;
                            remaining = 0;
                        }
                    } while (!remaining);
                }
            } //if (body)
        } //if FDSET
    } //end while(1)
    finish:
    
    int find_price = sdk_str_find(response, 1, 0, "<div id=\"price\"");
    if(find_price){
        find_price = sdk_str_find(response, find_price, 0, ">");
        if(find_price){
            printf("PRICE: %ld\n", strtoul(response+find_price, 0, 10));
        }
    }
    
    SDK_FREE(response);
    
    printf("\nClosing socket...\n");
    SSL_shutdown(ssl);
    sdk_socket_close(server);
    SSL_free(ssl);
    SSL_CTX_free(ctx);

#if defined(_WIN32)
    WSACleanup();
#endif
    
    printf("Finished.\n");
    return 0;
}

