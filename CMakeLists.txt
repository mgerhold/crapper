cmake_minimum_required(VERSION 3.28)
project(crapper)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(${PROJECT_SOURCE_DIR}/project_options.cmake)
include(${PROJECT_SOURCE_DIR}/dependencies.cmake)

crapper_setup_dependencies()

add_subdirectory(src bin)

if (${crapper_build_examples})
    add_subdirectory(examples)
endif ()

if (${crapper_build_tests})
    enable_testing()
    add_subdirectory(test)
endif ()
