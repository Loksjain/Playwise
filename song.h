#ifndef SONG_H
#define SONG_H

#include <string>
#include <chrono>

class Song {
public:
    std::string title;
    std::string artist;
    int duration; // in seconds
    int id;
    std::chrono::system_clock::time_point addedTime;
    
    // Constructor
    Song(const std::string& title = "", const std::string& artist = "", int duration = 0);
    
    // Copy constructor
    Song(const Song& other);
    
    // Assignment operator
    Song& operator=(const Song& other);
    
    // Comparison operators for sorting
    bool operator<(const Song& other) const;
    bool operator==(const Song& other) const;
    
    // Utility methods
    std::string getFormattedDuration() const;
    std::string toString() const;
};

#endif // SONG_H 