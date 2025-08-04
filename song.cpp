#include "song.h"
#include <sstream>
#include <iomanip>

// Static counter for generating unique IDs
static int nextSongId = 1;

Song::Song(const std::string& title, const std::string& artist, int duration) 
    : title(title), artist(artist), duration(duration), id(nextSongId++), 
      addedTime(std::chrono::system_clock::now()) {
}

Song::Song(const Song& other) 
    : title(other.title), artist(other.artist), duration(other.duration), 
      id(other.id), addedTime(other.addedTime) {
}

Song& Song::operator=(const Song& other) {
    if (this != &other) {
        title = other.title;
        artist = other.artist;
        duration = other.duration;
        id = other.id;
        addedTime = other.addedTime;
    }
    return *this;
}

bool Song::operator<(const Song& other) const {
    return title < other.title;
}

bool Song::operator==(const Song& other) const {
    return id == other.id;
}

std::string Song::getFormattedDuration() const {
    int minutes = duration / 60;
    int seconds = duration % 60;
    std::ostringstream oss;
    oss << minutes << ":" << std::setfill('0') << std::setw(2) << seconds;
    return oss.str();
}

std::string Song::toString() const {
    return title + " by " + artist + " (" + getFormattedDuration() + ")";
} 