
file (GLOB_RECURSE EXAMPLES CONFIGURE_DEPENDS ./*.cpp)
foreach (example ${EXAMPLES})
  get_filename_component(example_name ${example} NAME_WLE)
  add_executable(${example_name} ${example})
  target_link_libraries(${example_name} ${CORE_LIBS} oglu glm glfw GLEW whereami)
endforeach()