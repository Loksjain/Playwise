@echo off
echo ========================================
echo PlayWise GUI Builder
echo ========================================
echo.

REM Check if Qt is installed
echo Checking Qt installation...
where qmake >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: Qt not found in PATH
    echo Please install Qt and add it to your PATH
    echo Download from: https://www.qt.io/download
    pause
    exit /b 1
)

echo Qt found: 
qmake -v
echo.

REM Create build directory
if not exist build mkdir build
cd build

REM Try CMake first
echo Attempting to build with CMake...
cmake .. > cmake_output.txt 2>&1
if %errorlevel% equ 0 (
    echo CMake configuration successful!
    echo Building project...
    cmake --build . > build_output.txt 2>&1
    if %errorlevel% equ 0 (
        echo.
        echo ========================================
        echo BUILD SUCCESSFUL!
        echo ========================================
        echo.
        echo GUI executable: playwise_gui.exe
        echo Console executable: playwise_console.exe
        echo.
        echo To run the GUI:
        echo   playwise_gui.exe
        echo.
        echo To deploy Qt dependencies (for distribution):
        echo   windeployqt playwise_gui.exe
        echo.
    ) else (
        echo CMake build failed. Check build_output.txt for details.
        type build_output.txt
    )
) else (
    echo CMake configuration failed. Check cmake_output.txt for details.
    type cmake_output.txt
    echo.
    echo Trying alternative build method...
    
    REM Try direct compilation with g++
    cd ..
    echo Building with g++...
    g++ -std=c++17 -Wall -Wextra -O2 -I"C:/Qt/6.5.0/mingw_64/include" -I"C:/Qt/6.5.0/mingw_64/include/QtCore" -I"C:/Qt/6.5.0/mingw_64/include/QtWidgets" -I"C:/Qt/6.5.0/mingw_64/include/QtCharts" -o playwise_gui.exe gui_main.cpp song.cpp playlist_engine.cpp playback_history.cpp song_rating_tree.cpp song_lookup.cpp playlist_sorter.cpp system_snapshot.cpp -L"C:/Qt/6.5.0/mingw_64/lib" -lQt6Core -lQt6Widgets -lQt6Charts -lQt6Gui -mwindows > gcc_output.txt 2>&1
    
    if %errorlevel% equ 0 (
        echo.
        echo ========================================
        echo BUILD SUCCESSFUL!
        echo ========================================
        echo.
        echo GUI executable: playwise_gui.exe
        echo.
        echo To run the GUI:
        echo   playwise_gui.exe
        echo.
    ) else (
        echo g++ build failed. Check gcc_output.txt for details.
        type gcc_output.txt
        echo.
        echo ========================================
        echo BUILD FAILED
        echo ========================================
        echo.
        echo Possible solutions:
        echo 1. Install Qt 6.5.0 or later
        echo 2. Update Qt paths in the script
        echo 3. Install MinGW-w64 compiler
        echo 4. Use Qt Creator IDE instead
        echo.
    )
)

echo.
echo Press any key to exit...
pause >nul 