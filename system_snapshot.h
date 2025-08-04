#ifndef SYSTEM_SNAPSHOT_H
#define SYSTEM_SNAPSHOT_H

#include "song.h"
#include "playlist_engine.h"
#include "playback_history.h"
#include "song_rating_tree.h"
#include "song_lookup.h"
#include <vector>
#include <map>

// Structure to hold system statistics
struct SystemStats {
    std::vector<Song> topLongestSongs;
    std::vector<Song> recentlyPlayed;
    std::map<int, int> songCountByRating;
    int totalSongsInPlaylist;
    int totalSongsInDatabase;
    int totalPlayedSongs;
};

class SystemSnapshot {
public:
    // Constructor
    SystemSnapshot() = default;
    
    // Core method to export system snapshot
    SystemStats exportSnapshot(const PlaylistEngine& engine, 
                              const PlaybackHistory& history,
                              const SongRatingTree& ratingTree,
                              const SongLookup& lookup);
    
    // Individual statistics methods
    std::vector<Song> getTopLongestSongs(const std::vector<Song>& songs, int count = 5);
    std::vector<Song> getRecentlyPlayedSongs(const PlaybackHistory& history, int count = 5);
    std::map<int, int> getSongCountByRating(const SongRatingTree& ratingTree);
    
    // Utility methods
    void displaySystemStats(const SystemStats& stats);
    
    // Time complexity annotations:
    // exportSnapshot: O(n log n) - dominated by sorting operations
    // getTopLongestSongs: O(n log n) - sorting by duration
    // getRecentlyPlayedSongs: O(n) - linear traversal
    // getSongCountByRating: O(n) - tree traversal
};

#endif // SYSTEM_SNAPSHOT_H 