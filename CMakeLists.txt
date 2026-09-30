cmake_minimum_required(VERSION 3.18)

project(XAUUSD_AMD_Hybrid_Scalper VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(trading_core STATIC
    cpp/src/TradingEngine.cpp
)

target_include_directories(trading_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/include
)

if(MSVC)
    target_compile_options(trading_core PRIVATE /O2 /W4)
else()
    target_compile_options(trading_core PRIVATE -O3 -Wall -Wextra -Wpedantic)
endif()
