#ifndef PLAYLIST_ENGINE_H
#define PLAYLIST_ENGINE_H

#include "song.h"
#include <vector>
#include <stack>
#include <string>

// Node structure for doubly linked list
struct PlaylistNode {
    Song song;
    PlaylistNode* prev;
    PlaylistNode* next;
    
    PlaylistNode(const Song& s) : song(s), prev(nullptr), next(nullptr) {}
};

// Action types for undo functionality
enum class ActionType {
    ADD,
    DELETE,
    MOVE
};

// Action structure for undo system
struct PlaylistAction {
    ActionType type;
    Song song;
    int index1; // For move operations: from index
    int index2; // For move operations: to index
    
    PlaylistAction(ActionType t, const Song& s, int i1 = -1, int i2 = -1) 
        : type(t), song(s), index1(i1), index2(i2) {}
};

class PlaylistEngine {
private:
    PlaylistNode* head;
    PlaylistNode* tail;
    int size;
    std::stack<PlaylistAction> undoStack;
    
    // Helper methods
    PlaylistNode* getNodeAt(int index) const;
    void insertNodeAt(PlaylistNode* node, int index);
    void removeNode(PlaylistNode* node);
    void clearList();
    
public:
    // Constructor and destructor
    PlaylistEngine();
    ~PlaylistEngine();
    
    // Core playlist operations
    void addSong(const std::string& title, const std::string& artist, int duration);
    void deleteSong(int index);
    void moveSong(int fromIndex, int toIndex);
    void reversePlaylist();
    
    // Undo functionality
    void undoLastNEdits(int n);
    
    // Shuffle with constraints
    void shuffleWithConstraints();
    
    // Utility methods
    void displayPlaylist() const;
    std::vector<Song> getSongs() const;
    int getSize() const { return size; }
    
    // Time complexity annotations:
    // addSong: O(1) - adds to end
    // deleteSong: O(n) - needs to traverse to index
    // moveSong: O(n) - needs to traverse to both indices
    // reversePlaylist: O(n) - needs to traverse entire list
    // undoLastNEdits: O(n*m) where n is number of undos, m is average operation cost
    // shuffleWithConstraints: O(n^2) - may need multiple passes
};

#endif // PLAYLIST_ENGINE_H 