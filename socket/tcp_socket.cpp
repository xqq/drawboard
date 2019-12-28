//
// @author magicxqq <xqq@xqq.im>
//

#include "tcp_socket.hpp"

#ifndef _WIN32
    #include "tcp_socket_posix.hpp"
#else
    #include "tcp_socket_winsock.hpp"
#endif


TcpSocket* TcpSocket::Create() {
    TcpSocket* socket = nullptr;

#ifndef _WIN32
    socket = new TcpSocketPosix();
#else
    socket = new TcpSocketWinsock();
#endif

    return socket;
}
