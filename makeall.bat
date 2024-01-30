@echo off

setlocal



IF "%VS170COMNTOOLS%"=="" set VS170COMNTOOLS=C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\Tools\
IF "%VS160COMNTOOLS%"=="" set VS160COMNTOOLS=C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\Common7\Tools\
IF "%VS150COMNTOOLS%"=="" set VS150COMNTOOLS=C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\Common7\Tools\

set VC17AUX=C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\
set VC16AUX=C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\
set VC15AUX=C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\

IF "%MINGW64%"=="" set MINGW64=C:\msys64\mingw64\bin
IF "%MINGW32%"=="" set MINGW32=C:\msys64\mingw32\bin




if Exist "%MINGW64%\g++.exe" (
Setlocal
echo [34mFound Mingw64 - compiling[0m
set "PATH=%PATH%;%MINGW64%"
g++  -static-libgcc -static-libstdc++ -gdwarf -m64 -o shared-gcc-d-x64.dll   sharedlibrary.cpp -Wl,-subsystem,windows
g++  -static-libgcc -static-libstdc++ -gdwarf -m64 -O2 -o shared-gcc-r-x64.dll   sharedlibrary.cpp -Wl,-subsystem,windows

rem cv2pdb from https://github.com/rainers/cv2pdb
for %%X in (cv2pdb64.exe) do (set FOUND=%%~$PATH:X)
if defined FOUND (
cv2pdb64.exe shared-gcc-d-x64.dll
cv2pdb64.exe shared-gcc-r-x64.dll
)
Endlocal
) Else echo [31mgcc (x64) not found.[0m 


if Exist "%MINGW32%\g++.exe" (
Setlocal
echo [34mFound Mingw32 - compiling[0m
set "PATH=%PATH%;%MINGW32%"
g++ -static  -gdwarf -o shared-gcc-d-x86.dll -fdevirtualize  sharedlibrary.cpp -Wl,-subsystem,windows
g++ -static  -gdwarf -O2 -o shared-gcc-r-x86.dll -fdevirtualize  sharedlibrary.cpp -s -Wl,-subsystem,windows

rem cv2pdb from https://github.com/rainers/cv2pdb
for %%X in (cv2pdb.exe) do (set FOUND=%%~$PATH:X)
if defined FOUND (
cv2pdb.exe shared-gcc-d-x86.dll
cv2pdb.exe shared-gcc-r-x86.dll
)
Endlocal
) Else echo [31mgcc (x86) not found.[0m 


if Exist "%VC17AUX%vcvars64.bat" (
Setlocal
call "%VC17AUX%vcvars64.bat"
echo  [34mFound VC2022 x64 - compiling clang and msvc [0m
csc.exe -t:exe -platform:"x64" -out:host-x86.exe host-x86.cs
csc.exe -t:exe -platform:"anycpu" -out:host-x64.exe host-x64.cs
Endlocal
echo [32mdone.[0m
) Else echo [31mVS2022 not found.[0m


if Exist "%VC17AUX%vcvars64.bat" (
Setlocal
call "%VC17AUX%vcvars64.bat"
echo  [34mFound VC2022 x64 - compiling clang and msvc [0m
clang++ -fms-compatibility-version=19 --shared -m64 -o shared-vs22-clang-x64.dll sharedlibrary.cpp

cl  /nologo /WL /EHsc /Zi /Od sharedlibrary.cpp /MD  /link /DEBUG /DLL /Out:shared-msvc17-mtd-x64.dll
cl  /nologo /WL /EHsc /DEBUG /O2 sharedlibrary.cpp /MDd  /link /DLL /Out:shared-msvc17-mtr-x64.dll

Endlocal
echo [32mdone.[0m
) Else echo [31mVS2022 not found.[0m





if Exist "%VC17AUX%vcvars32.bat" (
Setlocal
call "%VC17AUX%vcvars32.bat"
echo  [34mFound VC2022 x86 - compiling clang and msvc [0m
clang++ -fms-compatibility-version=19 --shared -m32 -o shared-vs22-clang-x86.dll sharedlibrary.cpp
cl  /nologo /WL /EHsc /Zi /Od sharedlibrary.cpp /MD  /link /DEBUG /DLL /Out:shared-msvc17-mtd-x86.dll
cl  /nologo /WL  /EHsc /DEBUG /O2 sharedlibrary.cpp /MDd  /link /DLL /Out:shared-msvc17-mtr-x86.dll

Endlocal
echo [32mdone.[0m
) Else echo [31mVS2022 not found.[0m


if Exist "%VS160COMNTOOLS%\VsDevCmd.bat" (
Setlocal
echo [34mFound VS2019 - compiling[0m
call "%VS160COMNTOOLS%VsDevCmd.bat"
cl   /nologo /WL /EHsc /Od sharedlibrary.cpp /MD  /link /DLL /Out:shared-msvc16-mtr-x64.dll
cl   /nologo /WL /EHsc /DEBUG /O2 sharedlibrary.cpp /MDd  /link /DLL /Out:shared-msvc16-mtd-x64.dll
echo [32mdone.[0m
Endlocal
) Else echo [31mVS2019 not found.[0m

if Exist "%VS150COMNTOOLS%\vsvars32.bat" (
Setlocal
echo [34mFound VS2017 - compiling[0m
call "%VS150COMNTOOLS%vsvars32.bat"
cl  /nologo /WL /EHsc /Od sharedlibrary.cpp /MD  /link /DLL /Out:shared-msvc15-mtr-x64.dll
cl  /nologo /WL /EHsc /DEBUG /O2 sharedlibrary.cpp /MDd  /link /DLL /Out:shared-msvc15-mtd-x64.dll
echo [32mdone.[0m
Endlocal
) Else echo [31mVS2017 not found.[0m


if Exist "%VS140COMNTOOLS%\vsvars32.bat" (
Setlocal
echo [34mFound VS2015 - compiling[0m
call "%VS140COMNTOOLS%vsvars32.bat"
cl  /nologo /WL /EHsc /Od sharedlibrary.cpp /MD  /link /DLL /Out:shared-msvc14-mtr-x86.dll
cl  /nologo /WL /EHsc /DEBUG /O2 sharedlibrary.cpp /MDd  /link /DLL /Out:shared-msvc14-mtd-x86.dll
Endlocal
)



rem xcopy /y /d /i shared-msvc17-mtd-x64.dll host\bin\Debug\net6.0