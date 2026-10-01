cmake_minimum_required(VERSION 3.18)

project(XAUUSD_AMD_Hybrid_Scalper VERSION 0.3.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(trading_core STATIC
    cpp/src/TradingEngine.cpp
    cpp/src/AmdDetector.cpp
    cpp/src/M5StructureEngine.cpp
    cpp/src/SetupEngine.cpp
)

target_include_directories(trading_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/cpp/include)

if(MSVC)
    target_compile_options(trading_core PRIVATE /O2 /W4)
else()
    target_compile_options(trading_core PRIVATE -O3 -Wall -Wextra -Wpedantic)
endif()

include(CTest)
if(BUILD_TESTING)
    add_executable(amd_detector_test tests/amd_detector_test.cpp)
    target_link_libraries(amd_detector_test PRIVATE trading_core)
    add_test(NAME amd_detector_test COMMAND amd_detector_test)

    add_executable(m5_confirmation_test tests/m5_confirmation_test.cpp)
    target_link_libraries(m5_confirmation_test PRIVATE trading_core)
    add_test(NAME m5_confirmation_test COMMAND m5_confirmation_test)
endif()
