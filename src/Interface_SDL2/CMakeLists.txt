cmake_minimum_required (VERSION 3.20)

project (Interface_SDL2 LANGUAGES C VERSION 0.1)

include(CTest)
enable_testing()

include(${CMAKE_SOURCE_DIR}/cmake/option.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/CompilerWarnings.cmake)

set_choice (CMAKE_BUILD_TYPE "Debug;Release;MinSizeRel;RelWithDebInfo" "Debug" "Choose type of build")

set (SRCS
  ${CMAKE_SOURCE_DIR}/src/Interface_SDL2/Interface_SDL2.c
)

set (HEADERS
  ${CMAKE_SOURCE_DIR}/src/Interface.h
)

set (ASSETS
  ${CMAKE_SOURCE_DIR}/assets/calibrib.ttf
  ${CMAKE_SOURCE_DIR}/assets/imprisha.ttf
)

if (MINGW)
    set (CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")

    if (${CMAKE_BUILD_TYPE} MATCHES "Release" OR ${CMAKE_BUILD_TYPE} MATCHES "MinSizeRel")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
    endif()
endif (MINGW)

message(STATUS "${CMAKE_MODULE_PATH}")

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

set (CMAKE_C_STANDARD 17)
set (CMAKE_C_STANDARD_REQUIRED ON)

set_sanitizer ()

add_library(${PROJECT_NAME} ${SRCS} ${HEADERS})
#set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER ${HEADERS})
target_link_libraries(${PROJECT_NAME} m SDL2::SDL2 SDL2_ttf::SDL2_ttf)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)
set_target_warnings(${PROJECT_NAME})

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2PP_INCLUDE_DIRS})

# Win32 SDL2 dll
if (WIN32)
  
	# SDL2
  set_option(SDL2_DLL_DIR "SDL2_BINARY_DIR NOT FOUND" PATH "Path to SDL2.dll directory")
  configure_file(${SDL2_DLL_DIR}/SDL2.dll SDL2.dll COPYONLY)
  install(FILES ${SDL2_DLL_DIR}/SDL2.dll DESTINATION .)
	
	# SDL2 ttf
	set_option(SDL2_TTF_DLL_DIR "SDL2_TTF_BINARY_DIR NOT FOUND" PATH "Path to SDL2_ttf.dll directory")
  configure_file(${SDL2_TTF_DLL_DIR}/SDL2_ttf.dll SDL2_ttf.dll COPYONLY)
  install(FILES ${SDL2_TTF_DLL_DIR}/SDL2_ttf.dll DESTINATION .)

#  set_option(SDL2_IMAGE_DLL_DIR "SDL2_IMAGE_BINARY_DIR NOT FOUND" PATH "Path to SDL2_image.dll directory")
#  configure_file(${SDL2_IMAGE_DLL_DIR}/SDL2_image.dll SDL2_image.dll COPYONLY)
#  install(FILES ${SDL2_IMAGE_DLL_DIR}/SDL2_image.dll DESTINATION .)
#
#  set_option(SDL2_MIXER_DLL_DIR "SDL2_MIXER_BINARY_DIR NOT FOUND" PATH "Path to SDL2_mixer.dll directory")
#  configure_file(${SDL2_MIXER_DLL_DIR}/SDL2_mixer.dll SDL2_mixer.dll COPYONLY)
#  install(FILES ${SDL2_MIXER_DLL_DIR}/SDL2_mixer.dll DESTINATION .)

  foreach(df ${ASSETS})
      configure_file(${CMAKE_SOURCE_DIR}/${df} ${CMAKE_BINARY_DIR}/${df} COPYONLY)
  endforeach()
  

endif(WIN32)


include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
install(DIRECTORY assets/ DESTINATION assets)

foreach(df ${ASSETS})
    file(COPY ${df} DESTINATION ${CMAKE_BINARY_DIR}/assets)
endforeach()


include(InstallRequiredSystemLibraries)
set(CPACK_GENERATOR NSIS)
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION "${ABC_VERSION_MAJOR}.${ABC_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_VENDOR "Quentin Horgues")
set (CPACK_NSIS_MODIFY_PATH "ON")
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
set(CPACK_SOURCE_IGNORE_FILES ${CPACK_IGNORE_FILES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)