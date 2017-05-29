cmake_minimum_required(VERSION 2.8.9)
project (Discord-CPP)

#set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set(CMAKE_BUILD_TYPE Release)
find_package(Boost 1.62 REQUIRED COMPONENTS system)
#find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED openssl)

include_directories( ./include ./libs/websocketpp ./libs/json/src ${Boost_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIRS})

link_directories(${Boost_LIBRARY_DIRS} ${OPENSSL_LIBRARY_DIRS})

#add_executable(runfile src/main.cpp)

file(GLOB SOURCES "src/*.cpp")
add_executable(runfile ${SOURCES})
target_link_libraries(runfile ${PROJECT_LINK_LIBS} ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} pthread)
