cmake_minimum_required(VERSION 3.0.0)
project(update_field VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(update_field update_field.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
