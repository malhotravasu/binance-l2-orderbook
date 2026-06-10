#pragma once

#include <optional>
#include <string>
#include <vector>

namespace config {

    struct LoggingConfig {
        std::string level;
        std::string format;
    };

    namespace binance {

        struct Connection {
            std::string symbol;
            // std::vector<std::string> additional_streams;
        };

        struct ConnectivityConfig {
            std::string ws_base_url;
            std::string order_book_depth_stream;
            std::string order_book_depth_update_speed;
            std::vector<struct Connection> connections;

            std::string construct_ws_url(const Connection& connection) const;
        };

        struct Config {
            LoggingConfig logging;
            ConnectivityConfig connectivity;

            static Config load_from_file(const std::string& config_path);
            static Config load_from_file(const std::string& config_path, std::string& error);
        };
    } // namespace binance

} // namespace config
