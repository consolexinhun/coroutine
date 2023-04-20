cmake_minimum_required(VERSION 3.16)
project(libgo)

find_package(Boost REQUIRED COMPONENTS 
    system 
    filesystem
)

include_directories(
    ${Boost_INCLUDE_DIRS}
)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC_LIST)

foreach(var ${SRC_LIST})
    # string(REGEX REPLACE ".*/" "" var ${var})
    # string(REGEX REPLACE ".cpp" "" target ${var})
    get_filename_component(target ${var} NAME_WE)
    add_executable(${target} ${var})
    target_link_libraries(${target} libgo ${Boost_LIBRARIES} curl pthread dl)
endforeach(var)
