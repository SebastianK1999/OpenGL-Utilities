cmake_minimum_required(VERSION 3.10.2)
project(OpenGL_Utilities VERSION 0.1)


if (WIN32) # Include win64 platforms

    find_package(OpenGL REQUIRED)
    find_library(GLEW NAMES glew32s PATHS external/lib/x64)
    find_library(GLFW NAMES glfw3 PATHS external/lib)

    set(INCLUDE_DIRS
        .
        external/include
    )

    set(LIBRARY_DIRS
        external/lib
    )

    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_FLAGS 
        "/wd4018 /wd4244 /wd4305 
        /D_CRT_SECURE_NO_WARNINGS 
        /D_CRT_NONSTDC_NO_DEPRECATE 
        /D NOMINMAX /DGLEW_STATIC
        /EHsc"
    )

    set(VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
    set(CMAKE_EXE_LINKER_FLAGS "/NODEFAULTLIB:\"MSVCRT\" /NODEFAULTLIB:\"LIBCMT\"")
    set(CORE_LIBS ${GLEW} ${GLFW} opengl32.lib)
    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
    set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

elseif (APPLE)

    set(CMAKE_MACOSX_RPATH 1)
    set(CMAKE_CXX_FLAGS "-Wall -Wno-deprecated-declarations -Wno-reorder-ctor -Wno-unused-function -Wno-unused-variable -g -stdlib=libc++ -std=c++14")
    find_library(GL_LIB OpenGL)
    find_library(GLFW glfw)
    add_definitions(-DAPPLE)
    set(CORE_LIBS ${GLFW} ${GL_LIB})

elseif (UNIX)

    set(CMAKE_CXX_FLAGS "-Wall -march=native -O3 -g -std=c++20 -Wno-comment -Wno-sign-compare -Wno-reorder -Wno-unused-function")
    FIND_PACKAGE(OpenGL REQUIRED) 
    #FIND_PACKAGE(GLEW REQUIRED)
    add_definitions(-DUNIX)
    set(CORE_LIBS GL X11)

endif()
SET(CORE_LIBS  "${CORE_LIBS}" CACHE INTERNAL "CORE_LIBS")

set(CMAKE_CXX_STANDARD_LIBRARIES "-lGL")

#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#                COMMAND ${CMAKE_COMMAND} -E copy_directory
#                    ${CMAKE_SOURCE_DIR}/res/ $<TARGET_FILE_DIR:${PROJECT_NAME}>)

add_subdirectory("external") # linking submodules
add_subdirectory("submodules") # linking submodules
add_subdirectory("src") # linking sourcefiles
add_subdirectory("bin") # create executable