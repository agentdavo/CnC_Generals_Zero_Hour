#ifndef __WIN32_WINSOCK_STUB_H__
#define __WIN32_WINSOCK_STUB_H__

#ifdef _WIN32
#include <winsock.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

using SOCKET = int;
using u_long = unsigned long;

#ifndef INVALID_SOCKET
#define INVALID_SOCKET (-1)
#endif
#ifndef SOCKET_ERROR
#define SOCKET_ERROR (-1)
#endif

inline int closesocket(SOCKET s) { return close(s); }
inline int ioctlsocket(SOCKET s, long cmd, u_long* argp) { return ioctl(s, cmd, argp); }
#endif

#endif // __WIN32_WINSOCK_STUB_H__
