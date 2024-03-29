if (WIN32) 


elseif (APPLE)


elseif (UNIX)
    if (NOT TARGET GLEW)
        get_filename_component(path_glew "glew" REALPATH)
        if (NOT EXISTS "${path_glew}")
            execute_process(
            COMMAND wget "https://sourceforge.net/projects/glew/files/glew/2.1.0/glew-2.1.0.zip/download" "--output-document" "glew.zip"
            WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
            )
            execute_process(
                COMMAND unzip "glew.zip"
                WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
            )
            execute_process(
                COMMAND rm "glew.zip"
                WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
            )
            file(GLOB GLEW_DIR "${CMAKE_CURRENT_LIST_DIR}/glew-*")
            execute_process(
                COMMAND mv "${GLEW_DIR}" "glew"
                WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
            )
        endif()
    endif()
endif()

if (NOT TARGET GLEW)
    # GLEW
    file(GLOB_RECURSE GLEW_FILES_CPP "${CMAKE_CURRENT_LIST_DIR}/glew/src/*.c")
    file(GLOB_RECURSE GLEW_FILES_HPP "${CMAKE_CURRENT_LIST_DIR}/glew/include/GL/*.h")

    add_library(
        GLEW
        STATIC
        ${GLEW_FILES_HPP}
        ${GLEW_FILES_CPP}
    )
    target_include_directories(GLEW PUBLIC "${CMAKE_CURRENT_LIST_DIR}/glew/include/")
    target_link_libraries(GLEW PUBLIC ${CORE_LIBS} glm libepoxy glfw)
endif()
