cmake_minimum_required(VERSION 2.8.9)
project(concordpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set(CMAKE_BUILD_TYPE Release)
find_package(Boost 1.58 REQUIRED COMPONENTS system)
find_package(OpenSSL REQUIRED openssl)

# Disable tests for JSON
set(BuildTests OFF CACHE INTERNAL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/json)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/websocketpp)


file(GLOB_RECURSE SOURCES "src/*.cpp")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/libs/websocketpp)

add_library(concordpp ${SOURCES})
target_include_directories(concordpp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/libs/websocketpp)
target_link_libraries(concordpp ${PROJECT_LINK_LIBS} ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} pthread boost_thread restclient-cpp curl nlohmann_json)
