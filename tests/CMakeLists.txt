project(ulog_example)

add_executable(${PROJECT_NAME} ulog_example.c)
target_include_directories(${PROJECT_NAME} PRIVATE ulog)
target_link_libraries(${PROJECT_NAME} ulog)
