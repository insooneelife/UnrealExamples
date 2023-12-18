#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libmemcached::libmemcached" for configuration "Release"
set_property(TARGET libmemcached::libmemcached APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libmemcached::libmemcached PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/memcached.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/memcached.dll"
  )

list(APPEND _cmake_import_check_targets libmemcached::libmemcached )
list(APPEND _cmake_import_check_files_for_libmemcached::libmemcached "${_IMPORT_PREFIX}/lib/memcached.lib" "${_IMPORT_PREFIX}/bin/memcached.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
