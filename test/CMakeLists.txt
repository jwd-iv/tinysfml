add_executable(tinysfml-test main.cpp)

target_link_libraries(tinysfml-test tiny_core)
target_link_libraries(tinysfml-test tinysfml-gfx)
target_link_libraries(tinysfml-test tinysfml-sfx)
target_link_libraries(tinysfml-test tinysfml-sys)
if(TINY_EDITOR)
  target_link_libraries(tinysfml-test tiny-editor)
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set_target_properties(tinysfml-test PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
  set_target_properties(tinysfml-test PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/test")
  set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT tinysfml-test)
endif()

add_test(NAME tinysfmltest COMMAND tinysfml-test WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/test)
