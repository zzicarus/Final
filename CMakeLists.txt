cmake_minimum_required(VERSION 3.10)

# Project name
project(OOP_Final)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)
include_directories(tinyxml)

# Source files
set(SOURCE_FILES
    src/main.cpp
    tinyxml/tinyxml2.cpp
)

# Executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Link libraries if needed
# target_link_libraries(${PROJECT_NAME} your_library)

# Custom commands (if necessary)
# add_custom_command(
#     TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:${PROJECT_NAME}>/data
# )
