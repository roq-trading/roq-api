/* Copyright (c) 2017,  Hans Erik Thrane */

#include <gtest/gtest.h>

TEST(array, test_1) {
    ASSERT_EQ(1, 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
