include("${CMAKE_CURRENT_LIST_DIR}/mod_coderTargets.cmake")
set(MOD_CODER_OBJECTS_TOK_INC_DIRS_ALL
    ${MATLAB_ROOT}/extern/include
    ${MATLAB_ROOT}/simulink/include
    ${MATLAB_ROOT}/rtw/c/src
    ${MATLAB_ROOT}/rtw/c/src/ext_mode/common
    ${MATLAB_ROOT}/rtw/c/ert)
foreach(TOKDIR_LOOP IN LISTS MOD_CODER_OBJECTS_TOK_INC_DIRS_ALL)
    if(IS_DIRECTORY ${TOKDIR_LOOP})
        list(APPEND MOD_CODER_OBJECTS_TOK_INC_DIRS ${TOKDIR_LOOP})
    endif()
endforeach()
target_include_directories(mod_coder::mod_coder_objects INTERFACE ${MOD_CODER_OBJECTS_TOK_INC_DIRS})
