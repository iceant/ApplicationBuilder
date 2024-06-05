#include <sdk_socket.h>

sdk_err_t sdk_socket_init(void)
{
#if defined(_WIN32)
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2, 2), &wsaData)){
        return SDK_ERROR;
    }
#endif
}

void sdk_socket_destroy(void)
{
#if defined(_WIN32)
    WSACleanup();
#endif
}

