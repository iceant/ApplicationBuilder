#ifndef INCLUDED_SDK_SOCKET_H
#define INCLUDED_SDK_SOCKET_H

#if defined(_WIN32)
    #ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0600
    #endif

    #ifndef INCLUDED_WINSOCK2_H
    #define INCLUDED_WINSOCK2_H
    #include <winsock2.h>
    #endif /*INCLUDED_WINSOCK2_H*/

    #ifndef INCLUDED_WS2TCPIP_H
    #define INCLUDED_WS2TCPIP_H
    #include <ws2tcpip.h>
    #endif /*INCLUDED_WS2TCPIP_H*/

    #pragma comment(lib, "ws2_32.lib")
#else
    #ifndef INCLUDED_SYS_TYPES_H
    #define INCLUDED_SYS_TYPES_H
    #include <sys/types.h>
    #endif /*INCLUDED_SYS_TYPES_H*/

    #ifndef INCLUDED_SYS_SOCKET_H
    #define INCLUDED_SYS_SOCKET_H
    #include <sys/socket.h>
    #endif /*INCLUDED_SYS_SOCKET_H*/

    #ifndef INCLUDED_NETINET_IN_H
    #define INCLUDED_NETINET_IN_H
    #include <netinet/in.h>
    #endif /*INCLUDED_NETINET_IN_H*/

    #ifndef INCLUDED_ARPA_INET_H
    #define INCLUDED_ARPA_INET_H
    #include <arpa/inet.h>
    #endif /*INCLUDED_ARPA_INET_H*/

    #ifndef INCLUDED_NETDB_H
    #define INCLUDED_NETDB_H
    #include <netdb.h>
    #endif /*INCLUDED_NETDB_H*/

    #ifndef INCLUDED_UNISTD_H
    #define INCLUDED_UNISTD_H
    #include <unistd.h>
    #endif /*INCLUDED_UNISTD_H*/

    #ifndef INCLUDED_ERRNO_H
    #define INCLUDED_ERRNO_H
    #include <errno.h>
    #endif /*INCLUDED_ERRNO_H*/

#endif /* defined(_WIN32) */

#ifndef INCLUDED_STDIO_H
#define INCLUDED_STDIO_H
#include <stdio.h>
#endif /*INCLUDED_STDIO_H*/

#ifndef INCLUDED_STDLIB_H
#define INCLUDED_STDLIB_H
#include <stdlib.h>
#endif /*INCLUDED_STDLIB_H*/

#ifndef INCLUDED_STRING_H
#define INCLUDED_STRING_H
#include <string.h>
#endif /*INCLUDED_STRING_H*/


////////////////////////////////////////////////////////////////////////////////
////

#if defined(_WIN32)
    typedef SOCKET sdk_socket_t;
    #define SDK_SOCKET_IS_VALID_SOCKET(s)       ((s) != INVALID_SOCKET)
    #define SDK_SOCKET_CLOSE_SOCKET(s)          closesocket(s)
    #define SDK_SOCKET_GET_SOCKET_ERRNO()       (WSAGetLastError())
#else
    typedef int sdk_socket_t;
    #define SDK_SOCKET_IS_VALID_SOCKET(s)       ((s) >= 0)
    #define SDK_SOCKET_CLOSE_SOCKET(s)          close(s)
    #define SDK_SOCKET_GET_SOCKET_ERRNO()       (errno)
#endif /* defined(_WIN32) */

////////////////////////////////////////////////////////////////////////////////
////



#endif /*INCLUDED_SDK_SOCKET_H*/
