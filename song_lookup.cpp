#include "song_lookup.h"
#include <iostream>

void SongLookup::addSong(const Song& song) {
    titleToSong[song.title] = song;
    idToSong[song.id] = song;
}

Song* SongLookup::searchByTitle(const std::string& title) {
    auto it = titleToSong.find(title);
    if (it != titleToSong.end()) {
        return &(it->second);
    }
    return nullptr;
}

Song* SongLookup::searchById(int id) {
    auto it = idToSong.find(id);
    if (it != idToSong.end()) {
        return &(it->second);
    }
    return nullptr;
}

void SongLookup::deleteSong(const std::string& title) {
    auto it = titleToSong.find(title);
    if (it != titleToSong.end()) {
        int id = it->second.id;
        titleToSong.erase(it);
        idToSong.erase(id);
    }
}

void SongLookup::displayAllSongs() const {
    if (titleToSong.empty()) {
        std::cout << "No songs in lookup database.\n";
        return;
    }
    
    std::cout << "\n=== All Songs in Database ===\n";
    for (const auto& pair : titleToSong) {
        const Song& song = pair.second;
        std::cout << "ID: " << song.id << " | " << song.title << " by " << song.artist 
                  << " (" << song.getFormattedDuration() << ")\n";
    }
}

std::vector<Song> SongLookup::getAllSongs() const {
    std::vector<Song> songs;
    songs.reserve(titleToSong.size());
    
    for (const auto& pair : titleToSong) {
        songs.push_back(pair.second);
    }
    
    return songs;
} 