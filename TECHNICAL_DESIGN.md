# PlayWise Technical Design Document

## 1. High-Level Architecture

### 1.1 System Overview

PlayWise is a modular music playlist management system built with C++ that demonstrates the practical application of various data structures and algorithms. The system is designed with a layered architecture that separates concerns and promotes maintainability.

```
┌─────────────────────────────────────────────────────────────┐
│                    User Interface Layer                      │
├─────────────────────────────────────────────────────────────┤
│                   Business Logic Layer                       │
├─────────────────────────────────────────────────────────────┤
│                   Data Structure Layer                       │
├─────────────────────────────────────────────────────────────┤
│                    Core Data Layer                           │
└─────────────────────────────────────────────────────────────┘
```

### 1.2 Component Architecture

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│  PlaylistEngine │    │ PlaybackHistory │    │ SongRatingTree  │
│  (Linked List)  │    │    (Stack)      │    │     (BST)       │
└─────────────────┘    └─────────────────┘    └─────────────────┘
         │                       │                       │
         └───────────────────────┼───────────────────────┘
                                 │
                    ┌─────────────────┐    ┌─────────────────┐
                    │   SongLookup    │    │ PlaylistSorter  │
                    │  (Hash Maps)    │    │  (Sort Algos)   │
                    └─────────────────┘    └─────────────────┘
                                 │
                    ┌─────────────────┐
                    │ SystemSnapshot  │
                    │  (Dashboard)    │
                    └─────────────────┘
```

## 2. Data Structure Design

### 2.1 Doubly Linked List - Playlist Engine

**Purpose**: Efficient playlist management with bidirectional traversal and O(1) insertion at ends.

**Structure**:
```cpp
struct PlaylistNode {
    Song song;
    PlaylistNode* prev;
    PlaylistNode* next;
};
```

**Key Operations**:
- `addSong()`: O(1) - inserts at end
- `deleteSong()`: O(n) - traverses to index
- `moveSong()`: O(n) - traverses to both indices
- `reversePlaylist()`: O(n) - swaps all pointers

**Design Rationale**:
- Bidirectional traversal enables efficient reverse operations
- O(1) insertion at ends for typical playlist building
- Memory efficient for dynamic playlist modifications

### 2.2 Stack - Playback History

**Purpose**: LIFO behavior for tracking recently played songs and undo operations.

**Structure**:
```cpp
class PlaybackHistory {
    std::stack<Song> historyStack;
    std::vector<Song> historyVector; // For display
};
```

**Key Operations**:
- `addPlayedSong()`: O(1) - push operation
- `undoLastPlay()`: O(1) - pop operation
- `displayHistory()`: O(n) - vector traversal

**Design Rationale**:
- Stack provides natural LIFO behavior for playback history
- Vector maintains order for display purposes
- Separate structures optimize for different use cases

### 2.3 Binary Search Tree - Song Rating Tree

**Purpose**: Efficient organization and retrieval of songs by rating.

**Structure**:
```cpp
struct RatingNode {
    int rating;
    std::vector<Song> songs;
    RatingNode* left;
    RatingNode* right;
};
```

**Key Operations**:
- `insertSong()`: O(log n) - BST insertion
- `searchByRating()`: O(log n) - BST search
- `deleteSong()`: O(log n) - BST deletion

**Design Rationale**:
- BST provides logarithmic search time
- Rating buckets allow multiple songs per rating
- Balanced tree ensures consistent performance

### 2.4 Hash Maps - Song Lookup

**Purpose**: O(1) constant-time lookup by song title or ID.

**Structure**:
```cpp
class SongLookup {
    std::unordered_map<std::string, Song> titleToSong;
    std::unordered_map<int, Song> idToSong;
};
```

**Key Operations**:
- `searchByTitle()`: O(1) - hash map lookup
- `searchById()`: O(1) - hash map lookup
- `addSong()`: O(1) - hash map insertion

**Design Rationale**:
- Hash maps provide optimal lookup performance
- Dual indexing enables flexible search
- Separate maps maintain data consistency

## 3. Algorithm Design

### 3.1 Merge Sort Implementation

**Purpose**: Stable sorting algorithm for playlist organization.

**Pseudocode**:
```
function mergeSort(songs, left, right):
    if left < right:
        mid = (left + right) / 2
        mergeSort(songs, left, mid)
        mergeSort(songs, mid + 1, right)
        merge(songs, left, mid, right)

function merge(songs, left, mid, right):
    // Create temporary arrays
    leftArray = songs[left..mid]
    rightArray = songs[mid+1..right]
    
    // Merge arrays
    i = j = 0
    k = left
    while i < leftArray.size and j < rightArray.size:
        if leftArray[i] <= rightArray[j]:
            songs[k] = leftArray[i]
            i++
        else:
            songs[k] = rightArray[j]
            j++
        k++
    
    // Copy remaining elements
    while i < leftArray.size:
        songs[k] = leftArray[i]
        i++, k++
    while j < rightArray.size:
        songs[k] = rightArray[j]
        j++, k++
```

**Time Complexity**: O(n log n)
**Space Complexity**: O(n)

### 3.2 Quick Sort Implementation

**Purpose**: In-place sorting for duration-based organization.

**Pseudocode**:
```
function quickSort(songs, low, high):
    if low < high:
        pi = partition(songs, low, high)
        quickSort(songs, low, pi - 1)
        quickSort(songs, pi + 1, high)

function partition(songs, low, high):
    pivot = songs[high]
    i = low - 1
    
    for j = low to high - 1:
        if songs[j].duration <= pivot.duration:
            i++
            swap(songs[i], songs[j])
    
    swap(songs[i + 1], songs[high])
    return i + 1
```

**Time Complexity**: O(n log n) average, O(n²) worst case
**Space Complexity**: O(log n)

### 3.3 Shuffle with Constraints Algorithm

**Purpose**: Shuffle playlist while avoiding consecutive same artists.

**Pseudocode**:
```
function shuffleWithConstraints(songs):
    maxAttempts = 100
    attempts = 0
    
    while attempts < maxAttempts:
        shuffle(songs) // Random shuffle
        
        validShuffle = true
        for i = 1 to songs.size - 1:
            if songs[i].artist == songs[i-1].artist:
                validShuffle = false
                break
        
        if validShuffle:
            return songs
        
        attempts++
    
    return songs // Return best attempt
```

**Time Complexity**: O(n²) in worst case
**Space Complexity**: O(n)

## 4. Design Patterns

### 4.1 Command Pattern - Undo System

**Purpose**: Implement reversible operations for playlist edits.

**Structure**:
```cpp
enum class ActionType { ADD, DELETE, MOVE };

struct PlaylistAction {
    ActionType type;
    Song song;
    int index1, index2;
};

class PlaylistEngine {
    std::stack<PlaylistAction> undoStack;
    
    void undoLastNEdits(int n) {
        // Pop actions and reverse them
    }
};
```

**Benefits**:
- Encapsulates operation details
- Enables easy undo/redo functionality
- Maintains operation history

### 4.2 Strategy Pattern - Sorting Algorithms

**Purpose**: Allow different sorting strategies to be used interchangeably.

**Implementation**:
```cpp
class PlaylistSorter {
    std::vector<Song> sortByTitle(const std::vector<Song>& songs);
    std::vector<Song> sortByDuration(const std::vector<Song>& songs, bool ascending);
    std::vector<Song> sortByRecentlyAdded(const std::vector<Song>& songs);
};
```

**Benefits**:
- Algorithm selection at runtime
- Easy to add new sorting strategies
- Clean separation of concerns

## 5. Memory Management

### 5.1 RAII Principles

**Resource Acquisition Is Initialization**:
- All resources are acquired in constructors
- All resources are released in destructors
- Exception-safe resource management

**Implementation**:
```cpp
class PlaylistEngine {
    PlaylistNode* head, *tail;
    
public:
    PlaylistEngine() : head(nullptr), tail(nullptr), size(0) {}
    ~PlaylistEngine() { clearList(); }
    
private:
    void clearList() {
        // Proper cleanup of all nodes
    }
};
```

### 5.2 Smart Pointer Usage

**Where Applicable**:
- Automatic memory management
- Exception safety
- No memory leaks

**Manual Memory Management**:
- Used for linked list nodes for explicit control
- Performance optimization
- Custom allocation strategies

## 6. Performance Optimization

### 6.1 Time Complexity Optimization

**Strategy**: Choose optimal data structures for specific operations.

| Operation | Data Structure | Time Complexity | Rationale |
|-----------|----------------|-----------------|-----------|
| Song Lookup | Hash Map | O(1) | Constant-time access |
| Rating Search | BST | O(log n) | Logarithmic search |
| Playlist Traversal | Linked List | O(n) | Sequential access |
| Sorting | Merge/Quick Sort | O(n log n) | Optimal comparison-based |

### 6.2 Space Complexity Optimization

**Strategy**: Minimize memory overhead while maintaining performance.

- **Linked List**: O(n) - only stores necessary pointers
- **BST**: O(n) - balanced tree structure
- **Hash Maps**: O(n) - linear space for n elements
- **Stacks**: O(n) - only stores active history

### 6.3 Caching Strategy

**Implementation**:
- Frequently accessed songs cached in hash maps
- Recent operations cached for undo
- Sorted results cached temporarily

## 7. Error Handling

### 7.1 Input Validation

**Strategy**: Validate all user inputs before processing.

```cpp
void PlaylistEngine::deleteSong(int index) {
    if (index < 0 || index >= size) {
        std::cout << "Invalid index for deletion!\n";
        return;
    }
    // Proceed with deletion
}
```

### 7.2 Bounds Checking

**Implementation**:
- Array index validation
- Linked list boundary checks
- Tree traversal safety

### 7.3 Graceful Degradation

**Strategy**: System continues operation even with errors.

- Invalid operations are ignored
- User-friendly error messages
- System state remains consistent

## 8. Testing Strategy

### 8.1 Unit Testing

**Components**:
- Individual data structure operations
- Algorithm correctness
- Memory management

### 8.2 Integration Testing

**Scenarios**:
- Cross-component interactions
- System-wide operations
- Performance benchmarks

### 8.3 Performance Testing

**Metrics**:
- Time complexity verification
- Memory usage analysis
- Scalability testing

## 9. Future Enhancements

### 9.1 Potential Improvements

1. **Persistent Storage**: Database integration for song metadata
2. **Network Features**: Remote playlist synchronization
3. **Advanced Algorithms**: Machine learning for recommendations
4. **GUI Interface**: Graphical user interface
5. **Multi-threading**: Concurrent operations support

### 9.2 Scalability Considerations

1. **Large Dataset Handling**: Optimizations for millions of songs
2. **Memory Management**: Advanced memory pooling
3. **Caching Strategy**: Multi-level caching system
4. **Load Balancing**: Distributed system architecture

## 10. Conclusion

The PlayWise system demonstrates a comprehensive understanding of data structures and algorithms through practical implementation. The modular design, efficient algorithms, and robust error handling make it suitable for educational purposes and potential real-world applications.

**Key Achievements**:
- O(1) song lookup using hash maps
- O(log n) rating-based search using BST
- O(n log n) sorting with multiple algorithms
- Robust undo system using command pattern
- Comprehensive system statistics and monitoring

**Technical Excellence**:
- Clean, modular code architecture
- Comprehensive documentation
- Performance optimization
- Memory safety and leak prevention
- Extensible design for future enhancements 