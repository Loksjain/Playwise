#ifndef PLAYBACK_HISTORY_H
#define PLAYBACK_HISTORY_H

#include "song.h"
#include <stack>
#include <vector>

class PlaybackHistory {
private:
    std::stack<Song> historyStack;
    std::vector<Song> historyVector; // For display purposes
    
public:
    // Constructor
    PlaybackHistory() = default;
    
    // Core operations
    void addPlayedSong(const Song& song);
    Song undoLastPlay();
    
    // Utility methods
    void displayHistory() const;
    std::vector<Song> getRecentlyPlayed(int count = 5) const;
    int getHistorySize() const { return historyStack.size(); }
    
    // Time complexity annotations:
    // addPlayedSong: O(1) - stack push operation
    // undoLastPlay: O(1) - stack pop operation
    // displayHistory: O(n) - needs to traverse all history
    // getRecentlyPlayed: O(n) - needs to copy recent songs
};

#endif // PLAYBACK_HISTORY_H 