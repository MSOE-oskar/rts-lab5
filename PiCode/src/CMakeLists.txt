cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

# This is the name of the project
project(discoLight LANGUAGES CXX)

# This defines system level include directories, where headers for system libraries are defined.
include_directories(SYSTEM /rpi_sysroot/usr/local/include /rpi_sysroot/usr/include)

# This provides additional compile options.
add_definitions(-Wall -g -O0 -pg -L/rpi_sysroot/usr/local/lib -lwiringPi -lpthread -std=c++11)

# This identifies the source code files that are relevant to the project.
file (GLOB_RECURSE Foo_SOURCES CONFIGURE_DEPENDS "*.cpp")
file (GLOB_RECURSE Foo_HEADERS CONFIGURE_DEPENDS "*.h")
set (Foo_INCLUDE_DIRS "")
foreach (_headerFile ${Foo_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND Foo_INCLUDE_DIRS ${_dir})
endforeach()

list (REMOVE_DUPLICATES Foo_INCLUDE_DIRS)
# This defines that an executable is to be created.
add_executable(discoLight ${Foo_SOURCES})
target_include_directories(discoLight PRIVATE ${Foo_INCLUDE_DIRS})

# Find the doxygen tool
find_package(Doxygen)

include(UseDoxygenDoc)

add_doxygen_doc(
  BUILD_DIR
    ${CMAKE_CURRENT_BINARY_DIR}/../docs/
  DOXY_FILE
    ${CMAKE_CURRENT_SOURCE_DIR}/../docs/Doxyfile.in
  TARGET_NAME
    docs
  COMMENT
    "HTML documentation"
  )


# This defines the libraries which are needed by this project for it to link properly.
target_link_libraries(discoLight PUBLIC wiringPi pthread rt)



