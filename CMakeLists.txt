cmake_minimum_required(VERSION 3.12)

project(VALUE)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_options( -std=c++14 )
    message("VALUE Debug.build")
ELSE()
    add_compile_options( -O3 -std=c++14 )
    message("VALUE Release.build")
ENDIF()

if (CMAKE_VERSION VERSION_LESS 3.2)
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "")
else()
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")
endif()

include(DownloadProject.cmake)
download_project(PROJ                googletest
                 GIT_REPOSITORY      https://github.com/google/googletest.git
                 GIT_TAG             master
                 ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Disable building tests for GoogleTest
set(gtest_build_tests OFF CACHE BOOL "" FORCE)
set(gtest_build_samples OFF CACHE BOOL "" FORCE)

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

##download_project(PROJ                googlebench
##                 GIT_REPOSITORY      https://github.com/google/benchmark.git
##                 GIT_TAG             master
##                 ${UPDATE_DISCONNECTED_IF_AVAILABLE}
##)

# Disable building tests for GoogleBenchmark
##set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)

##add_subdirectory(${googlebench_SOURCE_DIR} ${googlebench_BINARY_DIR})

add_subdirectory(boost-cmake)


add_executable(recursive_value_tests tests/recursive_value_tests.cpp)
target_include_directories(recursive_value_tests PUBLIC $<BUILD_INTERFACE:${VALUE_SOURCE_DIR}/include/value>)
target_link_libraries(recursive_value_tests gtest gmock_main)
target_link_libraries(recursive_value_tests Boost::boost)
add_test(NAME recursive_value_tests COMMAND recursive_value_tests)

function(build_source_file base_name )
  message(STATUS ${base_name})
  add_executable(${base_name} ${base_name}.cpp)
  target_include_directories(${base_name} PUBLIC $<BUILD_INTERFACE:${VALUE_SOURCE_DIR}/include>)
endfunction(build_source_file)


list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/)
include (gcovr)

add_subdirectory("example")