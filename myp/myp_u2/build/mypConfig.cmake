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
