project(na_error)

add_library(na_error)
target_compile_features(na_error PUBLIC cxx_std_26)
target_compile_options(na_error PUBLIC -fno-rtti -fno-exceptions -Wall -Wextra -Wpedantic -Werror)
target_sources(na_error PUBLIC FILE_SET CXX_MODULES FILES error.cpp result.cpp na_error.cpp FILE_SET HEADERS FILES macros.hpp)