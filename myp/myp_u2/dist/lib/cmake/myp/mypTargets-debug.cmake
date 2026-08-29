#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "myp::myp_exe" for configuration "Debug"
set_property(TARGET myp::myp_exe APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(myp::myp_exe PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/myp-app-d"
  )

list(APPEND _cmake_import_check_targets myp::myp_exe )
list(APPEND _cmake_import_check_files_for_myp::myp_exe "${_IMPORT_PREFIX}/bin/myp-app-d" )

# Import target "myp::myp_lib" for configuration "Debug"
set_property(TARGET myp::myp_lib APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(myp::myp_lib PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libmyp_lib.a"
  )

list(APPEND _cmake_import_check_targets myp::myp_lib )
list(APPEND _cmake_import_check_files_for_myp::myp_lib "${_IMPORT_PREFIX}/lib/libmyp_lib.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
