project(snake)

add_executable(snake main.cpp)
target_compile_features(na_glfwapp PUBLIC cxx_std_26)
target_compile_options(na_glfwapp PUBLIC -fno-rtti -fno-exceptions -Wall -Wextra -Wpedantic -Werror)
target_link_libraries(snake PUBLIC
    na_error
    na_gl
    na_gl_render_common
    na_glad
    na_glfwapp
)
