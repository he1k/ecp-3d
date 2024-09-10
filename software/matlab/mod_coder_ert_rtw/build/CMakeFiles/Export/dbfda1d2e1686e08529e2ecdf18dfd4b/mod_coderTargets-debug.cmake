#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "mod_coder::mod_coder_objects" for configuration "Debug"
set_property(TARGET mod_coder::mod_coder_objects APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(mod_coder::mod_coder_objects PROPERTIES
  IMPORTED_COMMON_LANGUAGE_RUNTIME_DEBUG ""
  IMPORTED_OBJECTS_DEBUG "${_IMPORT_PREFIX}/mod_coder_ert_rtw/objects-Debug/mod_coder_objects/mod_coder.c.obj"
  )

list(APPEND _cmake_import_check_targets mod_coder::mod_coder_objects )
list(APPEND _cmake_import_check_files_for_mod_coder::mod_coder_objects "${_IMPORT_PREFIX}/mod_coder_ert_rtw/objects-Debug/mod_coder_objects/mod_coder.c.obj" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
