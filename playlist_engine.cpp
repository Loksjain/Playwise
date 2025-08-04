#include "playlist_engine.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <unordered_map>

PlaylistEngine::PlaylistEngine() : head(nullptr), tail(nullptr), size(0) {
}

PlaylistEngine::~PlaylistEngine() {
    clearList();
}

void PlaylistEngine::clearList() {
    PlaylistNode* current = head;
    while (current != nullptr) {
        PlaylistNode* next = current->next;
        delete current;
        current = next;
    }
    head = tail = nullptr;
    size = 0;
}

PlaylistNode* PlaylistEngine::getNodeAt(int index) const {
    if (index < 0 || index >= size) return nullptr;
    
    PlaylistNode* current = head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    return current;
}

void PlaylistEngine::insertNodeAt(PlaylistNode* node, int index) {
    if (index < 0 || index > size) return;
    
    if (index == 0) {
        // Insert at beginning
        node->next = head;
        if (head) head->prev = node;
        head = node;
        if (!tail) tail = node;
    } else if (index == size) {
        // Insert at end
        node->prev = tail;
        if (tail) tail->next = node;
        tail = node;
        if (!head) head = node;
    } else {
        // Insert in middle
        PlaylistNode* current = getNodeAt(index);
        node->prev = current->prev;
        node->next = current;
        current->prev->next = node;
        current->prev = node;
    }
    size++;
}

void PlaylistEngine::removeNode(PlaylistNode* node) {
    if (!node) return;
    
    if (node->prev) {
        node->prev->next = node->next;
    } else {
        head = node->next;
    }
    
    if (node->next) {
        node->next->prev = node->prev;
    } else {
        tail = node->prev;
    }
    
    delete node;
    size--;
}

void PlaylistEngine::addSong(const std::string& title, const std::string& artist, int duration) {
    Song song(title, artist, duration);
    PlaylistNode* newNode = new PlaylistNode(song);
    
    // Add to end of list
    insertNodeAt(newNode, size);
    
    // Record action for undo
    undoStack.push(PlaylistAction(ActionType::ADD, song, size - 1));
}

void PlaylistEngine::deleteSong(int index) {
    if (index < 0 || index >= size) {
        std::cout << "Invalid index for deletion!\n";
        return;
    }
    
    PlaylistNode* nodeToDelete = getNodeAt(index);
    if (!nodeToDelete) return;
    
    // Record action for undo
    undoStack.push(PlaylistAction(ActionType::DELETE, nodeToDelete->song, index));
    
    removeNode(nodeToDelete);
}

void PlaylistEngine::moveSong(int fromIndex, int toIndex) {
    if (fromIndex < 0 || fromIndex >= size || toIndex < 0 || toIndex >= size) {
        std::cout << "Invalid indices for move operation!\n";
        return;
    }
    
    if (fromIndex == toIndex) return;
    
    PlaylistNode* nodeToMove = getNodeAt(fromIndex);
    if (!nodeToMove) return;
    
    // Record action for undo
    undoStack.push(PlaylistAction(ActionType::MOVE, nodeToMove->song, fromIndex, toIndex));
    
    // Remove from current position
    removeNode(nodeToMove);
    
    // Insert at new position
    insertNodeAt(nodeToMove, toIndex);
}

void PlaylistEngine::reversePlaylist() {
    if (size <= 1) return;
    
    PlaylistNode* current = head;
    PlaylistNode* temp = nullptr;
    
    // Swap prev and next pointers for all nodes
    while (current != nullptr) {
        temp = current->prev;
        current->prev = current->next;
        current->next = temp;
        current = current->prev;
    }
    
    // Swap head and tail
    temp = head;
    head = tail;
    tail = temp;
}

void PlaylistEngine::undoLastNEdits(int n) {
    int undoCount = std::min(n, static_cast<int>(undoStack.size()));
    
    for (int i = 0; i < undoCount; i++) {
        if (undoStack.empty()) break;
        
        PlaylistAction action = undoStack.top();
        undoStack.pop();
        
        switch (action.type) {
            case ActionType::ADD:
                // Undo add by deleting
                deleteSong(action.index1);
                break;
            case ActionType::DELETE:
                // Undo delete by adding back
                {
                    PlaylistNode* newNode = new PlaylistNode(action.song);
                    insertNodeAt(newNode, action.index1);
                }
                break;
            case ActionType::MOVE:
                // Undo move by moving back
                moveSong(action.index2, action.index1);
                break;
        }
    }
}

void PlaylistEngine::shuffleWithConstraints() {
    if (size <= 1) return;
    
    std::vector<Song> songs = getSongs();
    std::random_device rd;
    std::mt19937 gen(rd());
    
    bool validShuffle = false;
    int maxAttempts = 100; // Prevent infinite loop
    int attempts = 0;
    
    while (!validShuffle && attempts < maxAttempts) {
        std::shuffle(songs.begin(), songs.end(), gen);
        
        // Check for consecutive same artists
        validShuffle = true;
        for (size_t i = 1; i < songs.size(); i++) {
            if (songs[i].artist == songs[i-1].artist) {
                validShuffle = false;
                break;
            }
        }
        attempts++;
    }
    
    // Rebuild playlist with shuffled songs
    clearList();
    for (const auto& song : songs) {
        PlaylistNode* newNode = new PlaylistNode(song);
        insertNodeAt(newNode, size);
    }
}

void PlaylistEngine::displayPlaylist() const {
    if (size == 0) {
        std::cout << "Playlist is empty!\n";
        return;
    }
    
    std::cout << "\n=== Current Playlist ===\n";
    PlaylistNode* current = head;
    int index = 1;
    
    while (current != nullptr) {
        std::cout << index << ". " << current->song.title << " by " << current->song.artist 
                  << " (" << current->song.getFormattedDuration() << ")\n";
        current = current->next;
        index++;
    }
    std::cout << "Total songs: " << size << "\n";
}

std::vector<Song> PlaylistEngine::getSongs() const {
    std::vector<Song> songs;
    PlaylistNode* current = head;
    
    while (current != nullptr) {
        songs.push_back(current->song);
        current = current->next;
    }
    
    return songs;
} 