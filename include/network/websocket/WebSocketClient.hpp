#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <memory>

using WebSocketClient = ix::WebSocket;

std::unique_ptr<WebSocketClient> getWebSocketClient(const std::string& url, bool useTLS, int pingIntervalSeconds) {
    auto client = std::make_unique<WebSocketClient>();

    client->setUrl(url);

    if (useTLS) {
        ix::SocketTLSOptions tlsOptions;
        tlsOptions.caFile = "SYSTEM"; // Use system CA store
        client->setTLSOptions(tlsOptions);
    }

    if (pingIntervalSeconds > 0) {
        client->setPingInterval(pingIntervalSeconds);
    }

    return client;
}
