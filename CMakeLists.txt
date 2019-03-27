cmake_minimum_required(VERSION 3.10)
project(AMException C)

set(CMAKE_C_STANDARD 11)

set(headers AMException.h)
set(sources test.c)

add_library(AMException AMException.h AMException.c)
add_executable(AMExceptionTest ${sources} ${headers})
target_link_libraries(AMExceptionTest AMException)
