//
// @author magicxqq <xqq@xqq.im>
//

#include "tcp_listener.hpp"

#ifndef _WIN32
    #include "tcp_listener_posix.hpp"
#else
    #include "tcp_listener_winsock.hpp"
#endif

TcpListener* TcpListener::Create(OnAcceptedCallback on_accept, OnErrorCallback on_error) {
    TcpListener* listener = nullptr;

#ifndef _WIN32
    listener = new TcpListenerPosix(on_accept, on_error);
#else
    listener = new TcpListenerWinsock(on_accept, on_error);
#endif

    return listener;
}
