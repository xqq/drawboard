//
// @author magicxqq <xqq@xqq.im>
//

#ifndef DRAWBOARD_CLIENT_APP_HPP
#define DRAWBOARD_CLIENT_APP_HPP

#include <cstdint>
#include <string>
#include "client/draw_client.hpp"
#include "sdl_include.hpp"

class ClientApp {
public:
    ClientApp(const std::string& host, uint16_t port);
    int Run();
private:
    std::string host_;
    uint16_t port_;
    DrawClient client_;
};


#endif // DRAWBOARD_CLIENT_APP_HPP
