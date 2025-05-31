project(na_gl)

add_library(na_gl)
target_compile_features(na_gl PUBLIC cxx_std_26)
target_compile_options(na_gl PUBLIC -fno-rtti -fno-exceptions -Wall -Wextra -Wpedantic -Werror)
target_sources(na_gl PUBLIC
    FILE_SET CXX_MODULES FILES
    na_gl.cpp
    wrapper.cpp
)
target_link_libraries(na_gl PUBLIC
    na_error
    na_glad
)