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


# Tentukan folder cache global (contoh: di home directory pengguna)
if(CMAKE_HOST_WIN32)
    set(FETCHCONTENT_BASE_DIR "$ENV{USERPROFILE}/.cmake_fetchcontent_cache" CACHE PATH "")
else()
    set(FETCHCONTENT_BASE_DIR "$ENV{HOME}/.cmake_fetchcontent_cache" CACHE PATH "")
endif()

# ---- Declare library ----
# ---- Pustaka Pihak Ketiga (Dependencies) ----

# find_package(fmt REQUIRED)
include(FetchContent)

# Menonaktifkan kompilasi unit test internal milik pustaka fmt
# Menolak unit test internal milik fmt agar hemat waktu compile
set(FMT_TEST OFF CACHE BOOL "" FORCE)

# PENTING: Secara default FetchContent mematikan aturan instalasi proyek luar.
# Kita paksa hidupkan agar fmt bisa ikut diinstal menggunakan EXCLUDE_FROM_ALL
# set(FMT_INSTALL ON CACHE BOOL "" FORCE)

# UBAH INI MENJADI OFF: Mencegah file fmt ikut disalin saat 'cmake --install'
set(FMT_INSTALL OFF CACHE BOOL "" FORCE)

# Paksa pustaka fmt untuk dikonfigurasi sebagai header-only
# Di beberapa versi fmt, target header-only sudah langsung tersedia 
# saat di-include menggunakan target `fmt::fmt-header-only`
# set(FMT_MASTER_PROJECT OFF CACHE INTERNAL "")
# set(FMT_INSTALL OFF CACHE INTERNAL "")

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        12.2.0 # 10.2.1 # Atau versi stabil lainnya
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(fmt)



# ---- Declare library ----

add_library(
    myp_lib STATIC
    source/lib.cpp
)

# target_include_directories(
#     myp_lib ${warning_guard}
#     PUBLIC
#     "${PROJECT_SOURCE_DIR}/source" # "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
# )

# SOLUSI ERROR 1: Pisahkan folder include lokal proyek dengan folder include hasil install
target_include_directories(
    myp_lib ${warning_guard}
    PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/myp>
)

target_compile_features(myp_lib PUBLIC cxx_std_20)

# Menghubungkan pustaka fmt ke target library proyek Anda
# Gunakan 'fmt::fmt' untuk versi compiled library (bawaan) 
# atau 'fmt::fmt-header-only' jika ingin mode tanpa kompilasi terpisah.
# target_link_libraries(myp_lib PRIVATE fmt::fmt)

# Hubungkan target Anda dengan target khusus header-only milik fmt
# target_link_libraries(myp_lib PRIVATE fmt::fmt-header-only)

# SOLUSI ERROR 2: Gunakan $<LINK_ONLY:...> agar tautan biner internal 'fmt' 
# tersembunyi sepenuhnya dari mata berkas ekspor 'mypTargets.cmake'
target_link_libraries(myp_lib PRIVATE 
    $<LINK_ONLY:fmt::fmt>
)

# ---- Declare executable ----

add_executable(myp_exe source/main.cpp)
add_executable(myp::exe ALIAS myp_exe)

# Skema nama file dasar hasil kompilasi
set_property(TARGET myp_exe PROPERTY OUTPUT_NAME "myp-app")

# 2. Skema penamaan dinamis berdasarkan konfigurasi Build (Kustomisasi Fitur)
# Menambahkan suffix '-d' jika dicompile dalam mode Debug (contoh hasil: myp-app-d.exe)
set_target_properties(myp_exe PROPERTIES
    DEBUG_POSTFIX "-d"
    RELEASE_POSTFIX ""
    MINSIZEREL_POSTFIX "-min"
)

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
