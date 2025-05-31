project(na_glfwapp)

add_library(na_glfwapp)
target_compile_features(na_glfwapp PUBLIC cxx_std_26)
target_compile_options(na_glfwapp PUBLIC -fno-rtti -fno-exceptions -Wall -Wextra -Wpedantic -Werror)
target_sources(na_glfwapp PUBLIC
    FILE_SET CXX_MODULES FILES
        application.cpp
        na_glfwapp.cpp
        runner.cpp
)
target_link_libraries(na_glfwapp PUBLIC
    glfw    
    na_error
    na_glad
)