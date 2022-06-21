add_executable(main main.cpp PropertyTest.cpp ReadonlyPropertyTest.cpp)

include(GoogleTest)

find_package(GTest REQUIRED)

target_link_libraries(main PUBLIC GTest::gtest Property)
target_compile_features(main PUBLIC cxx_std_20)