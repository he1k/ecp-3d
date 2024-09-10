# Install script for directory: C:/Users/farfar/Documents/Git/ecp-3d/software/matlab/mod_coder_ert_rtw

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/farfar/Documents/Git/ecp-3d/software/matlab")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/mod_coder_ert_rtw/objects-Debug/mod_coder_objects" TYPE FILE FILES "mod_coder.c.obj" FILES_FROM_DIR "C:/Users/farfar/Documents/Git/ecp-3d/software/matlab/mod_coder_ert_rtw/build/CMakeFiles/mod_coder_objects.dir/")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE EXECUTABLE OPTIONAL FILES "C:/Users/farfar/Documents/Git/ecp-3d/software/matlab/mod_coder_ert_rtw/build/mod_coder.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/mod_coder_ert_rtw/export" TYPE FILE FILES "C:/Users/farfar/Documents/Git/ecp-3d/software/matlab/mod_coder_ert_rtw/build/mod_coder.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/mod_coder_ert_rtw/export/mod_coderTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/mod_coder_ert_rtw/export/mod_coderTargets.cmake"
         "C:/Users/farfar/Documents/Git/ecp-3d/software/matlab/mod_coder_ert_rtw/build/CMakeFiles/Export/dbfda1d2e1686e08529e2ecdf18dfd4b/mod_coderTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/mod_coder_ert_rtw/export/mod_coderTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/mod_coder_ert_rtw/export/mod_coderTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/mod_coder_ert_rtw/export" TYPE FILE FILES "C:/Users/farfar/Documents/Git/ecp-3d/software/matlab/mod_coder_ert_rtw/build/CMakeFiles/Export/dbfda1d2e1686e08529e2ecdf18dfd4b/mod_coderTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/mod_coder_ert_rtw/export" TYPE FILE FILES "C:/Users/farfar/Documents/Git/ecp-3d/software/matlab/mod_coder_ert_rtw/build/CMakeFiles/Export/dbfda1d2e1686e08529e2ecdf18dfd4b/mod_coderTargets-debug.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/Users/farfar/Documents/Git/ecp-3d/software/matlab/mod_coder_ert_rtw/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
