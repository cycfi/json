###############################################################################
#  Copyright (c) 2016-2020 Joel de Guzman, Michal Urbanski
#
#  Distributed under the MIT License (https://opensource.org/licenses/MIT)
###############################################################################
cmake_minimum_required(VERSION 3.12)

if (TARGET json)
   return()
endif()

project(json)

option(JSON_BUILD_TESTS "build json library tests" ON)

###############################################################################
# Boost

cmake_policy(SET CMP0074 NEW)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.61 REQUIRED)

###############################################################################
# json

add_subdirectory(infra)
add_library(json INTERFACE)

target_compile_features(json INTERFACE cxx_std_17)

target_include_directories(json INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")

# disable auto-linking
target_compile_definitions(json INTERFACE BOOST_ALL_NO_LIB)

target_compile_options(json INTERFACE
   $<$<CXX_COMPILER_ID:GNU>:-ftemplate-backtrace-limit=0>
   $<$<CXX_COMPILER_ID:Clang>:-ftemplate-backtrace-limit=0>
)

target_link_libraries(json INTERFACE cycfi::infra Boost::boost)

add_library(cycfi::json ALIAS json)

if (JSON_BUILD_TESTS)
   include(CTest)
   add_subdirectory(test)
endif()
