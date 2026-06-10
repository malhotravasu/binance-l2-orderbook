#include "core/OrderBook.hpp"
#include <cstdint>
#include <iostream>

// Anonymous namespace - Only Binance stuff here
namespace {

    using Price = double;
    using Quantity = double;
    using Timestamp = std::uint64_t;

    enum class EventType {
        Trade,
        Depth,
        Ticker,
    };

    struct PriceLevel {
        Price price;
        Quantity volume;

        static consteval PriceLevel IncreasingOrderSentinel() {
            return PriceLevel(-1.0, 0.0);
        }

        static consteval PriceLevel DecreasingOrderSentinel() {
            return PriceLevel(std::numeric_limits<Price>::infinity(), 0.0);
        }

        constexpr PriceLevel(Price p, Quantity v) : price{p}, volume{v} {
        }

        constexpr PriceLevel() : PriceLevel(0, 0) {
        }

        bool operator==(const PriceLevel& other) const {
            return price == other.price;
        }

        bool operator<(const PriceLevel& other) const {
            return price < other.price;
        }

        bool operator>(const PriceLevel& other) const {
            return price > other.price;
        }

        // For printing
        friend std::ostream& operator<<(std::ostream& os, const PriceLevel& pl) {
            os << "Price: " << pl.price << ", Volume: " << pl.volume;
            return os;
        }
    };

} // namespace

class BinanceOrderBook : public OrderBook<PriceLevel, BinanceOrderBook> {
    BinanceOrderBook() = default;

    void onEvent() {
        // Custom event handling logic for BinanceOrderBook
    }

    void initializeBook() {
        // Initialization logic specific to BinanceOrderBook
    }
};
