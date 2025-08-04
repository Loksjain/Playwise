#include "playback_history.h"
#include <iostream>
#include <algorithm>

void PlaybackHistory::addPlayedSong(const Song& song) {
    historyStack.push(song);
    historyVector.push_back(song);
}

Song PlaybackHistory::undoLastPlay() {
    if (historyStack.empty()) {
        return Song(); // Return empty song
    }
    
    Song lastSong = historyStack.top();
    historyStack.pop();
    
    // Remove from vector as well
    if (!historyVector.empty()) {
        historyVector.pop_back();
    }
    
    return lastSong;
}

void PlaybackHistory::displayHistory() const {
    if (historyVector.empty()) {
        std::cout << "No playback history available.\n";
        return;
    }
    
    std::cout << "\n=== Playback History ===\n";
    for (int i = historyVector.size() - 1; i >= 0; i--) {
        std::cout << historyVector.size() - i << ". " << historyVector[i].title 
                  << " by " << historyVector[i].artist << " (" 
                  << historyVector[i].getFormattedDuration() << ")\n";
    }
}

std::vector<Song> PlaybackHistory::getRecentlyPlayed(int count) const {
    std::vector<Song> recent;
    int actualCount = std::min(count, static_cast<int>(historyVector.size()));
    
    for (int i = static_cast<int>(historyVector.size()) - 1; i >= 0 && recent.size() < static_cast<size_t>(actualCount); i--) {
        recent.push_back(historyVector[i]);
    }
    
    return recent;
} 