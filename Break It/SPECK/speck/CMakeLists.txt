set(CMAKE_LEGACY_CYGWIN_WIN32 0)
cmake_minimum_required (VERSION 2.6)
project(speck)

include_directories(.)
add_definitions(-Wall)
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

add_library(speck speck.cpp)

add_executable(speck-cli speck_cli.cpp)

add_executable(speck-command speck_command.cpp unixerror.cpp)

target_link_libraries(speck-cli speck)

target_link_libraries(speck-command speck)

# Testing
find_package(Boost COMPONENTS unit_test_framework)

add_executable(speck-tests tests/speck.test.cpp)
set_target_properties(speck-tests PROPERTIES COMPILE_FLAGS
	"-DBOOST_TEST_DYN_LINK")

# CMake dose not find gmp under cygwin. Manually naming these libraries
target_link_libraries(speck-tests 
	${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

enable_testing()
add_test(SpeckTest speck-tests)
