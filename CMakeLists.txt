cmake_minimum_required(VERSION 3.16)
project(libgo)


set(ROOT ${CMAKE_CURRENT_SOURCE_DIR})


# add_executable(sample1 ${ROOT}/sample1.cpp)
# target_link_libraries(sample1 pthread liblibgo.a dl)

# add_executable(sample2 ${ROOT}/sample2.cpp)
# target_link_libraries(sample2 liblibgo.a  dl pthread)

# add_executable(sample3 ${ROOT}/sample3.cpp)
# target_link_libraries(sample3 liblibgo.a  dl pthread)

# add_executable(sample4 ${ROOT}/sample4.cpp)
# target_link_libraries(sample4 liblibgo.a  dl pthread)

# aux_source_directory(${PROJECT_SOURCE_DIR} SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC_LIST)

foreach(var ${SRC_LIST})
    string(REGEX REPLACE ".*/" "" var ${var})
    string(REGEX REPLACE ".cpp" "" target ${var})
    add_executable(${target}.t ${var})
    target_link_libraries(${target}.t libgo ${Boost_LIBRARIES} pthread dl)
endforeach(var)
