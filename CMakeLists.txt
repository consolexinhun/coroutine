cmake_minimum_required(VERSION 3.16)
project(libgo)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC_LIST)

foreach(var ${SRC_LIST})
    string(REGEX REPLACE ".*/" "" var ${var})
    string(REGEX REPLACE ".cpp" "" target ${var})
    add_executable(${target}.t ${var})
    target_link_libraries(${target}.t libgo ${Boost_LIBRARIES} pthread dl)
endforeach(var)
