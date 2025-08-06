#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <random>
#include <map>
#include <stdexcept>

#include "song.h"
#include "playlist_engine.h"
#include "playback_history.h"
#include "song_rating_tree.h"
#include "song_lookup.h"
#include "playlist_sorter.h"
#include "system_snapshot.h"

using namespace std;

class SimpleGUI {
private:
    PlaylistEngine* playlistEngine;
    PlaybackHistory* history;
    SongRatingTree* ratingTree;
    SongLookup* lookup;
    PlaylistSorter* sorter;
    SystemSnapshot* snapshot;
    
    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
    
    void printHeader() {
        cout << "==============================================================\n";
        cout << "                PlayWise Console GUI\n";
        cout << "              Smart Music Playlist Management System\n";
        cout << "==============================================================\n\n";
    }
    
    void printMenu() {
        cout << "MAIN MENU:\n";
        cout << "+------------------------------------------------------------+\n";
        cout << "| 1. Playlist Management                                     |\n";
        cout << "| 2. Playback History                                        |\n";
        cout << "| 3. Song Rating System                                      |\n";
        cout << "| 4. Song Lookup                                             |\n";
        cout << "| 5. Sorting & Analytics                                     |\n";
        cout << "| 6. Undo Operations                                         |\n";
        cout << "| 7. Shuffle with Constraints                                |\n";
        cout << "| 8. System Snapshot                                         |\n";
        cout << "| 9. Quick Demo                                              |\n";
        cout << "| 0. Exit                                                    |\n";
        cout << "+------------------------------------------------------------+\n";
        cout << "Enter your choice: ";
    }
    
    void playlistMenu() {
        while (true) {
            clearScreen();
            printHeader();
            cout << "PLAYLIST MANAGEMENT\n";
            cout << "+------------------------------------------------------------+\n";
            cout << "| 1. Add Song                                               |\n";
            cout << "| 2. Delete Song                                            |\n";
            cout << "| 3. Move Song                                              |\n";
            cout << "| 4. Reverse Playlist                                       |\n";
            cout << "| 5. Display Playlist                                       |\n";
            cout << "| 0. Back to Main Menu                                      |\n";
            cout << "+------------------------------------------------------------+\n";
            cout << "Enter your choice: ";
            
            int choice;
            cin >> choice;
            
            switch (choice) {
                case 1: {
                    string title, artist;
                    int duration;
                    cout << "Enter song title: ";
                    cin.ignore();
                    getline(cin, title);
                    cout << "Enter artist: ";
                    getline(cin, artist);
                    cout << "Enter duration (seconds): ";
                    cin >> duration;
                    
                    playlistEngine->addSong(title, artist, duration);
                    Song song(title, artist, duration);
                    lookup->addSong(song);
                    cout << "Song added successfully!\n";
                    break;
                }
                case 2: {
                    cout << "Current playlist:\n";
                    playlistEngine->displayPlaylist();
                    cout << "Enter song index to delete: ";
                    int index;
                    cin >> index;
                    playlistEngine->deleteSong(index);
                    cout << "Song deleted successfully!\n";
                    break;
                }
                case 3: {
                    cout << "Current playlist:\n";
                    playlistEngine->displayPlaylist();
                    cout << "Enter source index: ";
                    int from, to;
                    cin >> from;
                    cout << "Enter destination index: ";
                    cin >> to;
                    playlistEngine->moveSong(from, to);
                    cout << "Song moved successfully!\n";
                    break;
                }
                case 4: {
                    playlistEngine->reversePlaylist();
                    cout << "Playlist reversed successfully!\n";
                    break;
                }
                case 5: {
                    playlistEngine->displayPlaylist();
                    break;
                }
                case 0:
                    return;
                default:
                    cout << "Invalid choice!\n";
            }
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }
    
    void historyMenu() {
        while (true) {
            clearScreen();
            printHeader();
            cout << "PLAYBACK HISTORY\n";
            cout << "+------------------------------------------------------------+\n";
            cout << "| 1. Play Song (Add to History)                              |\n";
            cout << "| 2. Undo Last Play                                          |\n";
            cout << "| 3. Display History                                         |\n";
            cout << "| 0. Back to Main Menu                                       |\n";
            cout << "+------------------------------------------------------------+\n";
            cout << "Enter your choice: ";
            
            int choice;
            cin >> choice;
            
            switch (choice) {
                case 1: {
                    cout << "Current playlist:\n";
                    playlistEngine->displayPlaylist();
                    cout << "Enter song index to play: ";
                    int index;
                    cin >> index;
                    
                    vector<Song> songs = playlistEngine->getSongs();
                    if (index >= 0 && index < static_cast<int>(songs.size())) {
                        history->addPlayedSong(songs[index]);
                        cout << "Now playing: " << songs[index].title << " by " << songs[index].artist << "\n";
                    } else {
                        cout << "Invalid song index!\n";
                    }
                    break;
                }
                case 2: {
                    try {
                        Song song = history->undoLastPlay();
                        cout << "Undone: " << song.title << " by " << song.artist << "\n";
                    } catch (const exception& e) {
                        cout << "No songs to undo!\n";
                    }
                    break;
                }
                case 3: {
                    history->displayHistory();
                    break;
                }
                case 0:
                    return;
                default:
                    cout << "Invalid choice!\n";
            }
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }
    
    void ratingMenu() {
        while (true) {
            clearScreen();
            printHeader();
            cout << "SONG RATING SYSTEM\n";
            cout << "+------------------------------------------------------------+\n";
            cout << "| 1. Add/Update Rating                                      |\n";
            cout << "| 2. Search by Rating                                       |\n";
            cout << "| 3. Display All Ratings                                    |\n";
            cout << "| 0. Back to Main Menu                                      |\n";
            cout << "+------------------------------------------------------------+\n";
            cout << "Enter your choice: ";
            
            int choice;
            cin >> choice;
            
            switch (choice) {
                case 1: {
                    cout << "Available songs:\n";
                    lookup->displayAllSongs();
                    cout << "Enter song title: ";
                    string title;
                    cin.ignore();
                    getline(cin, title);
                    cout << "Enter rating (1-5): ";
                    int rating;
                    cin >> rating;
                    
                    if (rating >= 1 && rating <= 5) {
                        Song* song = lookup->searchByTitle(title);
                        if (song) {
                            ratingTree->insertSong(*song, rating);
                            cout << "Rating added successfully!\n";
                        } else {
                            cout << "Song not found!\n";
                        }
                    } else {
                        cout << "Invalid rating! Must be 1-5.\n";
                    }
                    break;
                }
                case 2: {
                    cout << "Enter rating to search (1-5): ";
                    int rating;
                    cin >> rating;
                    vector<Song> songs = ratingTree->searchByRating(rating);
                    if (songs.empty()) {
                        cout << "No songs found with rating " << rating << "\n";
                    } else {
                        cout << "Songs with rating " << rating << ":\n";
                        for (const auto& song : songs) {
                            cout << "  - " << song.title << " by " << song.artist << "\n";
                        }
                    }
                    break;
                }
                case 3: {
                    ratingTree->displayAllRatings();
                    break;
                }
                case 0:
                    return;
                default:
                    cout << "Invalid choice!\n";
            }
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }
    
    void lookupMenu() {
        while (true) {
            clearScreen();
            printHeader();
            cout << "SONG LOOKUP\n";
            cout << "+------------------------------------------------------------+\n";
            cout << "| 1. Search by Title                                        |\n";
            cout << "| 2. Search by ID                                           |\n";
            cout << "| 3. Display All Songs                                      |\n";
            cout << "| 0. Back to Main Menu                                      |\n";
            cout << "+------------------------------------------------------------+\n";
            cout << "Enter your choice: ";
            
            int choice;
            cin >> choice;
            
            switch (choice) {
                case 1: {
                    cout << "Enter song title: ";
                    string title;
                    cin.ignore();
                    getline(cin, title);
                    Song* song = lookup->searchByTitle(title);
                    if (song) {
                        cout << "Found: " << song->toString() << "\n";
                    } else {
                        cout << "Song not found!\n";
                    }
                    break;
                }
                case 2: {
                    cout << "Enter song ID: ";
                    int id;
                    cin >> id;
                    Song* song = lookup->searchById(id);
                    if (song) {
                        cout << "Found: " << song->toString() << "\n";
                    } else {
                        cout << "Song not found!\n";
                    }
                    break;
                }
                case 3: {
                    lookup->displayAllSongs();
                    break;
                }
                case 0:
                    return;
                default:
                    cout << "Invalid choice!\n";
            }
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }
    
    void sortingMenu() {
        while (true) {
            clearScreen();
            printHeader();
            cout << "SORTING & ANALYTICS\n";
            cout << "+------------------------------------------------------------+\n";
            cout << "| 1. Sort by Title (Merge Sort)                             |\n";
            cout << "| 2. Sort by Duration (Quick Sort)                          |\n";
            cout << "| 3. Sort by Recently Added (std::sort)                     |\n";
            cout << "| 4. Performance Comparison                                 |\n";
            cout << "| 0. Back to Main Menu                                      |\n";
            cout << "+------------------------------------------------------------+\n";
            cout << "Enter your choice: ";
            
            int choice;
            cin >> choice;
            
            vector<Song> songs = playlistEngine->getSongs();
            if (songs.empty()) {
                cout << "No songs in playlist to sort!\n";
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
                return;
            }
            
            switch (choice) {
                case 1: {
                    cout << "Original playlist:\n";
                    playlistEngine->displayPlaylist();
                    cout << "\nSorting by title...\n";
                    vector<Song> sorted = sorter->sortByTitle(songs);
                    cout << "Sorted playlist:\n";
                    for (const auto& song : sorted) {
                        cout << "  - " << song.title << " by " << song.artist << "\n";
                    }
                    break;
                }
                case 2: {
                    cout << "Original playlist:\n";
                    playlistEngine->displayPlaylist();
                    cout << "\nSorting by duration...\n";
                    vector<Song> sorted = sorter->sortByDuration(songs, true);
                    cout << "Sorted playlist (ascending):\n";
                    for (const auto& song : sorted) {
                        cout << "  - " << song.title << " (" << song.getFormattedDuration() << ")\n";
                    }
                    break;
                }
                case 3: {
                    cout << "Original playlist:\n";
                    playlistEngine->displayPlaylist();
                    cout << "\nSorting by recently added...\n";
                    vector<Song> sorted = sorter->sortByRecentlyAdded(songs);
                    cout << "Sorted playlist:\n";
                    for (const auto& song : sorted) {
                        cout << "  - " << song.title << " by " << song.artist << "\n";
                    }
                    break;
                }
                case 4: {
                    cout << "Performance comparison:\n";
                    sorter->displaySortingStats(songs, sorter->sortByTitle(songs));
                    break;
                }
                case 0:
                    return;
                default:
                    cout << "Invalid choice!\n";
            }
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }
    
    void undoMenu() {
        clearScreen();
        printHeader();
        cout << "UNDO OPERATIONS\n";
        cout << "+------------------------------------------------------------+\n";
        cout << "Enter number of edits to undo: ";
        int n;
        cin >> n;
        
        if (n > 0) {
            playlistEngine->undoLastNEdits(n);
            cout << "Undone " << n << " edit(s) successfully!\n";
        } else {
            cout << "Invalid number!\n";
        }
        
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }
    
    void shuffleMenu() {
        clearScreen();
        printHeader();
        cout << "SHUFFLE WITH CONSTRAINTS\n";
        cout << "+------------------------------------------------------------+\n";
        cout << "Shuffling playlist (avoiding consecutive same artist)...\n";
        playlistEngine->shuffleWithConstraints();
        cout << "Playlist shuffled successfully!\n";
        cout << "+------------------------------------------------------------+\n";
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }
    
    void snapshotMenu() {
        while (true) {
            clearScreen();
            printHeader();
            cout << "SYSTEM SNAPSHOT\n";
            cout << "+------------------------------------------------------------+\n";
            cout << "| 1. Generate Snapshot                                      |\n";
            cout << "| 2. Export Snapshot                                        |\n";
            cout << "| 0. Back to Main Menu                                      |\n";
            cout << "+------------------------------------------------------------+\n";
            cout << "Enter your choice: ";
            
            int choice;
            cin >> choice;
            
            switch (choice) {
                case 1: {
                    SystemStats stats = snapshot->exportSnapshot(*playlistEngine, *history, *ratingTree, *lookup);
                    snapshot->displaySystemStats(stats);
                    break;
                }
                case 2: {
                    SystemStats stats = snapshot->exportSnapshot(*playlistEngine, *history, *ratingTree, *lookup);
                    string filename = "playwise_snapshot_" + to_string(chrono::system_clock::now().time_since_epoch().count()) + ".txt";
                    
                    cout << "Exporting snapshot to: " << filename << "\n";
                    cout << "Snapshot data:\n";
                    snapshot->displaySystemStats(stats);
                    cout << "Snapshot exported successfully!\n";
                    break;
                }
                case 0:
                    return;
                default:
                    cout << "Invalid choice!\n";
            }
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }
    
    void quickDemo() {
        clearScreen();
        printHeader();
        cout << "QUICK DEMO - PlayWise Features\n";
        cout << "+------------------------------------------------------------+\n";
        
        cout << "Adding sample songs (English & Hindi)...\n";
        
        // English Classics
        playlistEngine->addSong("Bohemian Rhapsody", "Queen", 354);
        playlistEngine->addSong("Hotel California", "Eagles", 391);
        playlistEngine->addSong("Stairway to Heaven", "Led Zeppelin", 482);
        playlistEngine->addSong("Imagine", "John Lennon", 183);
        playlistEngine->addSong("Hey Jude", "The Beatles", 431);
        
        // Hindi Songs by Darshan Raval
        playlistEngine->addSong("Tere Naal", "Darshan Raval", 245);
        playlistEngine->addSong("Ek Ladki Ko Dekha", "Darshan Raval", 278);
        playlistEngine->addSong("Tere Sang Yaara", "Darshan Raval", 312);
        playlistEngine->addSong("Chogada", "Darshan Raval", 267);
        playlistEngine->addSong("Kitni Haseen Hogi", "Darshan Raval", 289);
        
        // Hindi Songs by Arijit Singh
        playlistEngine->addSong("Tum Hi Ho", "Arijit Singh", 234);
        playlistEngine->addSong("Chahun Main Ya Naa", "Arijit Singh", 298);
        playlistEngine->addSong("Gerua", "Arijit Singh", 345);
        playlistEngine->addSong("Agar Tum Saath Ho", "Arijit Singh", 367);
        playlistEngine->addSong("Raabta", "Arijit Singh", 256);
        
        // Hindi Songs by Armaan Malik
        playlistEngine->addSong("Main Rahoon Ya Na Rahoon", "Armaan Malik", 234);
        playlistEngine->addSong("Naina", "Armaan Malik", 289);
        playlistEngine->addSong("Bol Do Na Zara", "Armaan Malik", 267);
        playlistEngine->addSong("Tere Sang Yaara", "Armaan Malik", 312);
        playlistEngine->addSong("Dil Mein Ho Tum", "Armaan Malik", 245);
        
        // More Popular Hindi Songs
        playlistEngine->addSong("Kesariya", "Arijit Singh", 278);
        playlistEngine->addSong("Raataan Lambiyan", "Jubin Nautiyal", 345);
        playlistEngine->addSong("Maan Meri Jaan", "King", 234);
        playlistEngine->addSong("What Jhumka", "Pritam", 289);
        playlistEngine->addSong("Heeriye", "Jasleen Royal", 267);
        
        // Add to lookup
        vector<Song> songs = playlistEngine->getSongs();
        for (const auto& song : songs) {
            lookup->addSong(song);
        }
        
        cout << "Added 25 sample songs (5 English + 20 Hindi)\n";
        
        cout << "Playing some songs...\n";
        history->addPlayedSong(songs[0]);  // Bohemian Rhapsody
        history->addPlayedSong(songs[5]);  // Tere Naal by Darshan Raval
        history->addPlayedSong(songs[10]); // Tum Hi Ho by Arijit Singh
        history->addPlayedSong(songs[15]); // Main Rahoon Ya Na Rahoon by Armaan Malik
        cout << "Added 4 songs to playback history\n";
        
        cout << "Adding ratings...\n";
        ratingTree->insertSong(songs[0], 5);   // Bohemian Rhapsody - 5 stars
        ratingTree->insertSong(songs[5], 5);   // Tere Naal - 5 stars
        ratingTree->insertSong(songs[10], 5);  // Tum Hi Ho - 5 stars
        ratingTree->insertSong(songs[15], 4);  // Main Rahoon Ya Na Rahoon - 4 stars
        ratingTree->insertSong(songs[20], 5);  // Kesariya - 5 stars
        cout << "Added ratings for 5 songs\n";
        
        cout << "Generating system snapshot...\n";
        SystemStats stats = snapshot->exportSnapshot(*playlistEngine, *history, *ratingTree, *lookup);
        cout << "Snapshot generated\n";
        
        cout << "+------------------------------------------------------------+\n";
        cout << "Demo completed! You can now explore all features.\n";
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }

public:
    SimpleGUI() {
        playlistEngine = new PlaylistEngine();
        history = new PlaybackHistory();
        ratingTree = new SongRatingTree();
        lookup = new SongLookup();
        sorter = new PlaylistSorter();
        snapshot = new SystemSnapshot();
    }
    
    ~SimpleGUI() {
        delete playlistEngine;
        delete history;
        delete ratingTree;
        delete lookup;
        delete sorter;
        delete snapshot;
    }
    
    void run() {
        while (true) {
            clearScreen();
            printHeader();
            printMenu();
            
            int choice;
            cin >> choice;
            
            switch (choice) {
                case 1:
                    playlistMenu();
                    break;
                case 2:
                    historyMenu();
                    break;
                case 3:
                    ratingMenu();
                    break;
                case 4:
                    lookupMenu();
                    break;
                case 5:
                    sortingMenu();
                    break;
                case 6:
                    undoMenu();
                    break;
                case 7:
                    shuffleMenu();
                    break;
                case 8:
                    snapshotMenu();
                    break;
                case 9:
                    quickDemo();
                    break;
                case 0:
                    cout << "Thank you for using PlayWise! Goodbye!\n";
                    return;
                default:
                    cout << "Invalid choice! Please try again.\n";
            }
        }
    }
};

int main() {
    SimpleGUI gui;
    gui.run();
    return 0;
} 