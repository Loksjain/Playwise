#ifndef SONG_RATING_TREE_H
#define SONG_RATING_TREE_H

#include "song.h"
#include <vector>
#include <string>

// Node structure for BST
struct RatingNode {
    int rating;
    std::vector<Song> songs;
    RatingNode* left;
    RatingNode* right;
    
    RatingNode(int r) : rating(r), left(nullptr), right(nullptr) {}
};

class SongRatingTree {
private:
    RatingNode* root;
    
    // Helper methods
    RatingNode* insertNode(RatingNode* node, int rating);
    RatingNode* findNode(RatingNode* node, int rating) const;
    RatingNode* deleteNode(RatingNode* node, int rating);
    RatingNode* findMin(RatingNode* node) const;
    void inorderTraversal(RatingNode* node, std::vector<std::pair<int, std::vector<Song>>>& result) const;
    void clearTree(RatingNode* node);
    
public:
    // Constructor and destructor
    SongRatingTree();
    ~SongRatingTree();
    
    // Core operations
    void insertSong(const Song& song, int rating);
    std::vector<Song> searchByRating(int rating) const;
    void deleteSong(const std::string& songTitle);
    
    // Utility methods
    void displayAllRatings() const;
    std::vector<std::pair<int, int>> getSongCountByRating() const;
    
    // Time complexity annotations:
    // insertSong: O(log n) - BST insertion
    // searchByRating: O(log n) - BST search
    // deleteSong: O(log n) - BST deletion
    // displayAllRatings: O(n) - inorder traversal
    // getSongCountByRating: O(n) - inorder traversal
};

#endif // SONG_RATING_TREE_H 