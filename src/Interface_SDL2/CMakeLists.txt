cmake_minimum_required (VERSION 3.20)

project (Interface_SDL2 LANGUAGES C VERSION 1.0)

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
  assets/calibrib.ttf
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
  configure_file(${SDL2_DLL_DIR}/SDL2.dll ${CMAKE_BINARY_DIR}/SDL2.dll COPYONLY)
  install(FILES ${SDL2_DLL_DIR}/SDL2.dll DESTINATION bin)
	
	# SDL2 ttf
	set_option(SDL2_TTF_DLL_DIR "SDL2_TTF_BINARY_DIR NOT FOUND" PATH "Path to SDL2_ttf.dll directory")
  configure_file(${SDL2_TTF_DLL_DIR}/SDL2_ttf.dll ${CMAKE_BINARY_DIR}/SDL2_ttf.dll COPYONLY)
  install(FILES ${SDL2_TTF_DLL_DIR}/SDL2_ttf.dll DESTINATION bin)

#  set_option(SDL2_IMAGE_DLL_DIR "SDL2_IMAGE_BINARY_DIR NOT FOUND" PATH "Path to SDL2_image.dll directory")
#  configure_file(${SDL2_IMAGE_DLL_DIR}/SDL2_image.dll ${CMAKE_BINARY_DIR}/SDL2_image.dll COPYONLY)
#  install(FILES ${SDL2_IMAGE_DLL_DIR}/SDL2_image.dll DESTINATION .)
#
#  set_option(SDL2_MIXER_DLL_DIR "SDL2_MIXER_BINARY_DIR NOT FOUND" PATH "Path to SDL2_mixer.dll directory")
#  configure_file(${SDL2_MIXER_DLL_DIR}/SDL2_mixer.dll ${CMAKE_BINARY_DIR}/SDL2_mixer.dll COPYONLY)
#  install(FILES ${SDL2_MIXER_DLL_DIR}/SDL2_mixer.dll DESTINATION .) 

endif(WIN32)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets/ DESTINATION bin/assets)

foreach(df ${ASSETS})
    file(COPY ${CMAKE_SOURCE_DIR}/${df} DESTINATION ${CMAKE_BINARY_DIR}/assets)
endforeach()