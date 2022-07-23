# Create license
set(LICENSE_CONTENT 
"* MIT License
* 
* Copyright (c) 2022 Sebastian Kwaśniak
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the \"Software\"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE."
)

# Read Folders
## Tools
file(GLOB_RECURSE OGL_UTIL_INCLUDE_TOOLS_FILES "${CMAKE_CURRENT_LIST_DIR}/../include/oglUtil/tools/*.hpp")
set(OGL_UTIL_INCLUDE_TOOLS_FILE_CONTENT "")
foreach (TOOL_FILE ${OGL_UTIL_INCLUDE_TOOLS_FILES})
    file(RELATIVE_PATH TOOL_FILE_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/../include/" ${TOOL_FILE})
    set(OGL_UTIL_INCLUDE_TOOLS_FILE_CONTENT "${OGL_UTIL_INCLUDE_TOOLS_FILE_CONTENT} #include <${TOOL_FILE_INCLUDE}> \n")
endforeach()
file(
    WRITE "${CMAKE_CURRENT_LIST_DIR}/../include/oglUtil/ogluTools.hpp"
    "/*
${LICENSE_CONTENT}\n*/\n\n
#pragma once\n
// Include tools
${OGL_UTIL_INCLUDE_TOOLS_FILE_CONTENT}
"
)

## Drawables
file(GLOB_RECURSE OGL_UTIL_INCLUDE_DRAWABLES_FILES "${CMAKE_CURRENT_LIST_DIR}/../include/oglUtil/drawables/*.hpp")
set(OGL_UTIL_INCLUDE_DRAWABLES_FILE_CONTENT "")
foreach (TOOL_FILE ${OGL_UTIL_INCLUDE_DRAWABLES_FILES})
    file(RELATIVE_PATH TOOL_FILE_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/../include/" ${TOOL_FILE})
    set(OGL_UTIL_INCLUDE_DRAWABLES_FILE_CONTENT "${OGL_UTIL_INCLUDE_DRAWABLES_FILE_CONTENT} #include <${TOOL_FILE_INCLUDE}> \n")
endforeach()
file(
    WRITE "${CMAKE_CURRENT_LIST_DIR}/../include/oglUtil/ogluDrawables.hpp"
    "/*
${LICENSE_CONTENT}\n*/\n\n
#pragma once\n
// Include drawables
${OGL_UTIL_INCLUDE_DRAWABLES_FILE_CONTENT}
"
)

set(
    OGL_UTIL_INCLUDE_FILE_CONTENT
    "/*
${LICENSE_CONTENT}\n*/\n\n
#pragma once\n
// Include all utilities
${OGL_UTIL_INCLUDE_TOOLS_FILE_CONTENT}
${OGL_UTIL_INCLUDE_DRAWABLES_FILE_CONTENT}
"
)
file(
    WRITE "${CMAKE_CURRENT_LIST_DIR}/../include/oglUtil/oglu.hpp"
    "${OGL_UTIL_INCLUDE_FILE_CONTENT}"
)

# Create library
file(GLOB_RECURSE OGL_UTIL_FILES_CPP "${CMAKE_CURRENT_LIST_DIR}/*.cpp")
file(GLOB_RECURSE OGL_UTIL_FILES_HPP "${CMAKE_CURRENT_LIST_DIR}/*.hpp")
file(GLOB_RECURSE OGL_UTIL_INCLUDE_FILES "${CMAKE_CURRENT_LIST_DIR}/../include/*.hpp")

add_library(
    oglu
    STATIC
    ${OGL_UTIL_FILES_HPP}
    ${OGL_UTIL_FILES_CPP}
)
target_include_directories(oglu PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../include/")
target_link_libraries(oglu PUBLIC ${CORE_LIBS} glm libepoxy glfw GLEW)



