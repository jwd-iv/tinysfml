cmake_minimum_required(VERSION 3.8)
enable_testing()

project(tinysfml)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(CMAKE_C_FLAGS_DEBUG   "${CMAKE_C_FLAGS_DEBUG} /MTd")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
endif()

set(BUILD_SHARED_LIBS FALSE)

option(TINY_SFML_GFX "Build SFML renderer"          ON)
option(TINY_SFML_SFX "Build SFML sound system"      ON)
option(TINY_SFML_SYS "Build SFML platform services" ON)

# Download tinyeng and SFML if not present
if(NOT EXISTS "${TINYSFML_SOURCE_DIR}/../tinyeng/CMakeLists.txt"
OR NOT EXISTS "${TINYSFML_SOURCE_DIR}/../ext/SFML/CMakeLists.txt")
    configure_file(CMakeLists.txt.in ${CMAKE_BINARY_DIR}/tinysfml-reqs/CMakeLists.txt)
    execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/tinysfml-reqs" )
    execute_process(COMMAND "${CMAKE_COMMAND}" --build .
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/tinysfml-reqs" )
endif()

# Adding tinyeng
if(NOT EXISTS "${TINYSFML_SOURCE_DIR}/../tinyeng/CMakeLists.txt")
    message(FATAL_ERROR "Unable to find or download tinyeng, something's gone wrong.")
endif()
set(TINY_DOCS                OFF CACHE BOOL " " FORCE)
set(RIKU_DOCS                OFF CACHE BOOL " " FORCE)
set(TINY_TEST                OFF CACHE BOOL " " FORCE)
add_subdirectory("${TINYSFML_SOURCE_DIR}/../tinyeng" "${CMAKE_CURRENT_BINARY_DIR}/tinyeng_build")
message(STATUS "tinyeng found and added to build!")

# Adding SFML
if(NOT EXISTS "${TINYSFML_SOURCE_DIR}/../ext/SFML/CMakeLists.txt")
    message(FATAL_ERROR "Unable to find or download SFML, something's gone wrong.")
endif()
set(SFML_BUILD_NETWORK       OFF CACHE BOOL " " FORCE)
set(SFML_BUILD_SHARED_LIBS   OFF CACHE BOOL " " FORCE)
set(SFML_USE_STATIC_STD_LIBS OFF CACHE BOOL " " FORCE)
add_subdirectory("${TINYSFML_SOURCE_DIR}/../ext/SFML" "${CMAKE_CURRENT_BINARY_DIR}/sfml_build" EXCLUDE_FROM_ALL)
message(STATUS "SFML found and added to build!")

include_directories(include
                    ../ext/SFML/include
                    ../tinyeng/include
                    ../RIKU/include)

add_subdirectory(src)
if(TINY_SFML_GFX AND TINY_SFML_SFX AND TINY_SFML_SYS)
    add_subdirectory(test)
endif()
