###############################################################################
#  Copyright (c) 2016-2018 Joel de Guzman
#
#  Distributed under the MIT License (https://opensource.org/licenses/MIT)
###############################################################################

project(json_test)

###############################################################################
# The executable App

add_executable(json_test)
target_sources(json_test PRIVATE main.cpp)

target_link_libraries(json_test PRIVATE cycfi::json)

add_test(NAME json_test COMMAND json_test)
