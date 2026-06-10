#include "Config.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <toml++/toml.h>

namespace config {

    namespace binance {
        Config Config::load_from_file(const std::string& config_path) {
            std::string error;
            return load_from_file(config_path, error);
        }

        Config Config::load_from_file(const std::string& config_path, std::string& error) {
            try {
                auto config_file = toml::parse_file(config_path);

                Config config;

                // Parse logging configuration
                auto logging_table = config_file["logging"];
                config.logging.level = logging_table["level"].value_or("info");
                config.logging.format = logging_table["format"].value_or("%Y-%m-%d %H:%M:%S");

                // Parse connectivity configuration
                auto connectivity_table = config_file["connectivity"];
                config.connectivity.ws_base_url = connectivity_table["ws_url"].value_or("wss://stream.binance.com:9443");
                config.connectivity.order_book_depth_stream = connectivity_table["order_book_depth_stream"].value_or("depth");
                config.connectivity.order_book_depth_update_speed = connectivity_table["order_book_depth_update_speed"].value_or("100ms");

                if (auto connections = connectivity_table["connections"].as_array()) {
                    for (auto&& node : *connections) {
                        if (auto* conn_table = node.as_table()) {
                            Connection conn;
                            conn.symbol = (*conn_table)["symbol"].value_or("");
                            config.connectivity.connections.push_back(conn);
                        }
                    }
                }

                return config;
            } catch (const toml::parse_error& e) {
                error = "Failed to parse TOML config: " + std::string(e.what());
                throw std::runtime_error(error);
            } catch (const std::exception& e) {
                error = "Error loading config: " + std::string(e.what());
                throw std::runtime_error(error);
            }
        }

        std::string ConnectivityConfig::construct_ws_url(const Connection& connection) const {
            return ws_base_url + "/ws/" + connection.symbol + "@" + order_book_depth_stream + "@" + order_book_depth_update_speed;
        }
    } // namespace binance

} // namespace config
