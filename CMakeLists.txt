cmake_minimum_required(VERSION 2.8.9)
project (Discord-CPP)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set(CMAKE_BUILD_TYPE Release)
find_package(Boost 1.58 REQUIRED COMPONENTS system)
find_package(OpenSSL REQUIRED openssl)

include_directories( ./include ./libs/websocketpp ./libs/json/src ${Boost_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIRS})

link_directories(${Boost_LIBRARY_DIRS} ${OPENSSL_LIBRARY_DIRS} /usr/local/lib/)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(runfile application/main.cpp ${SOURCES})
target_link_libraries(runfile ${PROJECT_LINK_LIBS} ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} pthread boost_thread restclient-cpp curl)
