cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    myp
    VERSION 0.1.0
    DESCRIPTION "My Simple Program"
    HOMEPAGE_URL "https://exam.co"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    myp_lib OBJECT
    source/lib.cpp
)

target_include_directories(
    myp_lib ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(myp_lib PUBLIC cxx_std_20)

# find_package(fmt REQUIRED)
include(FetchContent)
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        12.2.0 # 10.2.1 # Atau versi stabil lainnya
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(fmt)

target_link_libraries(myp_lib PRIVATE fmt::fmt)

# ---- Declare executable ----

add_executable(myp_exe source/main.cpp)
add_executable(myp::exe ALIAS myp_exe)

set_property(TARGET myp_exe PROPERTY OUTPUT_NAME myp)

target_compile_features(myp_exe PRIVATE cxx_std_20)

target_link_libraries(myp_exe PRIVATE myp_lib)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT myp_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of myp"
  )
endif()

include(cmake/dev-mode.cmake)
