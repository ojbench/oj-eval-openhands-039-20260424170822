
cmake_minimum_required(VERSION 3.10)
project(CSRMatrix)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create an executable that will be used for testing
add_executable(code main.cpp)

# Set compiler options for better warnings and optimization
target_compile_options(code PRIVATE -Wall -Wextra -O2)

# Link necessary libraries if needed
# target_link_libraries(code)
