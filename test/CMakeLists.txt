set(TEST_SOURCES test_main.cpp
                 algorithm/searching/bitap.cpp
                 algorithm/searching/element_position_bitmask_table.cpp
                 algorithm/sorting/counting_sort.cpp
                 iterator/intersect_iterator.cpp
                 iterator/join_iterator.cpp
                 iterator/merge_iterator.cpp
                 range/skip_to.cpp)

set(TEST_EXECUTABLE test)

add_executable(${TEST_EXECUTABLE} ${TEST_SOURCES})
target_link_libraries(${TEST_EXECUTABLE} ${Boost_LIBRARIES})

add_custom_target(check COMMAND ${TEST_EXECUTABLE} --log_level=test_suite --report_level=short)