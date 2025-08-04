#ifndef PLAYLIST_SORTER_H
#define PLAYLIST_SORTER_H

#include "song.h"
#include <vector>
#include <algorithm>

class PlaylistSorter {
private:
    // Merge sort helper methods
    void mergeSort(std::vector<Song>& songs, int left, int right);
    void merge(std::vector<Song>& songs, int left, int mid, int right);
    
    // Quick sort helper methods
    void quickSort(std::vector<Song>& songs, int low, int high);
    int partition(std::vector<Song>& songs, int low, int high);
    
public:
    // Constructor
    PlaylistSorter() = default;
    
    // Sorting methods
    std::vector<Song> sortByTitle(const std::vector<Song>& songs);
    std::vector<Song> sortByDuration(const std::vector<Song>& songs, bool ascending = true);
    std::vector<Song> sortByRecentlyAdded(const std::vector<Song>& songs);
    
    // Utility methods
    void displaySortingStats(const std::vector<Song>& original, const std::vector<Song>& sorted);
    
    // Time complexity annotations:
    // sortByTitle: O(n log n) - merge sort
    // sortByDuration: O(n log n) - merge sort
    // sortByRecentlyAdded: O(n log n) - merge sort
    // mergeSort: O(n log n) - divide and conquer
    // quickSort: O(n log n) average, O(n^2) worst case
};

#endif // PLAYLIST_SORTER_H 