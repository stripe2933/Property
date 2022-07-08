#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <Property.hpp>

using namespace std::string_literals;

TEST(PropertyTest, GetSet){
    // Test with int

    Property<int> prop1 { 5 };
    EXPECT_EQ(prop1.get(), 5);

    prop1 = 6;
    EXPECT_EQ(prop1.get(), 6);

    prop1.set(7);
    EXPECT_EQ(prop1.get(), 7);

    {
        auto t = prop1.getTransaction();
        t.data = 8;
        t.data = 9;
        t.data = 10;
    }
    EXPECT_EQ(prop1.get(), 10);

    // Test with std::string

    Property<std::string> prop2 { "Property 2" };
    EXPECT_EQ(prop2.get(), "Property 2"s);

    prop2 = "Property 2 modified";
    EXPECT_EQ(prop2.get(), "Property 2 modified"s);

    std::string_view view { "Property 2 modified by std::string_view" };
    prop2.set(view);
    EXPECT_EQ(prop2.get(), "Property 2 modified by std::string_view");

    {
        auto t = prop2.getTransaction();
        t.data = "Property 2 modified by transaction";
        t.data.pop_back();
    }
    EXPECT_EQ(prop2.get(), "Property 2 modified by transactio");

    // Test with std::vector<int>

    Property<std::vector<int>> prop3 { 1, 2, 3, 4, 5 };
    EXPECT_THAT(prop3.get(), ::testing::ElementsAreArray({ 1, 2, 3, 4, 5 }));

    {
        auto t = prop3.getTransaction();
        t.data.push_back(6);
        t.data.push_back(7);
    }
    EXPECT_THAT(prop3.get(), ::testing::ElementsAreArray({ 1, 2, 3, 4, 5, 6, 7 }));
}

TEST(PropertyTest, CallbackTest){
    int alpha_sum = 0;

    Property<char> prop1 { [&](const char &new_value) -> void {
        alpha_sum += new_value;
    }, 'a' };

    EXPECT_EQ(alpha_sum, 97); // 'a'

    prop1 = 'b';
    EXPECT_EQ(alpha_sum, 195); // 'a' + 'b'

    prop1.set('c');
    EXPECT_EQ(alpha_sum, 294); // 'a' + 'b' + 'c' (allow overflow)

    {
        auto t = prop1.getTransaction();
        t.data = 'd';
        t.data = 'e';
        t.data = 'f';
    }
    EXPECT_EQ(alpha_sum, 396); // 'a' + 'b' + 'c' + 'f' (allow overflow)
}