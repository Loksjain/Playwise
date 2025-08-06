cmake_minimum_required(VERSION 3.16)
project(PlayWise VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt packages
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Charts)
if (NOT Qt6_FOUND)
    find_package(Qt5 REQUIRED COMPONENTS Core Widgets Charts)
endif()

# Set Qt MOC and AUTOUIC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Source files for core system
set(CORE_SOURCES
    song.cpp
    playlist_engine.cpp
    playback_history.cpp
    song_rating_tree.cpp
    song_lookup.cpp
    playlist_sorter.cpp
    system_snapshot.cpp
)

# GUI source files
set(GUI_SOURCES
    gui_main.cpp
)

# Create GUI executable
add_executable(playwise_gui ${GUI_SOURCES} ${CORE_SOURCES})

# Link Qt libraries
target_link_libraries(playwise_gui
    Qt::Core
    Qt::Widgets
    Qt::Charts
)

# Set properties for GUI
set_target_properties(playwise_gui PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

# Create console version executable
add_executable(playwise_console main.cpp ${CORE_SOURCES})

# Set compiler flags
target_compile_options(playwise_gui PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -O2>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -O2>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /O2>
)

target_compile_options(playwise_console PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -O2>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -O2>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /O2>
)

# Install targets
install(TARGETS playwise_gui playwise_console
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
)

# Copy Qt dependencies on Windows
if(WIN32)
    find_program(WINDEPLOYQT_EXECUTABLE NAMES windeployqt HINTS "${_qt_bin_dir}")
    if(WINDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET playwise_gui POST_BUILD
            COMMAND ${WINDEPLOYQT_EXECUTABLE} --no-compiler-runtime --no-translations --no-system-d3d-compiler $<TARGET_FILE:playwise_gui>
            COMMENT "Deploying Qt dependencies for playwise_gui"
        )
    endif()
endif()

# Print configuration info
message(STATUS "PlayWise Configuration:")
message(STATUS "  - C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  - Qt Version: ${Qt_VERSION}")
message(STATUS "  - Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  - GUI Executable: playwise_gui")
message(STATUS "  - Console Executable: playwise_console") 