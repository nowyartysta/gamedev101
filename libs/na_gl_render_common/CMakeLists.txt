project(na_gl_render_common)

add_library(na_gl_render_common)
target_compile_features(na_gl_render_common PUBLIC cxx_std_26)
target_compile_options(na_gl_render_common PUBLIC -fno-rtti -fno-exceptions -Wall -Wextra -Wpedantic -Werror)
target_sources(na_gl_render_common PUBLIC
    FILE_SET CXX_MODULES FILES
    na_gl_render_common.cpp
    shader.cpp
)
target_link_libraries(na_gl_render_common PUBLIC
    na_error
    na_gl
    na_glad
)