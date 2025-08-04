#include "playlist_sorter.h"
#include <iostream>
#include <chrono>

void PlaylistSorter::mergeSort(std::vector<Song>& songs, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(songs, left, mid);
        mergeSort(songs, mid + 1, right);
        merge(songs, left, mid, right);
    }
}

void PlaylistSorter::merge(std::vector<Song>& songs, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    std::vector<Song> leftArray(n1);
    std::vector<Song> rightArray(n2);
    
    // Copy data to temporary arrays
    for (int i = 0; i < n1; i++) {
        leftArray[i] = songs[left + i];
    }
    for (int j = 0; j < n2; j++) {
        rightArray[j] = songs[mid + 1 + j];
    }
    
    // Merge the temporary arrays back
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftArray[i].title <= rightArray[j].title) {
            songs[k] = leftArray[i];
            i++;
        } else {
            songs[k] = rightArray[j];
            j++;
        }
        k++;
    }
    
    // Copy remaining elements
    while (i < n1) {
        songs[k] = leftArray[i];
        i++;
        k++;
    }
    while (j < n2) {
        songs[k] = rightArray[j];
        j++;
        k++;
    }
}

void PlaylistSorter::quickSort(std::vector<Song>& songs, int low, int high) {
    if (low < high) {
        int pi = partition(songs, low, high);
        quickSort(songs, low, pi - 1);
        quickSort(songs, pi + 1, high);
    }
}

int PlaylistSorter::partition(std::vector<Song>& songs, int low, int high) {
    Song pivot = songs[high];
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        if (songs[j].duration <= pivot.duration) {
            i++;
            std::swap(songs[i], songs[j]);
        }
    }
    std::swap(songs[i + 1], songs[high]);
    return i + 1;
}

std::vector<Song> PlaylistSorter::sortByTitle(const std::vector<Song>& songs) {
    std::vector<Song> sortedSongs = songs;
    
    auto start = std::chrono::high_resolution_clock::now();
    mergeSort(sortedSongs, 0, sortedSongs.size() - 1);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Sort by title completed in " << duration.count() << " microseconds\n";
    
    return sortedSongs;
}

std::vector<Song> PlaylistSorter::sortByDuration(const std::vector<Song>& songs, bool ascending) {
    std::vector<Song> sortedSongs = songs;
    
    auto start = std::chrono::high_resolution_clock::now();
    quickSort(sortedSongs, 0, sortedSongs.size() - 1);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Sort by duration completed in " << duration.count() << " microseconds\n";
    
    if (!ascending) {
        std::reverse(sortedSongs.begin(), sortedSongs.end());
    }
    
    return sortedSongs;
}

std::vector<Song> PlaylistSorter::sortByRecentlyAdded(const std::vector<Song>& songs) {
    std::vector<Song> sortedSongs = songs;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // Sort by added time (most recent first)
    std::sort(sortedSongs.begin(), sortedSongs.end(), 
              [](const Song& a, const Song& b) {
                  return a.addedTime > b.addedTime;
              });
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Sort by recently added completed in " << duration.count() << " microseconds\n";
    
    return sortedSongs;
}

void PlaylistSorter::displaySortingStats(const std::vector<Song>& original, const std::vector<Song>& sorted) {
    std::cout << "\n=== Sorting Statistics ===\n";
    std::cout << "Original size: " << original.size() << " songs\n";
    std::cout << "Sorted size: " << sorted.size() << " songs\n";
    
    if (original.size() > 0 && sorted.size() > 0) {
        std::cout << "First song: " << sorted[0].title << "\n";
        std::cout << "Last song: " << sorted[sorted.size() - 1].title << "\n";
    }
} 