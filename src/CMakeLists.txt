add_library(project_common INTERFACE)
target_include_directories(project_common INTERFACE 
    ${PROJECT_SOURCE_DIR}/include
)
set_project_warnings(project_common)

add_library(config)
target_sources(config PRIVATE
    ${PROJECT_SOURCE_DIR}/src/config/Config.hpp
    ${PROJECT_SOURCE_DIR}/src/config/Config.cpp
)
target_link_libraries(config PUBLIC project_common tomlplusplus::tomlplusplus)

add_library(binance_order_book_lib INTERFACE)
target_sources(binance_order_book_lib INTERFACE
    ${PROJECT_SOURCE_DIR}/src/BinanceOrderBook.hpp
)

add_executable(binance_engine
    BinanceEngine.cpp
)
target_link_libraries(binance_engine PRIVATE
    project_common
    config
    binance_order_book_lib
    ixwebsocket
    simdjson::simdjson
    # OpenSSL::SSL 
    # OpenSSL::Crypto
    # ZLIB::ZLIB
)
