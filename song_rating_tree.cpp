#include "song_rating_tree.h"
#include <iostream>
#include <algorithm>

SongRatingTree::SongRatingTree() : root(nullptr) {
}

SongRatingTree::~SongRatingTree() {
    clearTree(root);
}

void SongRatingTree::clearTree(RatingNode* node) {
    if (node) {
        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }
}

RatingNode* SongRatingTree::insertNode(RatingNode* node, int rating) {
    if (node == nullptr) {
        return new RatingNode(rating);
    }
    
    if (rating < node->rating) {
        node->left = insertNode(node->left, rating);
    } else if (rating > node->rating) {
        node->right = insertNode(node->right, rating);
    }
    
    return node;
}

RatingNode* SongRatingTree::findNode(RatingNode* node, int rating) const {
    if (node == nullptr || node->rating == rating) {
        return node;
    }
    
    if (rating < node->rating) {
        return findNode(node->left, rating);
    }
    
    return findNode(node->right, rating);
}

RatingNode* SongRatingTree::findMin(RatingNode* node) const {
    if (node == nullptr || node->left == nullptr) {
        return node;
    }
    return findMin(node->left);
}

RatingNode* SongRatingTree::deleteNode(RatingNode* node, int rating) {
    if (node == nullptr) {
        return nullptr;
    }
    
    if (rating < node->rating) {
        node->left = deleteNode(node->left, rating);
    } else if (rating > node->rating) {
        node->right = deleteNode(node->right, rating);
    } else {
        // Node to delete found
        if (node->left == nullptr) {
            RatingNode* temp = node->right;
            delete node;
            return temp;
        } else if (node->right == nullptr) {
            RatingNode* temp = node->left;
            delete node;
            return temp;
        }
        
        // Node has two children
        RatingNode* temp = findMin(node->right);
        node->rating = temp->rating;
        node->songs = temp->songs;
        node->right = deleteNode(node->right, temp->rating);
    }
    
    return node;
}

void SongRatingTree::inorderTraversal(RatingNode* node, std::vector<std::pair<int, std::vector<Song>>>& result) const {
    if (node) {
        inorderTraversal(node->left, result);
        result.push_back({node->rating, node->songs});
        inorderTraversal(node->right, result);
    }
}

void SongRatingTree::insertSong(const Song& song, int rating) {
    if (rating < 1 || rating > 5) {
        std::cout << "Invalid rating! Must be between 1 and 5.\n";
        return;
    }
    
    RatingNode* node = findNode(root, rating);
    if (node) {
        // Rating bucket exists, add song to it
        node->songs.push_back(song);
    } else {
        // Create new rating bucket
        root = insertNode(root, rating);
        node = findNode(root, rating);
        node->songs.push_back(song);
    }
}

std::vector<Song> SongRatingTree::searchByRating(int rating) const {
    if (rating < 1 || rating > 5) {
        return std::vector<Song>();
    }
    
    RatingNode* node = findNode(root, rating);
    if (node) {
        return node->songs;
    }
    return std::vector<Song>();
}

void SongRatingTree::deleteSong(const std::string& songTitle) {
    // Search through all rating buckets
    std::vector<std::pair<int, std::vector<Song>>> allRatings;
    inorderTraversal(root, allRatings);
    
    for (auto& ratingPair : allRatings) {
        auto& songs = ratingPair.second;
        auto it = std::find_if(songs.begin(), songs.end(),
                              [&songTitle](const Song& song) {
                                  return song.title == songTitle;
                              });
        
        if (it != songs.end()) {
            songs.erase(it);
            
            // If bucket becomes empty, remove the rating node
            if (songs.empty()) {
                root = deleteNode(root, ratingPair.first);
            }
            break;
        }
    }
}

void SongRatingTree::displayAllRatings() const {
    if (root == nullptr) {
        std::cout << "No rated songs available.\n";
        return;
    }
    
    std::cout << "\n=== Songs by Rating ===\n";
    std::vector<std::pair<int, std::vector<Song>>> allRatings;
    inorderTraversal(root, allRatings);
    
    for (const auto& ratingPair : allRatings) {
        std::cout << "\nRating " << ratingPair.first << " stars (" 
                  << ratingPair.second.size() << " songs):\n";
        for (const auto& song : ratingPair.second) {
            std::cout << "  - " << song.title << " by " << song.artist 
                      << " (" << song.getFormattedDuration() << ")\n";
        }
    }
}

std::vector<std::pair<int, int>> SongRatingTree::getSongCountByRating() const {
    std::vector<std::pair<int, int>> result;
    std::vector<std::pair<int, std::vector<Song>>> allRatings;
    inorderTraversal(root, allRatings);
    
    for (const auto& ratingPair : allRatings) {
        result.push_back({ratingPair.first, static_cast<int>(ratingPair.second.size())});
    }
    
    return result;
} 