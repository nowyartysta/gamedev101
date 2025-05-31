project(na_glad)

add_library(na_glad
    src/glad.c
)
target_include_directories(na_glad
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
