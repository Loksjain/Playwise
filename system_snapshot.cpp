#include "system_snapshot.h"
#include <algorithm>
#include <iostream>

SystemStats SystemSnapshot::exportSnapshot(const PlaylistEngine& engine, 
                                          const PlaybackHistory& history,
                                          const SongRatingTree& ratingTree,
                                          const SongLookup& lookup) {
    SystemStats stats;
    
    // Get songs from playlist
    std::vector<Song> playlistSongs = engine.getSongs();
    
    // Get top 5 longest songs
    stats.topLongestSongs = getTopLongestSongs(playlistSongs, 5);
    
    // Get recently played songs
    stats.recentlyPlayed = getRecentlyPlayedSongs(history, 5);
    
    // Get song count by rating
    stats.songCountByRating = getSongCountByRating(ratingTree);
    
    // Get total counts
    stats.totalSongsInPlaylist = engine.getSize();
    stats.totalSongsInDatabase = lookup.getSongCount();
    stats.totalPlayedSongs = history.getHistorySize();
    
    return stats;
}

std::vector<Song> SystemSnapshot::getTopLongestSongs(const std::vector<Song>& songs, int count) {
    std::vector<Song> sortedSongs = songs;
    
    // Sort by duration in descending order
    std::sort(sortedSongs.begin(), sortedSongs.end(), 
              [](const Song& a, const Song& b) {
                  return a.duration > b.duration;
              });
    
    // Return top 'count' songs
    int actualCount = std::min(count, static_cast<int>(sortedSongs.size()));
    return std::vector<Song>(sortedSongs.begin(), sortedSongs.begin() + actualCount);
}

std::vector<Song> SystemSnapshot::getRecentlyPlayedSongs(const PlaybackHistory& history, int count) {
    return history.getRecentlyPlayed(count);
}

std::map<int, int> SystemSnapshot::getSongCountByRating(const SongRatingTree& ratingTree) {
    std::map<int, int> ratingCounts;
    auto ratingPairs = ratingTree.getSongCountByRating();
    
    for (const auto& pair : ratingPairs) {
        ratingCounts[pair.first] = pair.second;
    }
    
    return ratingCounts;
}

void SystemSnapshot::displaySystemStats(const SystemStats& stats) {
    std::cout << "\n=== System Statistics ===\n";
    std::cout << "Total songs in playlist: " << stats.totalSongsInPlaylist << "\n";
    std::cout << "Total songs in database: " << stats.totalSongsInDatabase << "\n";
    std::cout << "Total played songs: " << stats.totalPlayedSongs << "\n";
    
    std::cout << "\nTop 5 longest songs:\n";
    for (size_t i = 0; i < stats.topLongestSongs.size(); i++) {
        const Song& song = stats.topLongestSongs[i];
        std::cout << i + 1 << ". " << song.title << " by " << song.artist 
                  << " (" << song.getFormattedDuration() << ")\n";
    }
    
    std::cout << "\nMost recently played songs:\n";
    for (size_t i = 0; i < stats.recentlyPlayed.size(); i++) {
        const Song& song = stats.recentlyPlayed[i];
        std::cout << i + 1 << ". " << song.title << " by " << song.artist << "\n";
    }
    
    std::cout << "\nSong count by rating:\n";
    for (const auto& pair : stats.songCountByRating) {
        std::cout << "Rating " << pair.first << ": " << pair.second << " songs\n";
    }
} 