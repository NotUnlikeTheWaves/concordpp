cmake_minimum_required(VERSION 3.1.3 FATAL_ERROR)

# Force external build because source tree builds make a mess.
if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR} AND NOT WIN32)
	    message(FATAL_ERROR "You can not use CMake to build from the root of it's source tree! Remove the CMakeCache.txt file from this directory, then create a separate directory (either below this directory or elsewhere), and then re-run CMake from there.")
endif(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR} AND NOT WIN32)

project(concordpp CXX)
enable_language(C)

include(ExternalProject)

#set(CMAKE_BUILD_TYPE Release)
# Use boost multithreading
set(Boost_USE_MULTITHREADED  ON)
find_package(Boost 1.58 REQUIRED COMPONENTS system)

find_package(OpenSSL REQUIRED openssl)
find_package(CURL REQUIRED)
# Required because boost/C++11 uses pthread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/websocketpp)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/restclient-cpp)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(${PROJECT_NAME} ${SOURCES})
set_target_properties(${PROJECT_NAME}
	PROPERTIES LINKER_LANGUAGE CXX
	LINK_FLAGS "${LINKFLAGS}"
	# Require C++11, NOTE: this requires us to use CMake 3.1.3 at minimum
	CXX_STANDARD 11
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS NO # use -std=c++11 and not -std=gnu++11
)

include_directories(
		${CMAKE_CURRENT_SOURCE_DIR}/include
		${Boost_INCLUDE_DIRS}
		${CMAKE_CURRENT_SOURCE_DIR}/libs/websocketpp
		${CMAKE_CURRENT_SOURCE_DIR}/libs/json
		${CURL_INCLUDE_DIRS}
)
target_link_libraries(${PROJECT_NAME} ${PROJECT_LINK_LIBS} ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} ${CURL_LIBRARIES} Threads::Threads restclient-cpp)
