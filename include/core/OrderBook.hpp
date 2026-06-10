#include "utils/SkipList.hpp"
#include <array>
#include <cstdint>
#include <memory>
#include <random>
#include <vector>

constexpr std::size_t MAX_LEVEL = 16;

enum class Side {
    BUY,
    SELL,
};

template <typename PriceLevel, typename Derived>
class OrderBook {

    // Bids
    SkipList<PriceLevel, PriceLevel::DecreasingOrderSentinel(), SkipListOrder::DECREASING, MAX_LEVEL> bids;

    // Asks
    SkipList<PriceLevel, PriceLevel::IncreasingOrderSentinel(), SkipListOrder::INCREASING, MAX_LEVEL> asks;

public:
    void updateAsks(const PriceLevel& pl) {
        if (pl.volume == 0.0) {
            asks.remove(pl);
        } else {
            asks.update(pl);
        }
    }

    void updateBids(const PriceLevel& pl) {
        if (pl.volume == 0.0) {
            bids.remove(pl);
        } else {
            bids.update(pl);
        }
    }

    void initializeBook() {
        static_cast<Derived*>(this)->initializeBook();
    }

    void onConnect() {
        // Default implementation (can be overridden)
        initializeBook();
    }

    void onEvent() {
        static_cast<Derived*>(this)->onEvent();
    }
};
