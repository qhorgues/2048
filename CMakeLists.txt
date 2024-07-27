cmake_minimum_required (VERSION 3.20)

project (2048 LANGUAGES C VERSION 0.1)

#include(CTest)
#enable_testing()

include(${CMAKE_SOURCE_DIR}/cmake/option.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/CompilerWarnings.cmake)

set_choice (CMAKE_BUILD_TYPE "Debug;Release;MinSizeRel;RelWithDebInfo" "Debug" "Choose type of build")

set (SRCS
    ${CMAKE_SOURCE_DIR}/src/main.c
)

set (HEADERS
    ${CMAKE_SOURCE_DIR}/src/Interactions.h
    ${CMAKE_SOURCE_DIR}/src/GameStatus.h
)

set_sanitizer()


option(BUILD_DOC "Build documentation" ON)
if (BUILD_DOC)
    add_subdirectory(${CMAKE_SOURCE_DIR}/docs)
endif()

set (CMAKE_C_STANDARD 17)
set (CMAKE_C_STANDARD_REQUIRED ON)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/GameEngine)

add_executable ( ${PROJECT_NAME} ${SRCS} ${HEADERS})
target_link_libraries(${PROJECT_NAME} PUBLIC GameEngine)
set_target_warnings(${PROJECT_NAME})

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME} RUNTIME)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}")
set(CPACK_PACKAGE_VENDOR "Quentin Horgues")
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
set(CPACK_SOURCE_IGNORE_FILES ${CPACK_IGNORE_FILES})
SET(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "quentin.horgues@outlook.fr")
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6")
SET(CPACK_PACKAGE_VENDOR "Quentin Horgues")
set (CPACK_NSIS_MODIFY_PATH "ON")
    
include(CPack)