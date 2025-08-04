#ifndef SONG_LOOKUP_H
#define SONG_LOOKUP_H

#include "song.h"
#include <unordered_map>
#include <string>
#include <vector>

class SongLookup {
private:
    std::unordered_map<std::string, Song> titleToSong;
    std::unordered_map<int, Song> idToSong;
    
public:
    // Constructor
    SongLookup() = default;
    
    // Core operations
    void addSong(const Song& song);
    Song* searchByTitle(const std::string& title);
    Song* searchById(int id);
    void deleteSong(const std::string& title);
    
    // Utility methods
    void displayAllSongs() const;
    std::vector<Song> getAllSongs() const;
    int getSongCount() const { return titleToSong.size(); }
    
    // Time complexity annotations:
    // addSong: O(1) - hash map insertion
    // searchByTitle: O(1) - hash map lookup
    // searchById: O(1) - hash map lookup
    // deleteSong: O(1) - hash map deletion
    // displayAllSongs: O(n) - needs to traverse all songs
    // getAllSongs: O(n) - needs to copy all songs
};

#endif // SONG_LOOKUP_H 