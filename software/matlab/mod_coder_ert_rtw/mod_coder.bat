set CMAKE_EXPORT_COMPILE_COMMANDS=TRUE
set PATH=$(MINGW64_INSTALL_DIR)\bin;%PATH%
C:\PROGRA~1\MATLAB\R2023b\bin\win64\cmake\bin\cmake.exe -S . -B build -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Debug  -DCMAKE_PLATFORM_INFO_INITIALIZED:INTERNAL=1 --no-warn-unused-cli -DCMAKE_INSTALL_PREFIX=".."
@if errorlevel 1 (
    @echo The cmake command returned an error of %errorlevel% 1>&2
    @exit /B 1
)

C:\PROGRA~1\MATLAB\R2023b\bin\win64\cmake\bin\cmake.exe --build build
@if errorlevel 1 (
    @echo The cmake command returned an error of %errorlevel% 1>&2
    @exit /B 1
)

C:\PROGRA~1\MATLAB\R2023b\bin\win64\cmake\bin\cmake.exe --install build --prefix ..
@if errorlevel 1 (
    @echo The cmake command returned an error of %errorlevel% 1>&2
    @exit /B 1
)

