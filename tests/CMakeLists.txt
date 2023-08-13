file(GLOB SRC
        "*.cpp"
)

add_executable(test-base64 test.cpp)
add_executable(measure-time measure-time.cpp)

target_link_libraries(test-base64
        base64)
target_link_libraries(measure-time
        base64)
