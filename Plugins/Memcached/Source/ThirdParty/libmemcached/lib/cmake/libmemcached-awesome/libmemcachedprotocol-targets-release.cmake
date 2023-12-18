#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libmemcached::libmemcachedprotocol" for configuration "Release"
set_property(TARGET libmemcached::libmemcachedprotocol APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libmemcached::libmemcachedprotocol PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/memcachedprotocol.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/memcachedprotocol.dll"
  )

list(APPEND _cmake_import_check_targets libmemcached::libmemcachedprotocol )
list(APPEND _cmake_import_check_files_for_libmemcached::libmemcachedprotocol "${_IMPORT_PREFIX}/lib/memcachedprotocol.lib" "${_IMPORT_PREFIX}/bin/memcachedprotocol.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
