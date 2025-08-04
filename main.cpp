#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "playlist_engine.h"
#include "playback_history.h"
#include "song_rating_tree.h"
#include "song_lookup.h"
#include "playlist_sorter.h"
#include "system_snapshot.h"

using namespace std;

void displayMenu() {
    cout << "\n=== PlayWise Music Playlist Management System ===\n";
    cout << "1. Create/Manage Playlist\n";
    cout << "2. Playback History Operations\n";
    cout << "3. Song Rating Management\n";
    cout << "4. Song Lookup\n";
    cout << "5. Sort Playlist\n";
    cout << "6. System Snapshot\n";
    cout << "7. Undo Last N Edits\n";
    cout << "8. Shuffle with Constraints\n";
    cout << "9. Exit\n";
    cout << "Enter your choice: ";
}

void playlistMenu(PlaylistEngine& engine) {
    cout << "\n=== Playlist Management ===\n";
    cout << "1. Add Song\n";
    cout << "2. Delete Song\n";
    cout << "3. Move Song\n";
    cout << "4. Reverse Playlist\n";
    cout << "5. Display Playlist\n";
    cout << "6. Back to Main Menu\n";
    cout << "Enter your choice: ";
    
    int choice;
    cin >> choice;
    
    switch(choice) {
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
            engine.addSong(title, artist, duration);
            cout << "Song added successfully!\n";
            break;
        }
        case 2: {
            int index;
            cout << "Enter song index to delete: ";
            cin >> index;
            engine.deleteSong(index);
            cout << "Song deleted successfully!\n";
            break;
        }
        case 3: {
            int from, to;
            cout << "Enter source index: ";
            cin >> from;
            cout << "Enter destination index: ";
            cin >> to;
            engine.moveSong(from, to);
            cout << "Song moved successfully!\n";
            break;
        }
        case 4:
            engine.reversePlaylist();
            cout << "Playlist reversed successfully!\n";
            break;
        case 5:
            engine.displayPlaylist();
            break;
    }
}

void historyMenu(PlaybackHistory& history, PlaylistEngine& engine) {
    cout << "\n=== Playback History ===\n";
    cout << "1. Add played song\n";
    cout << "2. Undo last play\n";
    cout << "3. Display history\n";
    cout << "4. Back to Main Menu\n";
    cout << "Enter your choice: ";
    
    int choice;
    cin >> choice;
    
    switch(choice) {
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
            Song song(title, artist, duration);
            history.addPlayedSong(song);
            cout << "Song added to history!\n";
            break;
        }
        case 2: {
            Song lastSong = history.undoLastPlay();
            if (lastSong.title != "") {
                engine.addSong(lastSong.title, lastSong.artist, lastSong.duration);
                cout << "Last played song re-added to playlist!\n";
            } else {
                cout << "No songs in history to undo!\n";
            }
            break;
        }
        case 3:
            history.displayHistory();
            break;
    }
}

void ratingMenu(SongRatingTree& ratingTree) {
    cout << "\n=== Song Rating Management ===\n";
    cout << "1. Add song with rating\n";
    cout << "2. Search songs by rating\n";
    cout << "3. Delete song\n";
    cout << "4. Display all ratings\n";
    cout << "5. Back to Main Menu\n";
    cout << "Enter your choice: ";
    
    int choice;
    cin >> choice;
    
    switch(choice) {
        case 1: {
            string title, artist;
            int duration, rating;
            cout << "Enter song title: ";
            cin.ignore();
            getline(cin, title);
            cout << "Enter artist: ";
            getline(cin, artist);
            cout << "Enter duration (seconds): ";
            cin >> duration;
            cout << "Enter rating (1-5): ";
            cin >> rating;
            Song song(title, artist, duration);
            ratingTree.insertSong(song, rating);
            cout << "Song added with rating!\n";
            break;
        }
        case 2: {
            int rating;
            cout << "Enter rating to search (1-5): ";
            cin >> rating;
            vector<Song> songs = ratingTree.searchByRating(rating);
            cout << "Songs with rating " << rating << ":\n";
            for (const auto& song : songs) {
                cout << "- " << song.title << " by " << song.artist << " (" << song.duration << "s)\n";
            }
            break;
        }
        case 3: {
            string title;
            cout << "Enter song title to delete: ";
            cin.ignore();
            getline(cin, title);
            ratingTree.deleteSong(title);
            cout << "Song deleted from ratings!\n";
            break;
        }
        case 4:
            ratingTree.displayAllRatings();
            break;
    }
}

void lookupMenu(SongLookup& lookup) {
    cout << "\n=== Song Lookup ===\n";
    cout << "1. Add song to lookup\n";
    cout << "2. Search by title\n";
    cout << "3. Search by ID\n";
    cout << "4. Display all songs\n";
    cout << "5. Back to Main Menu\n";
    cout << "Enter your choice: ";
    
    int choice;
    cin >> choice;
    
    switch(choice) {
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
            Song song(title, artist, duration);
            lookup.addSong(song);
            cout << "Song added to lookup!\n";
            break;
        }
        case 2: {
            string title;
            cout << "Enter song title to search: ";
            cin.ignore();
            getline(cin, title);
            Song* song = lookup.searchByTitle(title);
            if (song) {
                cout << "Found: " << song->title << " by " << song->artist << " (" << song->duration << "s)\n";
            } else {
                cout << "Song not found!\n";
            }
            break;
        }
        case 3: {
            int id;
            cout << "Enter song ID to search: ";
            cin >> id;
            Song* song = lookup.searchById(id);
            if (song) {
                cout << "Found: " << song->title << " by " << song->artist << " (" << song->duration << "s)\n";
            } else {
                cout << "Song not found!\n";
            }
            break;
        }
        case 4:
            lookup.displayAllSongs();
            break;
    }
}

void sortMenu(PlaylistSorter& sorter, PlaylistEngine& engine) {
    cout << "\n=== Playlist Sorting ===\n";
    cout << "1. Sort by title (alphabetical)\n";
    cout << "2. Sort by duration (ascending)\n";
    cout << "3. Sort by duration (descending)\n";
    cout << "4. Sort by recently added\n";
    cout << "5. Back to Main Menu\n";
    cout << "Enter your choice: ";
    
    int choice;
    cin >> choice;
    
    vector<Song> songs = engine.getSongs();
    vector<Song> sortedSongs;
    
    switch(choice) {
        case 1:
            sortedSongs = sorter.sortByTitle(songs);
            break;
        case 2:
            sortedSongs = sorter.sortByDuration(songs, true);
            break;
        case 3:
            sortedSongs = sorter.sortByDuration(songs, false);
            break;
        case 4:
            sortedSongs = sorter.sortByRecentlyAdded(songs);
            break;
    }
    
    if (choice >= 1 && choice <= 4) {
        cout << "Sorted playlist:\n";
        for (size_t i = 0; i < sortedSongs.size(); i++) {
            cout << i + 1 << ". " << sortedSongs[i].title << " by " << sortedSongs[i].artist 
                 << " (" << sortedSongs[i].duration << "s)\n";
        }
    }
}

void undoMenu(PlaylistEngine& engine) {
    cout << "\n=== Undo Last N Edits ===\n";
    int n;
    cout << "Enter number of edits to undo: ";
    cin >> n;
    engine.undoLastNEdits(n);
    cout << "Undid last " << n << " edits!\n";
}

void shuffleMenu(PlaylistEngine& engine) {
    cout << "\n=== Shuffle with Constraints ===\n";
    engine.shuffleWithConstraints();
    cout << "Playlist shuffled with constraints (no consecutive same artist)!\n";
    engine.displayPlaylist();
}

int main() {
    PlaylistEngine engine;
    PlaybackHistory history;
    SongRatingTree ratingTree;
    SongLookup lookup;
    PlaylistSorter sorter;
    SystemSnapshot snapshot;
    
    // Add some sample data
    engine.addSong("Bohemian Rhapsody", "Queen", 354);
    engine.addSong("Hotel California", "Eagles", 391);
    engine.addSong("Stairway to Heaven", "Led Zeppelin", 482);
    engine.addSong("Imagine", "John Lennon", 183);
    engine.addSong("Hey Jude", "The Beatles", 431);
    
    lookup.addSong(Song("Bohemian Rhapsody", "Queen", 354));
    lookup.addSong(Song("Hotel California", "Eagles", 391));
    lookup.addSong(Song("Stairway to Heaven", "Led Zeppelin", 482));
    
    ratingTree.insertSong(Song("Bohemian Rhapsody", "Queen", 354), 5);
    ratingTree.insertSong(Song("Hotel California", "Eagles", 391), 4);
    ratingTree.insertSong(Song("Stairway to Heaven", "Led Zeppelin", 482), 5);
    
    int choice;
    do {
        displayMenu();
        cin >> choice;
        
        switch(choice) {
            case 1:
                playlistMenu(engine);
                break;
            case 2:
                historyMenu(history, engine);
                break;
            case 3:
                ratingMenu(ratingTree);
                break;
            case 4:
                lookupMenu(lookup);
                break;
            case 5:
                sortMenu(sorter, engine);
                break;
            case 6: {
                cout << "\n=== System Snapshot ===\n";
                auto stats = snapshot.exportSnapshot(engine, history, ratingTree, lookup);
                cout << "Top 5 longest songs:\n";
                for (const auto& song : stats.topLongestSongs) {
                    cout << "- " << song.title << " (" << song.duration << "s)\n";
                }
                cout << "\nMost recently played songs:\n";
                for (const auto& song : stats.recentlyPlayed) {
                    cout << "- " << song.title << " by " << song.artist << "\n";
                }
                cout << "\nSong count by rating:\n";
                for (const auto& pair : stats.songCountByRating) {
                    cout << "Rating " << pair.first << ": " << pair.second << " songs\n";
                }
                break;
            }
            case 7:
                undoMenu(engine);
                break;
            case 8:
                shuffleMenu(engine);
                break;
            case 9:
                cout << "Thank you for using PlayWise!\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 9);
    
    return 0;
} 