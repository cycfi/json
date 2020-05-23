###############################################################################
#  Copyright (c) 2016-2020 Joel de Guzman, Michal Urbanski
#
#  Distributed under the MIT License (https://opensource.org/licenses/MIT)
###############################################################################
cmake_minimum_required(VERSION 3.12)

if (TARGET json)
   return()
endif()

project(cycfi-json LANGUAGES CXX VERSION 0.1.0)

option(JSON_BUILD_TESTS "build json library tests" ON)

###############################################################################
# Boost

cmake_policy(SET CMP0074 NEW)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.61 REQUIRED)

###############################################################################
# json

# add_subdirectory(infra)
add_library(json INTERFACE)

target_compile_features(json INTERFACE cxx_std_17)

include(GNUInstallDirs)
target_include_directories(
   json INTERFACE
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/cycfi-json-${PROJECT_VERSION}>
)

# disable auto-linking
target_compile_definitions(json INTERFACE BOOST_ALL_NO_LIB)

target_compile_options(json INTERFACE
   $<BUILD_INTERFACE:
      $<$<CXX_COMPILER_ID:GNU>:-ftemplate-backtrace-limit=0>
      $<$<CXX_COMPILER_ID:Clang>:-ftemplate-backtrace-limit=0>
   >
)

target_link_libraries(json INTERFACE Boost::boost)

add_library(cycfi::json ALIAS json)

if (JSON_BUILD_TESTS)
   include(CTest)
   add_subdirectory(infra)
   add_subdirectory(test)
endif()

install(
   TARGETS
      json

   EXPORT
     cycfi-json-targets

   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
   RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
 )

 install(
   EXPORT
     cycfi-json-targets

   NAMESPACE cycfi::
   DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cycfi-json-${PROJECT_VERSION}/
 )

 install(
   DIRECTORY
     include/

   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cycfi-json-${PROJECT_VERSION}/cycfi
   FILES_MATCHING PATTERN "json.hpp"
 )

 include(CMakePackageConfigHelpers)
 write_basic_package_version_file(
   ${CMAKE_CURRENT_BINARY_DIR}/cycfi-json-config-version.cmake
   VERSION ${PROJECT_VERSION}
   COMPATIBILITY AnyNewerVersion
 )

 configure_package_config_file(
   ${CMAKE_CURRENT_LIST_DIR}/cmake/cycfi-json-config.cmake.in
   ${CMAKE_CURRENT_BINARY_DIR}/cycfi-json-config.cmake
   INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cycfi-json-${PROJECT_VERSION}
 )

 install(
   FILES
     ${CMAKE_CURRENT_BINARY_DIR}/cycfi-json-config.cmake
     ${CMAKE_CURRENT_BINARY_DIR}/cycfi-json-config-version.cmake
   DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cycfi-json-${PROJECT_VERSION}
 )
