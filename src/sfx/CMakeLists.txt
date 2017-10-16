file(GLOB_RECURSE TINYSFML_SFX_src *.cpp)
file(GLOB_RECURSE TINYSFML_SFX_hdr
  {PROJECT_SOURCE_DIR}/include/tiny/sfml/sfx/*.h*)

add_library(tinysfml-sfx STATIC ${TINYSFML_SFX_src} ${TINYSFML_SFX_hdr}) 
set_target_properties(tinysfml-sfx PROPERTIES
    OUTPUT_NAME tinysfml-sfx
    DEBUG_POSTFIX -d
    FOLDER tinysfml
)

target_link_libraries(tinysfml-sfx tiny)
target_link_libraries(tinysfml-sfx sfml-audio)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/tiny/sfml/sfx
    DESTINATION include/tiny/sfml
)
install(
    TARGETS tinysfml-sfx
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)
