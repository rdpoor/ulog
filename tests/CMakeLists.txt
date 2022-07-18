project(ulog_example)

include_directories(${PROJECT_SOURCE_DIR}/../src)

add_executable(ulog_example ulog_example.c)

target_link_libraries(ulog_example ulog)
