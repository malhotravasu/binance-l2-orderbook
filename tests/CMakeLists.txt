add_executable(
  all_tests
  test_orderbook.cpp
  test_binance_client.cpp
  test_skip_list.cpp
)
set_project_warnings(all_tests)

target_include_directories(all_tests PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(all_tests PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(
    all_tests
    GTest::gtest
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(all_tests DISCOVERY_MODE PRE_TEST)
