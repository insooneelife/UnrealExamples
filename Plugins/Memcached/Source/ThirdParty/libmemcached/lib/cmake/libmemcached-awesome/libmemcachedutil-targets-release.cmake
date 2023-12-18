#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libmemcached::libmemcachedutil" for configuration "Release"
set_property(TARGET libmemcached::libmemcachedutil APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libmemcached::libmemcachedutil PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/memcachedutil.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/memcachedutil.dll"
  )

list(APPEND _cmake_import_check_targets libmemcached::libmemcachedutil )
list(APPEND _cmake_import_check_files_for_libmemcached::libmemcachedutil "${_IMPORT_PREFIX}/lib/memcachedutil.lib" "${_IMPORT_PREFIX}/bin/memcachedutil.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
