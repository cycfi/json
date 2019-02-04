###############################################################################
#  Copyright (c) 2016-2018 Joel de Guzman
#
#  Distributed under the MIT License (https://opensource.org/licenses/MIT)
###############################################################################
cmake_minimum_required(VERSION 3.7.2)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT DEFINED INTEGRE_COMMON_ROOT)
  set(INTEGRE_COMMON_ROOT ../infra)
endif()

add_subdirectory(${INTEGRE_COMMON_ROOT} "${CMAKE_CURRENT_BINARY_DIR}/infra")

add_library(json INTERFACE)

target_include_directories(json INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB_RECURSE JSON_HEADERS include/*.hpp)

target_sources(infra INTERFACE ${JSON_HEADERS})

target_link_libraries(json INTERFACE infra)

add_subdirectory(test)

