include(CTest)

file(GLOB SRC "*.cpp")

add_executable(test-base64 test.cpp)
add_executable(measure-time measure-time.cpp)

target_link_libraries(test-base64 base64)
target_link_libraries(measure-time base64)

add_test(NAME test-base64 COMMAND tests/test-base64 WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
add_test(NAME measure-time COMMAND tests/measure-time WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
