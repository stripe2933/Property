#include <gtest/gtest.h>

#include <ReadonlyProperty.hpp>

TEST(ReadonlyPropertyTest, Getter){
    ReadonlyProperty<int> prop { 3 };
    EXPECT_EQ(prop.get(), 3);

    ReadonlyProperty<int> prop2 = 5;
    EXPECT_EQ(prop2.get(), 5);
}