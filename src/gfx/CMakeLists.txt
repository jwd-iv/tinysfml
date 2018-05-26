file(GLOB_RECURSE TINYSFML_GFX_src *.cpp)
file(GLOB_RECURSE TINYSFML_GFX_hdr
  ${PROJECT_SOURCE_DIR}/include/tiny/sfml/gfx/*.h*)

add_library(tinysfml-gfx STATIC ${TINYSFML_GFX_src} ${TINYSFML_GFX_hdr}) 
set_target_properties(tinysfml-gfx PROPERTIES
    OUTPUT_NAME tinysfml-gfx
    DEBUG_POSTFIX -d
    FOLDER tinysfml
)

target_link_libraries(tinysfml-gfx tiny)
target_link_libraries(tinysfml-gfx sfml-graphics)

if(TINY_EDITOR)
  target_link_libraries(tinysfml-gfx tiny-editor)
endif()

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/tiny/sfml/gfx
    DESTINATION include/tiny/sfml
)
install(
    TARGETS tinysfml-gfx
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)
