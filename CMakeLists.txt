cmake_minimum_required (VERSION 3.20)

project (2048 LANGUAGES C VERSION 1.0)

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
add_subdirectory(${CMAKE_SOURCE_DIR}/src/Game)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/Interface_SDL2)

if (WIN32)
    set(ICONE 2048.rc)
else ()
    # linux .desktop file
    set (2048_DESKTOP_DIR     "/usr/share/applications/")
    set (2048_PIXMAPS_DIR     "/usr/share/icons/")

    configure_file(2048.desktop.in 2048.desktop)

    install (FILES ${CMAKE_BINARY_DIR}/2048.desktop DESTINATION ${2048_DESKTOP_DIR})
    install(FILES ${CMAKE_SOURCE_DIR}/2048.svg DESTINATION ${2048_PIXMAPS_DIR})
endif()

add_executable ( ${PROJECT_NAME} ${SRCS} ${HEADERS} ${ICONE})
target_link_libraries(${PROJECT_NAME} PUBLIC Game GameEngine Interface_SDL2)
set_target_warnings(${PROJECT_NAME})

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME} RUNTIME)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(InstallRequiredSystemLibraries)
set (CPACK_PACKAGE_NAME ${PROJECT_NAME})
set (CPACK_PACKAGE_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}")
set (CPACK_PACKAGE_VENDOR "Quentin Horgues")
set (CPACK_SOURCE_GENERATOR "TGZ")
set (CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
set (CPACK_SOURCE_IGNORE_FILES ${CPACK_IGNORE_FILES})
set (CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})

# Debian
set (CPACK_DEBIAN_PACKAGE_MAINTAINER "quentin.horgues@outlook.fr")
set (CPACK_DEBIAN_PACKAGE_ARCHITECTURE "x86")
set (CPACK-DEBIAN-PACKAGE-DESCRIPTION "Le jeu du 2048 en français")
set (CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 libsdl2 libsdl2-ttf")

# RPM
set (CPACK-RPM-PACKAGE-ARCHITECTURE "x86")
set (CPACK_RPM_PACKAGE_REQUIRES "SDL2 SDL2_ttf")
# NSIS
set (CPACK_NSIS_MODIFY_PATH OFF)
set (CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/2048.ico")
set (CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}/2048_uninstaler.ico")
set (CPACK_CREATE_DESKTOP_LINK ON)
set (CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
    CreateShortCut \\\"$DESKTOP\\\\${PROJECT_NAME}.lnk\\\" \\\"$INSTDIR\\\\bin\\\\${PROJECT_NAME}.exe\\\"
")

set (CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
    Delete \\\"$DESKTOP\\\\${PROJECT_NAME}.lnk\\\"
")

include(CPack)
