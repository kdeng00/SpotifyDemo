# CMakeList.txt : CMake project for SpotifyDemo, include source and define
# project specific logic here.
#
# cmake_minimum_required (VERSION 3.8)

set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_STANDARD 14)


find_package (nlohmann_json CONFIG REQUIRED)
find_package (cpr CONFIG REQUIRED)

find_path (CPPCODEC_INCLUDE_DIRS "cppcodec/base32_crockford.hpp")


set (SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SpotifyDemo.cpp"
)

set (SD_INCLUDE_DIR

    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    )

set (HEADERS
    "${SD_INCLUDE_DIR}/Models.h"
)

include_directories ("${SD_INCLUDE_DIR}")

configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/client_information.json" "${CMAKE_BINARY_DIR}/SpotifyDemo/client_information.json" COPYONLY)

# Add source to this project's executable.
add_executable (SpotifyDemo ${SOURCES})

target_link_libraries (SpotifyDemo PRIVATE nlohmann_json nlohmann_json::nlohmann_json cpr::cpr)
target_include_directories (SpotifyDemo PRIVATE ${CPPCODEC_INCLUDE_DIRS})
