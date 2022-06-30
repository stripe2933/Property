#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <Property.hpp>

using namespace std::string_literals;

TEST(PropertyTest, GetSet){
    Property<int> prop { 3 };
    EXPECT_EQ(prop.get(), 3);

    prop = 5;
    EXPECT_EQ(prop.get(), 5);

    prop.set(7);
    EXPECT_EQ(prop.get(), 7);

    {
        decltype(prop)::Transaction t { prop };
        t.data = 1;
        t.data = 2;
        t.data = 3;
        t.data = 4;
    }
    EXPECT_EQ(prop.get(), 4);
}

TEST(PropertyTest, PreSetter){
    int presetter_executed = 0;
    int prevalue;

    // presetter not executed at construction
    Property<int> prop { 1, [&](const int &old_value) { presetter_executed++; prevalue = old_value; }};

    prop.set(2);
    ASSERT_EQ(presetter_executed, 1);
    ASSERT_EQ(prevalue, 1);

    prop = 3;
    ASSERT_EQ(presetter_executed, 2);
    ASSERT_EQ(prevalue, 2);

    {
        decltype(prop)::Transaction t { prop };
        t.data = 1;
        t.data = 2;
        t.data = 3;
        t.data = 4;
    }
    ASSERT_EQ(presetter_executed, 3);
    ASSERT_EQ(prevalue, 3);
}

TEST(PropertyTest, PostSetter){
    int postsetter_executed = 0;
    int postvalue;

    // postsetter executed at construction
    Property<int> prop { 1, {}, [&](const int &new_value) { postsetter_executed++; postvalue = new_value; }};
    ASSERT_EQ(postsetter_executed, 1);
    ASSERT_EQ(postvalue, 1);

    prop.set(2);
    ASSERT_EQ(postsetter_executed, 2);
    ASSERT_EQ(postvalue, 2);

    prop = 3;
    ASSERT_EQ(postsetter_executed, 3);
    ASSERT_EQ(postvalue, 3);

    {
        decltype(prop)::Transaction t { prop };
        t.data = 1;
        t.data = 2;
        t.data = 3;
        t.data = 4;
    }
    ASSERT_EQ(postsetter_executed, 4);
    ASSERT_EQ(postvalue, 4);

    std::stringstream ss;
    Property<std::string> prop2 { "Property", {}, [&](const std::string &new_value) { ss << new_value; } }; // Property

    prop2.set("Property2"); // PropertyProperty2

    prop2.set("Property3"s); // PropertyProperty2Property3

    prop2 = "Property4"; // PropertyProperty2Property3Property4

    std::string text { "Property5" };
    prop2.set(text); // PropertyProperty2Property3Property4Property5
    prop2.set(std::move(text)); // PropertyProperty2Property3Property4Property5Property5

    {
        decltype(prop2)::Transaction t { prop2 };
        t.data.pop_back();
        t.data.pop_back();
    }
    // PropertyProperty2Property3Property4Property5Property5Propert

    const std::string text2 { "Property6" };
    prop2.set(text2); // PropertyProperty2Property3Property4Property5Property5PropertProperty6

    EXPECT_EQ(ss.str(), "PropertyProperty2Property3Property4Property5Property5PropertProperty6"s);
}

TEST(PropertyTest, ConstructionTest){
    std::string text { "my text" };
    Property<std::string> prop { text };

    EXPECT_EQ(prop.get(), "my text"s);

    // TODO: check unintended copy construction

    struct Foo{
        int dc = 0; // default constructor construction count
        int cc = 0; // copy constructor construction count
        int mc = 0; // move constructor construction count

        char value;

        Foo(char value) : value { value } { dc++; }
        Foo(const Foo &source) : value { source.value } { cc++; }
        Foo(Foo &&source) : value { std::move(source.value) } { mc++; }
        Foo &operator=(const Foo &source) { value = source.value; cc++; return *this; }
        Foo &operator=(Foo &&source) { value = std::move(source.value); mc++; return *this; }
    };

    // Property<Foo> prop1 { 'a' };
    // EXPECT_EQ(prop1.get().dc + prop1.get().mc, 1);
    // EXPECT_EQ(prop1.get().cc, 0);

    // prop1 = std::move('b');
    // EXPECT_EQ(prop1.get().dc + prop1.get().mc, 1);
    // EXPECT_EQ(prop1.get().cc, 0);
}

TEST(PropertyTest, CollectionType){
    Property<std::vector<int>> prop { std::vector<int>{ 1, 2, 3 } };
    EXPECT_THAT(prop.get(), ::testing::ElementsAreArray({ 1, 2, 3 }));

    prop = std::vector<int>{ 2, 4, 6, 8, 10 };
    EXPECT_THAT(prop.get(), ::testing::ElementsAreArray({ 2, 4, 6, 8, 10 }));

    prop.set(std::vector<int>{ 2, 3, 5, 7, 11, 13 });
    EXPECT_THAT(prop.get(), ::testing::ElementsAreArray({ 2, 3, 5, 7, 11, 13 }));

    {
        decltype(prop)::Transaction t { prop };
        t.data.push_back(17);
        t.data.push_back(19);
        t.data.push_back(21);
        t.data.pop_back();
    }
    EXPECT_THAT(prop.get(), ::testing::ElementsAreArray({ 2, 3, 5, 7, 11, 13, 17, 19 }));
}