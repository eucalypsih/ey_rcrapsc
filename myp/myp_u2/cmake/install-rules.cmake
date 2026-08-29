# (Di dalam file cmake/install-rules.cmake)

if(NOT DEFINED CMAKE_INSTALL_BINDIR)
  include(GNUInstallDirs)
endif()

# ==============================================================================
# 1. Ekspor Nama Target (Export Set)
# ==============================================================================
# Kita mengelompokkan target proyek ke dalam satu set ekspor bernama "mypTargets"
set(export_name "mypTargets")

# ==============================================================================
# 2. Aturan Instalasi Target (Executable & Library)
# ==============================================================================
install(
    TARGETS myp_exe myp_lib
    EXPORT ${export_name}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    # Khusus untuk OBJECT library, CMake 3.14+ membutuhkan ini agar objeknya ikut terinstal jika diperlukan
    # OBJECTS DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# ==============================================================================
# 3. Aturan Instalasi File Header (.hpp / .h)
# ==============================================================================
# Asumsi: Anda meletakkan file header publik di dalam folder 'source/'
install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/source/"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/myp
    FILES_MATCHING
    PATTERN "*.hpp"
    PATTERN "*.h"
)

# ==============================================================================
# 4. Pembuatan File Konfigurasi CMake untuk Proyek Lain (find_package)
# ==============================================================================
# Folder tempat menyimpan file konfigurasi CMake hasil instalasi
set(cmake_config_dest "${CMAKE_INSTALL_LIBDIR}/cmake/myp")

# Menghasilkan file target (mypTargets.cmake) yang mendaftarkan target 'myp::exe' dsb.
install(
    EXPORT ${export_name}
    NAMESPACE myp::
    DESTINATION ${cmake_config_dest}
    # EXPORT_FIND_PACKAGES # CMake Version 3.30+
)

# Membuat file versi paket (mypConfigVersion.cmake) untuk memeriksa kecocokan versi
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/mypConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Menginstal file versi tersebut
install(
    FILES "${PROJECT_BINARY_DIR}/mypConfigVersion.cmake"
    DESTINATION ${cmake_config_dest}
)

# (Opsional) Jika Anda butuh file mypConfig.cmake kustom, Anda bisa membuatnya di folder cmake/
# Jika tidak ada logika rumit, cukup instal file minimal seperti di bawah ini:
# Salin isi dari mypConfig.cmake yang dibuat otomatis agar menyertakan fmt
file(WRITE "${PROJECT_BINARY_DIR}/mypConfig.cmake"
[[
include(CMakeFindDependencyMacro)
# Jika proyek lain membutuhkan fmt saat menggunakan myp, sertakan dependency di sini:
# find_dependency(fmt REQUIRED) 

# Kunci Solusi: Memaksa proyek luar untuk ikut mencari 'fmt' saat mereka memanggil find_package(myp)
# if(NOT TARGET fmt::fmt)
#     find_dependency(fmt REQUIRED)
# endif()

# Memastikan file target fmt yang ikut terinstal dapat ditemukan oleh proyek luar
# if(NOT TARGET fmt::fmt)
#    include("${CMAKE_CURRENT_LIST_DIR}/fmt-targets.cmake" OPTIONAL)
# endif()

# Mengupdate isi file mypConfig.cmake agar memuat dependensi otomatis hasil dari EXPORT_FIND_PACKAGES
# Mengikutkan file dependensi otomatis yang digenerate oleh EXPORT_FIND_PACKAGES
# if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/mypTargets-helpers.cmake")
#     include("${CMAKE_CURRENT_LIST_DIR}/mypTargets-helpers.cmake")
# endif()


include("${CMAKE_CURRENT_LIST_DIR}/mypTargets.cmake")
]]
)

install(
    FILES "${PROJECT_BINARY_DIR}/mypConfig.cmake"
    DESTINATION ${cmake_config_dest}
)
