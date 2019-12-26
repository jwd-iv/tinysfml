file(GLOB_RECURSE TINYSFML_SYS_src *.cpp)
file(GLOB_RECURSE TINYSFML_SYS_hdr
  ../../include/tiny/sfml/system/*.h*)

add_library(tinysfml-sys STATIC ${TINYSFML_SYS_src} ${TINYSFML_SYS_hdr}) 
set_target_properties(tinysfml-sys PROPERTIES
    OUTPUT_NAME tinysfml-sys
    DEBUG_POSTFIX -d
    FOLDER tinysfml
)

target_link_libraries(tinysfml-sys tiny)
target_link_libraries(tinysfml-sys sfml-system)
target_link_libraries(tinysfml-sys sfml-window)
target_link_libraries(tinysfml-sys sfml-main)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/tiny/sfml/sys
    DESTINATION include/tiny/sfml
)
install(
    TARGETS tinysfml-sys
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)
