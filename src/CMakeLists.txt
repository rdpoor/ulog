# Set the project name
project(ulog)

# Add a library with the above sources
add_library(${PROJECT_NAME} EXCLUDE_FROM_ALL ulog.c)

target_include_directories(${PROJECT_NAME} PUBLIC .)
