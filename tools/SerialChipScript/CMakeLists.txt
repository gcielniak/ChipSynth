cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(SerialChipScript)

set(CMAKE_CXX_STANDARD 11)

set(EXTRA_INCLUDE_DIRS include)
set(EXTRA_LIBRARY_DIRS "")
set(EXTRA_LIBRARIES "")

# Boost
find_package(Boost REQUIRED COMPONENTS system)
set (EXTRA_INCLUDE_DIRS ${EXTRA_INCLUDE_DIRS} ${Boost_INCLUDE_DIR})
set (EXTRA_LIBRARY_DIRS ${EXTRA_LIBRARY_DIRS} $ENV{BOOST_LIBRARYDIR})
set (EXTRA_LIBRARIES ${EXTRA_LIBRARIES} ${Boost_LIBRARIES})
add_definitions(-D_SCL_SECURE_NO_WARNINGS)

include_directories(${EXTRA_INCLUDE_DIRS})
link_directories(${EXTRA_LIBRARY_DIRS})

set (SOURCES
	include/SerialPort.h
	src/SerialChipScriptApp.cpp)
	
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBRARIES})

