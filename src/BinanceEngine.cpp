#include "BinanceOrderBook.hpp"
#include "config/Config.hpp"
#include "network/websocket/WebSocketClient.hpp"
#include <iostream>
#include <simdjson.h>

void spawnBinanceConnections(const config::binance::Config& cfg, std::vector<std::unique_ptr<WebSocketClient>>& web_sockets) {
    for (const auto& connection : cfg.connectivity.connections) {
        auto ws_url = cfg.connectivity.construct_ws_url(connection);
        std::cout << "Constructed WebSocket URL for " << connection.symbol << ": " << ws_url << std::endl;

        // Configure the WebSocket
        auto web_socket = getWebSocketClient(ws_url, true, 300);

        // Define the Event Callback
        // This lambda runs on a background thread managed by the library.
        web_socket->setOnMessageCallback([](const ix::WebSocketMessagePtr& msg) {
            if (msg->type == ix::WebSocketMessageType::Message) {
                // Success: Data received
                std::cout << "[Data] " << msg->str << "\n";
                // Note: msg->wireSize tells you the compressed size (bandwidth usage)
            } else if (msg->type == ix::WebSocketMessageType::Open) {
                std::cout << "[Status] Connection Established!\n";
                std::cout << "[Info] Handshake Headers: \n";
                for (auto [key, val] : msg->openInfo.headers) {
                    std::cout << "  " << key << ": " << val << "\n";
                }
            } else if (msg->type == ix::WebSocketMessageType::Error) {
                // Error handling (Authentication failed, Network down, etc.)
                std::cerr << "[Error] " << msg->errorInfo.reason << "\n";
                std::cerr << "[Error] HTTP Status: " << msg->errorInfo.http_status << "\n";
            } else if (msg->type == ix::WebSocketMessageType::Close) {
                std::cout << "[Status] Connection Closed: " << msg->closeInfo.reason << "\n";
            }
        });

        // start() creates a background thread and returns immediately.
        // If you want blocking behavior, use webSocket.run() instead.
        std::cout << "Starting WebSocket for " << connection.symbol << "...\n";
        web_socket->start();

        // Store a reference to keep the connection alive
        web_sockets.push_back(std::move(web_socket));
    }
}

int main() {
    std::string config_path = "config/binance.toml";
    std::string error;

    config::binance::Config cfg;
    try {
        cfg = config::binance::Config::load_from_file(config_path, error);

        std::cout << "WebSocket Base URL: " << cfg.connectivity.ws_base_url << std::endl;
        std::cout << "Order Book Depth Update Speed: " << cfg.connectivity.order_book_depth_update_speed << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error loading configuration: " << error << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<std::unique_ptr<WebSocketClient>> web_sockets; // Needed to keep connections alive
    spawnBinanceConnections(cfg, web_sockets);

    // Keep Main Thread Alive
    // Since webSocket.start() is non-blocking, we need a loop here
    // to prevent the program from exiting immediately.
    // Add additional logic as needed.
    while (true) {
        // Your main application logic goes here
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return EXIT_SUCCESS;
}
