//
// @author magicxqq <xqq@xqq.im>
//

#include <memory>
#include <argh.h>
#include "client/client_app.hpp"
#include "server/draw_server.hpp"

int main(int argc, char* argv[]) {
    argh::parser cmdl(argc, argv, argh::parser::PREFER_PARAM_FOR_UNREG_OPTION);

    enum Mode {
        kClient,
        kServer
    } mode = kClient;

    std::string host;
    uint16_t port;
    bool error = false;

    if ((cmdl({"-s", "--server"}) >> host)) {
        mode = kServer;
    } else if ((cmdl({"-c", "--client"}) >> host)) {
        mode = kClient;
    } else {
        error = true;
    }

    if (!(cmdl({"-p", "--port"}) >> port)) {
        error = true;
    }

    if (error) {
        printf("Usage: drawboard [OPTIONS]\n" \
               "    --client <host> --port <port>\n"
               "    --server <host> --port <port>\n");
        return -1;
    }

    std::unique_ptr<DrawServer> server;

    if (mode == kServer) {
        server = std::make_unique<DrawServer>();
        server->Start(host, port);
    }

    if (host == "0.0.0.0") {
        host = "127.0.0.1";
    }

    ClientApp client(host, port);
    int ret = client.Run();

    if (mode == kServer) {
        server->Stop();
    }

    return ret;
}
