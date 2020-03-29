###############################################################################
#  Copyright (c) 2016-2018 Joel de Guzman
#
#  Distributed under the MIT License (https://opensource.org/licenses/MIT)
###############################################################################
project(cycfi::json)
cmake_minimum_required(VERSION 3.7.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"
      OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}-ftemplate-backtrace-limit=0")

endif()

###############################################################################
# infra

if (NOT DEFINED INTEGRE_COMMON_ROOT)
  set(INTEGRE_COMMON_ROOT ../infra)
endif()

add_subdirectory(${INTEGRE_COMMON_ROOT} "${CMAKE_CURRENT_BINARY_DIR}/infra")

###############################################################################
# Boost

cmake_policy(SET CMP0074 NEW)

find_package(
  Boost 1.61 REQUIRED
  )
include_directories(${Boost_INCLUDE_DIRS})

set(BOOST_CMAKE_ARGS)
if (DEFINED BOOST_ROOT)
   set(BOOST_CMAKE_ARGS ${BOOST_CMAKE_ARGS} "-DBOOST_ROOT=${BOOST_ROOT}")
endif()
if (DEFINED BOOST_INCLUDEDIR)
   set(BOOST_CMAKE_ARGS ${BOOST_CMAKE_ARGS}
      "-DBOOST_INCLUDEDIR=${BOOST_INCLUDEDIR}")
endif()
if (DEFINED BOOST_LIBRARYDIR)
   set(BOOST_CMAKE_ARGS ${BOOST_CMAKE_ARGS}
      "-DBOOST_LIBRARYDIR=${BOOST_LIBRARYDIR}")
endif()
###############################################################################
# json

add_library(json INTERFACE)

target_include_directories(json INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB_RECURSE JSON_HEADERS include/*.hpp)

target_sources(infra INTERFACE ${JSON_HEADERS})

target_link_libraries(json INTERFACE infra)

add_subdirectory(test)

