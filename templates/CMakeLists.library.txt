project(LIBRARY_NAME)

add_library(LIBRARY_NAME)
target_compile_features(LIBRARY_NAME PUBLIC cxx_std_26)
target_compile_options(LIBRARY_NAME PUBLIC -fno-rtti -fno-exceptions -Wall -Wextra -Wpedantic -Werror)
target_sources(LIBRARY_NAME PUBLIC
    FILE_SET CXX_MODULES FILES
    # files go here
)
target_link_libraries(LIBRARY_NAME PUBLIC
    na_error
)