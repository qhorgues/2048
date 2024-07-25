cmake_minimum_required (VERSION 3.20)

project (Board LANGUAGES C VERSION 0.1)

include (${CMAKE_SOURCE_DIR}/cmake/CompilerWarnings.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/option.cmake)

set_choice (CMAKE_BUILD_TYPE "Debug;Release;MinSizeRel;RelWithDebInfo" "Debug" "Choose type of build")

set (SRCS
    Board/Board.c
)

set (HEADERS
    Board/Board.h
)

set_sanitizer()

set (CMAKE_C_STANDARD 17)
set (CMAKE_C_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} ${SRCS} ${HEADERS})
target_link_libraries(${PROJECT_NAME} PUBLIC m)
set_target_warnings(${PROJECT_NAME})