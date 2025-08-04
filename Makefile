CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = playwise
SOURCES = main.cpp song.cpp playlist_engine.cpp playback_history.cpp song_rating_tree.cpp song_lookup.cpp playlist_sorter.cpp system_snapshot.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

# Compile source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJECTS) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Install dependencies (for Ubuntu/Debian)
install-deps:
	sudo apt-get update
	sudo apt-get install -y build-essential

# Install dependencies (for macOS)
install-deps-mac:
	brew install gcc

# Install dependencies (for Windows with MinGW)
install-deps-windows:
	# Assuming MinGW is already installed
	# If not, download from: https://www.mingw-w64.org/

.PHONY: all clean run install-deps install-deps-mac install-deps-windows 