
file (GLOB_RECURSE EXAMPLES CONFIGURE_DEPENDS ./*.cpp)
foreach (example ${EXAMPLES})
  get_filename_component(example_name ${example} NAME_WLE)
  add_executable(${example_name} ${example})
  target_link_directories(${example_name} PUBLIC "${CMAKE_SOURCE_DIR}/submodules/agl/shaders/")
  target_link_libraries(${example_name} ${CORE_LIBS} OGL_UTIL_LIB GLM_LIB)
endforeach()