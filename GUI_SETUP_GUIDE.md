# PlayWise GUI Setup Guide
## Building the Graphical User Interface

---

## Overview

The PlayWise GUI provides a modern, user-friendly interface for the music playlist management system. It features:

- **Tabbed Interface**: Organized sections for each major feature
- **Real-time Updates**: Live display of playlist changes and statistics
- **Interactive Charts**: Visual analytics using Qt Charts
- **Modern Styling**: Professional appearance with custom CSS
- **Cross-platform**: Works on Windows, macOS, and Linux

---

## Prerequisites

### Required Software

1. **Qt Framework** (Version 5.15+ or 6.0+)
   - Download from: https://www.qt.io/download
   - Choose the open-source version
   - Install Qt Creator (IDE) and Qt libraries

2. **C++ Compiler**
   - **Windows**: MinGW-w64 or MSVC
   - **macOS**: Xcode Command Line Tools
   - **Linux**: GCC or Clang

3. **CMake** (Version 3.16+)
   - Download from: https://cmake.org/download/

### Qt Installation Options

#### Option 1: Qt Online Installer (Recommended)
1. Download Qt Online Installer
2. Create a Qt account (free)
3. Select Qt 6.5.0 or Qt 5.15.2
4. Choose MinGW 64-bit compiler
5. Install Qt Creator and Qt libraries

#### Option 2: Package Manager (Linux)
```bash
# Ubuntu/Debian
sudo apt-get install qt6-base-dev qt6-charts-dev

# Fedora
sudo dnf install qt6-qtbase-devel qt6-qtcharts-devel

# Arch Linux
sudo pacman -S qt6-base qt6-charts
```

---

## Building the GUI

### Method 1: Using CMake (Recommended)

1. **Create build directory**:
   ```bash
   mkdir build
   cd build
   ```

2. **Configure with CMake**:
   ```bash
   cmake ..
   ```

3. **Build the project**:
   ```bash
   cmake --build .
   ```

4. **Run the GUI**:
   ```bash
   ./playwise_gui
   ```

### Method 2: Using Makefile

1. **Edit Qt paths** in `Makefile_GUI`:
   ```makefile
   # Update these paths to match your Qt installation
   QT_INCLUDE = -I"C:/Qt/6.5.0/mingw_64/include" ...
   QT_LIBS = -L"C:/Qt/6.5.0/mingw_64/lib" ...
   ```

2. **Build using Makefile**:
   ```bash
   make -f Makefile_GUI playwise_gui.exe
   ```

3. **Run the GUI**:
   ```bash
   ./playwise_gui.exe
   ```

### Method 3: Using Qt Creator

1. **Open Qt Creator**
2. **Open Project**: Select `CMakeLists.txt`
3. **Configure Project**: Choose your kit (compiler + Qt version)
4. **Build Project**: Press Ctrl+B or click Build button
5. **Run Project**: Press Ctrl+R or click Run button

---

## GUI Features

### 1. Playlist Management Tab
- **Current Playlist Display**: Shows all songs with title, artist, and duration
- **Play Controls**: Play selected song, move songs up/down
- **Playlist Operations**: Reverse, shuffle with constraints
- **Add/Delete Songs**: Input form for new songs, delete selected songs

### 2. Playback History Tab
- **Recent Plays**: List of recently played songs
- **Undo Operations**: Undo last play, clear history
- **History Management**: Track and manage playback history

### 3. Song Ratings Tab
- **Rating Tree Display**: Visual representation of rating distribution
- **Add Ratings**: Rate selected songs (1-5 stars)
- **Search by Rating**: Find songs with specific ratings
- **Rating Statistics**: View song counts by rating

### 4. Song Lookup Tab
- **Search Interface**: Search by title or ID
- **Results Display**: Show search results
- **Quick Access**: Fast O(1) lookup using hash maps

### 5. Playlist Sorting Tab
- **Sort Options**: By title, duration, or recently added
- **Sorted Display**: View sorted playlist
- **Performance Stats**: Timing information for sorting operations

### 6. Undo Operations Tab
- **Undo Controls**: Specify number of edits to undo
- **Undo History**: Track undo operations
- **Command Pattern**: Implemented using stack-based undo system

### 7. System Snapshot Tab
- **Live Analytics**: Real-time system statistics
- **Visual Charts**: Pie chart for ratings, bar chart for durations
- **Export Functionality**: Save snapshots to files
- **System Overview**: Comprehensive system status

---

## GUI Architecture

### Core Components

```cpp
class PlayWiseGUI : public QMainWindow {
    // Core system components
    PlaylistEngine* playlistEngine;
    PlaybackHistory* playbackHistory;
    SongRatingTree* ratingTree;
    SongLookup* songLookup;
    PlaylistSorter* sorter;
    SystemSnapshot* snapshot;
    
    // GUI components
    QTabWidget* mainTabWidget;
    // ... other UI elements
};
```

### Key Design Patterns

1. **Model-View-Controller (MVC)**:
   - Model: Core C++ classes (PlaylistEngine, etc.)
   - View: Qt widgets and UI components
   - Controller: GUI event handlers and slots

2. **Observer Pattern**:
   - Real-time updates using QTimer
   - Automatic UI refresh when data changes

3. **Signal-Slot Mechanism**:
   - Qt's event-driven architecture
   - Loose coupling between components

---

## Troubleshooting

### Common Build Issues

#### 1. Qt Not Found
```
CMake Error: Could not find Qt6
```
**Solution**: 
- Ensure Qt is properly installed
- Set `CMAKE_PREFIX_PATH` to Qt installation directory
- Use `find_package(Qt5)` if Qt6 is not available

#### 2. Missing Qt Libraries
```
undefined reference to `QApplication::QApplication(int&, char**)'
```
**Solution**:
- Check Qt library paths in Makefile
- Ensure all required Qt modules are linked
- Verify Qt installation includes development files

#### 3. MOC Compilation Errors
```
moc: command not found
```
**Solution**:
- Install Qt development tools
- Ensure Qt bin directory is in PATH
- Use `CMAKE_AUTOMOC ON` in CMake

#### 4. Windows Deployment Issues
```
The application was unable to start correctly
```
**Solution**:
- Run `windeployqt playwise_gui.exe` to copy Qt DLLs
- Ensure all Qt dependencies are in the same directory
- Check for missing Visual C++ redistributables

### Platform-Specific Notes

#### Windows
- Use MinGW-w64 or MSVC compiler
- Install Visual Studio Build Tools if using MSVC
- Run `windeployqt` for distribution

#### macOS
- Install Xcode Command Line Tools
- Use Qt Creator for easiest development
- Create `.app` bundle for distribution

#### Linux
- Install Qt development packages
- Use package manager for dependencies
- Consider AppImage for distribution

---

## Customization

### Styling
The GUI uses custom CSS styling defined in `setupStyling()`:

```cpp
setStyleSheet(R"(
    QMainWindow {
        background-color: #f0f0f0;
    }
    QPushButton {
        background-color: #0078d4;
        color: white;
        border-radius: 4px;
    }
    // ... more styles
)");
```

### Adding New Features
1. **Create new tab**: Add to `mainTabWidget`
2. **Add new functionality**: Create new slot methods
3. **Update display**: Modify `updateDisplay()` method
4. **Connect signals**: Use `connect()` for event handling

### Extending Charts
The GUI includes Qt Charts for analytics:

```cpp
void updateCharts(const SystemStats& stats) {
    // Create pie chart for ratings
    QPieSeries* ratingSeries = new QPieSeries();
    // Create bar chart for durations
    QBarSeries* durationSeries = new QBarSeries();
    // ... chart configuration
}
```

---

## Performance Considerations

### GUI Responsiveness
- **Timer-based updates**: Refresh every second, not on every change
- **Efficient data structures**: O(1) lookups for large playlists
- **Lazy loading**: Load data only when needed

### Memory Management
- **RAII principles**: Automatic cleanup of Qt objects
- **Smart pointers**: Consider using `std::unique_ptr` for core components
- **Efficient updates**: Minimize unnecessary UI redraws

### Scalability
- **Large playlists**: GUI handles thousands of songs efficiently
- **Real-time updates**: Minimal performance impact
- **Memory usage**: Linear scaling with playlist size

---

## Distribution

### Windows
1. Build release version
2. Run `windeployqt playwise_gui.exe`
3. Create installer using NSIS or Inno Setup
4. Include Visual C++ redistributables

### macOS
1. Build release version
2. Create `.app` bundle
3. Code sign for distribution
4. Create DMG installer

### Linux
1. Build release version
2. Create AppImage or Snap package
3. Package with dependencies
4. Distribute through package managers

---

## Future Enhancements

### Planned Features
- **Audio playback**: Integrate with audio libraries
- **Database integration**: Persistent storage
- **Network features**: Cloud synchronization
- **Plugin system**: Extensible architecture
- **Dark mode**: Theme switching
- **Keyboard shortcuts**: Enhanced usability

### Technical Improvements
- **Multi-threading**: Background operations
- **Caching**: Improved performance
- **Internationalization**: Multi-language support
- **Accessibility**: Screen reader support

---

## Support

### Getting Help
- Check Qt documentation: https://doc.qt.io/
- Review CMake documentation: https://cmake.org/documentation/
- Search Qt forums: https://forum.qt.io/

### Reporting Issues
- Include system information
- Provide build logs
- Describe steps to reproduce
- Attach error messages

---

## Conclusion

The PlayWise GUI provides a professional, user-friendly interface for the music playlist management system. It demonstrates advanced Qt programming techniques while maintaining the performance and functionality of the core C++ system.

The modular design allows for easy extension and customization, making it suitable for both educational purposes and real-world applications. 