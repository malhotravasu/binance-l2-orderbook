#include <gtest/gtest.h>
#include "utils/SkipList.hpp"

TEST(SkipListTestEmpty, InitialStateEmpty) {
    SkipList<int, -1> skiplist;
    EXPECT_TRUE(skiplist.is_empty());
}

TEST(SkipListTestInsert, InitialStateEmpty) {
    SkipList<int, -1> skiplist;
    skiplist.update(10);
    skiplist.update(20);
    skiplist.update(15);
    EXPECT_FALSE(skiplist.is_empty());
}

TEST(SkipListTestUpdate, InitialStateEmpty) {
    SkipList<int, -1> skiplist;
    skiplist.update(10);
    skiplist.update(20);
    skiplist.update(20);
    EXPECT_FALSE(skiplist.is_empty());
}

TEST(SkipListTestRemove, InitialStateEmpty) {
    SkipList<int, -1> skiplist;
    skiplist.update(10);
    skiplist.update(20);
    skiplist.remove(10);
    skiplist.remove(20);
    EXPECT_TRUE(skiplist.is_empty());
}

TEST(SkipListTestRemoveNonExistent, InitialStateEmpty) {
    SkipList<int, -1> skiplist;
    skiplist.update(10);
    skiplist.remove(20);
    EXPECT_FALSE(skiplist.is_empty());
}
