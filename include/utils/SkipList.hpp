#include <array>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

enum class SkipListOrder {
    INCREASING,
    DECREASING,
};

template <typename Data>
struct Node {
    Data data;

    std::vector<std::shared_ptr<Node<Data>>> forward;

    Node(const Data& d, std::size_t level) : data{d}, forward(level + 1, nullptr) {
    }

    void setData(const Data& d) {
        data = d;
    }
};

template <typename Data, Data Sentinel, SkipListOrder Order = SkipListOrder::INCREASING, std::size_t MAX_LEVEL = 16>
class SkipList {
private:
    std::size_t current_max_level;

    // Random number generation for Skip List height
    std::mt19937 gen;

    std::shared_ptr<Node<Data>> head;

    std::size_t random_level() {
        auto rnd = gen();

        // Count trailing zeros
        std::size_t lvl = static_cast<std::size_t>(std::countr_zero(rnd));
        // Clamp to MAX_LEVEL
        if (lvl > MAX_LEVEL) {
            lvl = MAX_LEVEL;
        }

        return lvl;
    }

public:
    SkipList() : current_max_level{0}, gen{std::random_device{}()}, head{std::make_shared<Node<Data>>(Sentinel, MAX_LEVEL)} {
    }

    bool is_before(Data a, Data b) const {
        // Use constexpr if to decide comparison at compile time
        if constexpr (Order == SkipListOrder::INCREASING) {
            return a < b;
        } else {
            return a > b;
        }
    }

    // Update (Handles Insertion AND Modification)
    // If data exists -> Update (By assignment)
    // If data missing -> Insert
    void update(const Data& data) {
        std::vector<std::shared_ptr<Node<Data>>> update_path(MAX_LEVEL + 1);
        auto current = head;

        // Traverse: Find the position just BEFORE our target price
        std::size_t i = current_max_level;
        while (true) {
            while (current->forward[i] && is_before(current->forward[i]->data, data)) {
                current = current->forward[i];
            }
            update_path[i] = current;
            if (i == 0)
                break;
            i--;
        }

        auto target = current->forward[0];

        // Case A: data exists -> Just update data (Fastest path)
        if (target && target->data == data) {
            target->setData(data);
            return;
        }

        // Case B: Insert new skip list level
        std::size_t lvl = random_level();
        if (lvl > current_max_level) {
            // Expand our update path to cover new higher levels
            for (i = current_max_level + 1; i <= lvl; i++) {
                update_path[i] = head;
            }
            current_max_level = lvl;
        }

        auto new_node = std::make_shared<Node<Data>>(data, lvl);

        // Link the new node into the list
        for (i = 0; i <= lvl; i++) {
            new_node->forward[i] = update_path[i]->forward[i];
            update_path[i]->forward[i] = new_node;
        }
    }

    // Remove
    // Finds the price and unlinks the node
    void remove(const Data& data) {
        std::vector<std::shared_ptr<Node<Data>>> update_path(MAX_LEVEL + 1);
        auto current = head;

        std::size_t i = current_max_level;
        while (true) {
            while (current->forward[i] && is_before(current->forward[i]->data, data)) {
                current = current->forward[i];
            }
            update_path[i] = current;
            if (i == 0)
                break;
            i--;
        }

        auto target = current->forward[0];

        if (target && target->data == data) {
            // Unlink at every level this node exists
            for (i = 0; i <= current_max_level; i++) {
                if (update_path[i]->forward[i] != target)
                    break;
                update_path[i]->forward[i] = target->forward[i];
            }

            // Optimization: Lower the max level if the top is now empty
            while (current_max_level > 0 && head->forward[current_max_level] == nullptr) {
                current_max_level--;
            }
        }
    }

    void print(const std::string& side_name) const {
        auto current = head->forward[0];
        std::cout << "--- " << side_name << " ---\n";
        if (!current) {
            std::cout << "(Empty)\n";
        }
        while (current) {
            std::cout << "Node Data: " << current->data << "\n";
            current = current->forward[0];
        }
    }

    bool is_empty() const {
        return head->forward[0] == nullptr;
    }
};
