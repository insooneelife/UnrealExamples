#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libmemcached::p9y" for configuration "Release"
set_property(TARGET libmemcached::p9y APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libmemcached::p9y PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C;CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/p9y.lib"
  )

list(APPEND _cmake_import_check_targets libmemcached::p9y )
list(APPEND _cmake_import_check_files_for_libmemcached::p9y "${_IMPORT_PREFIX}/lib/p9y.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
