#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libmemcached::libhashkit" for configuration "Release"
set_property(TARGET libmemcached::libhashkit APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libmemcached::libhashkit PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/hashkit.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/hashkit.dll"
  )

list(APPEND _cmake_import_check_targets libmemcached::libhashkit )
list(APPEND _cmake_import_check_files_for_libmemcached::libhashkit "${_IMPORT_PREFIX}/lib/hashkit.lib" "${_IMPORT_PREFIX}/bin/hashkit.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
