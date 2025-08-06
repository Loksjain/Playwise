#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTabWidget>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QTextEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QSlider>
#include <QProgressBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QTimer>
#include <QStyle>
#include <QFont>
#include <QPalette>
#include <QIcon>
#include <QAction>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QSplitter>
#include <QScrollArea>
#include <QFrame>
#include <QHeaderView>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QChart>
#include <QChartView>
#include <QPieSeries>
#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>

#include "song.h"
#include "playlist_engine.h"
#include "playback_history.h"
#include "song_rating_tree.h"
#include "song_lookup.h"
#include "playlist_sorter.h"
#include "system_snapshot.h"

QT_CHARTS_USE_NAMESPACE

class PlayWiseGUI : public QMainWindow {
    Q_OBJECT

private:
    // Core system components
    PlaylistEngine* playlistEngine;
    PlaybackHistory* playbackHistory;
    SongRatingTree* ratingTree;
    SongLookup* songLookup;
    PlaylistSorter* sorter;
    SystemSnapshot* snapshot;

    // GUI Components
    QTabWidget* mainTabWidget;
    
    // Playlist Tab
    QListWidget* playlistWidget;
    QLineEdit* titleEdit;
    QLineEdit* artistEdit;
    QSpinBox* durationSpinBox;
    QPushButton* addSongBtn;
    QPushButton* deleteSongBtn;
    QPushButton* moveUpBtn;
    QPushButton* moveDownBtn;
    QPushButton* reverseBtn;
    QPushButton* shuffleBtn;
    
    // History Tab
    QListWidget* historyWidget;
    QPushButton* playSongBtn;
    QPushButton* undoPlayBtn;
    QPushButton* clearHistoryBtn;
    
    // Rating Tab
    QTreeWidget* ratingTreeWidget;
    QSpinBox* ratingSpinBox;
    QPushButton* addRatingBtn;
    QPushButton* searchRatingBtn;
    QListWidget* ratingResultsWidget;
    
    // Lookup Tab
    QLineEdit* searchEdit;
    QComboBox* searchTypeCombo;
    QPushButton* searchBtn;
    QListWidget* searchResultsWidget;
    QPushButton* addToLookupBtn;
    
    // Sorting Tab
    QComboBox* sortTypeCombo;
    QPushButton* sortBtn;
    QListWidget* sortedPlaylistWidget;
    QTextEdit* sortStatsText;
    
    // Undo Tab
    QSpinBox* undoCountSpinBox;
    QPushButton* undoBtn;
    QListWidget* undoHistoryWidget;
    
    // Snapshot Tab
    QTextEdit* snapshotText;
    QPushButton* generateSnapshotBtn;
    QPushButton* exportSnapshotBtn;
    QChartView* ratingChartView;
    QChartView* durationChartView;
    
    // Status and Info
    QStatusBar* statusBar;
    QLabel* songCountLabel;
    QLabel* totalDurationLabel;
    QProgressBar* memoryUsageBar;
    
    // Timer for updates
    QTimer* updateTimer;

public:
    PlayWiseGUI(QWidget* parent = nullptr) : QMainWindow(parent) {
        // Initialize core components
        playlistEngine = new PlaylistEngine();
        playbackHistory = new PlaybackHistory();
        ratingTree = new SongRatingTree();
        songLookup = new SongLookup();
        sorter = new PlaylistSorter();
        snapshot = new SystemSnapshot();
        
        // Add sample data
        initializeSampleData();
        
        // Setup GUI
        setupUI();
        setupConnections();
        setupStyling();
        
        // Start update timer
        updateTimer = new QTimer(this);
        connect(updateTimer, &QTimer::timeout, this, &PlayWiseGUI::updateDisplay);
        updateTimer->start(1000); // Update every second
        
        updateDisplay();
    }

    ~PlayWiseGUI() {
        delete playlistEngine;
        delete playbackHistory;
        delete ratingTree;
        delete songLookup;
        delete sorter;
        delete snapshot;
    }

private slots:
    void addSong() {
        QString title = titleEdit->text().trimmed();
        QString artist = artistEdit->text().trimmed();
        int duration = durationSpinBox->value();
        
        if (title.isEmpty() || artist.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please enter both title and artist!");
            return;
        }
        
        playlistEngine->addSong(title.toStdString(), artist.toStdString(), duration);
        songLookup->addSong(Song(title.toStdString(), artist.toStdString(), duration));
        
        titleEdit->clear();
        artistEdit->clear();
        durationSpinBox->setValue(0);
        
        updateDisplay();
        statusBar->showMessage("Song added successfully!", 3000);
    }
    
    void deleteSong() {
        int currentRow = playlistWidget->currentRow();
        if (currentRow >= 0) {
            playlistEngine->deleteSong(currentRow);
            updateDisplay();
            statusBar->showMessage("Song deleted successfully!", 3000);
        } else {
            QMessageBox::information(this, "Selection", "Please select a song to delete!");
        }
    }
    
    void moveSongUp() {
        int currentRow = playlistWidget->currentRow();
        if (currentRow > 0) {
            playlistEngine->moveSong(currentRow, currentRow - 1);
            updateDisplay();
            playlistWidget->setCurrentRow(currentRow - 1);
            statusBar->showMessage("Song moved up!", 2000);
        }
    }
    
    void moveSongDown() {
        int currentRow = playlistWidget->currentRow();
        if (currentRow >= 0 && currentRow < playlistWidget->count() - 1) {
            playlistEngine->moveSong(currentRow, currentRow + 1);
            updateDisplay();
            playlistWidget->setCurrentRow(currentRow + 1);
            statusBar->showMessage("Song moved down!", 2000);
        }
    }
    
    void reversePlaylist() {
        playlistEngine->reversePlaylist();
        updateDisplay();
        statusBar->showMessage("Playlist reversed!", 2000);
    }
    
    void shuffleWithConstraints() {
        playlistEngine->shuffleWithConstraints();
        updateDisplay();
        statusBar->showMessage("Playlist shuffled with constraints!", 2000);
    }
    
    void playSong() {
        int currentRow = playlistWidget->currentRow();
        if (currentRow >= 0) {
            std::vector<Song> songs = playlistEngine->getSongs();
            if (currentRow < songs.size()) {
                Song song = songs[currentRow];
                playbackHistory->addPlayedSong(song);
                updateDisplay();
                statusBar->showMessage("Now playing: " + QString::fromStdString(song.title), 3000);
            }
        } else {
            QMessageBox::information(this, "Selection", "Please select a song to play!");
        }
    }
    
    void undoPlay() {
        Song undoneSong = playbackHistory->undoLastPlay();
        if (undoneSong.title != "") {
            updateDisplay();
            statusBar->showMessage("Undid play: " + QString::fromStdString(undoneSong.title), 3000);
        } else {
            statusBar->showMessage("No songs to undo!", 2000);
        }
    }
    
    void clearHistory() {
        // Note: This would need to be implemented in PlaybackHistory class
        updateDisplay();
        statusBar->showMessage("History cleared!", 2000);
    }
    
    void addRating() {
        int currentRow = playlistWidget->currentRow();
        if (currentRow >= 0) {
            std::vector<Song> songs = playlistEngine->getSongs();
            if (currentRow < songs.size()) {
                Song song = songs[currentRow];
                int rating = ratingSpinBox->value();
                ratingTree->insertSong(song, rating);
                updateDisplay();
                statusBar->showMessage("Rating added: " + QString::fromStdString(song.title) + " - " + QString::number(rating) + " stars", 3000);
            }
        } else {
            QMessageBox::information(this, "Selection", "Please select a song to rate!");
        }
    }
    
    void searchByRating() {
        int rating = ratingSpinBox->value();
        std::vector<Song> results = ratingTree->searchByRating(rating);
        
        ratingResultsWidget->clear();
        for (const auto& song : results) {
            ratingResultsWidget->addItem(QString::fromStdString(song.title) + " - " + QString::fromStdString(song.artist));
        }
        
        statusBar->showMessage("Found " + QString::number(results.size()) + " songs with rating " + QString::number(rating), 3000);
    }
    
    void searchSong() {
        QString searchTerm = searchEdit->text().trimmed();
        if (searchTerm.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please enter a search term!");
            return;
        }
        
        searchResultsWidget->clear();
        QString searchType = searchTypeCombo->currentText();
        
        if (searchType == "By Title") {
            Song* result = songLookup->searchByTitle(searchTerm.toStdString());
            if (result) {
                searchResultsWidget->addItem(QString::fromStdString(result->title) + " - " + QString::fromStdString(result->artist));
            }
        } else if (searchType == "By ID") {
            bool ok;
            int id = searchTerm.toInt(&ok);
            if (ok) {
                Song* result = songLookup->searchById(id);
                if (result) {
                    searchResultsWidget->addItem(QString::fromStdString(result->title) + " - " + QString::fromStdString(result->artist));
                }
            }
        }
        
        if (searchResultsWidget->count() == 0) {
            searchResultsWidget->addItem("No results found");
        }
    }
    
    void sortPlaylist() {
        QString sortType = sortTypeCombo->currentText();
        std::vector<Song> songs = playlistEngine->getSongs();
        std::vector<Song> sortedSongs;
        
        auto start = std::chrono::high_resolution_clock::now();
        
        if (sortType == "By Title") {
            sortedSongs = sorter->sortByTitle(songs);
        } else if (sortType == "By Duration") {
            sortedSongs = sorter->sortByDuration(songs);
        } else if (sortType == "Recently Added") {
            sortedSongs = sorter->sortByRecentlyAdded(songs);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        sortedPlaylistWidget->clear();
        for (const auto& song : sortedSongs) {
            sortedPlaylistWidget->addItem(QString::fromStdString(song.title) + " - " + QString::fromStdString(song.artist) + " (" + QString::fromStdString(song.getFormattedDuration()) + ")");
        }
        
        sortStatsText->setText(QString("Sorting completed in %1 microseconds\nSorted %2 songs").arg(duration.count()).arg(sortedSongs.size()));
        statusBar->showMessage("Playlist sorted by " + sortType, 3000);
    }
    
    void undoLastNEdits() {
        int count = undoCountSpinBox->value();
        playlistEngine->undoLastNEdits(count);
        updateDisplay();
        statusBar->showMessage("Undid " + QString::number(count) + " edits!", 3000);
    }
    
    void generateSnapshot() {
        SystemStats stats = snapshot->exportSnapshot(*playlistEngine, *playbackHistory, *ratingTree, *songLookup);
        
        QString snapshotInfo = "=== PlayWise System Snapshot ===\n\n";
        snapshotInfo += "Playlist Statistics:\n";
        snapshotInfo += "- Total songs in playlist: " + QString::number(stats.totalSongsInPlaylist) + "\n";
        snapshotInfo += "- Total songs in database: " + QString::number(stats.totalSongsInDatabase) + "\n";
        snapshotInfo += "- Total played songs: " + QString::number(stats.totalPlayedSongs) + "\n\n";
        
        snapshotInfo += "Top 5 Longest Songs:\n";
        for (const auto& song : stats.topLongestSongs) {
            snapshotInfo += "- " + QString::fromStdString(song.title) + " (" + QString::fromStdString(song.getFormattedDuration()) + ")\n";
        }
        snapshotInfo += "\n";
        
        snapshotInfo += "Recently Played Songs:\n";
        for (const auto& song : stats.recentlyPlayed) {
            snapshotInfo += "- " + QString::fromStdString(song.title) + "\n";
        }
        snapshotInfo += "\n";
        
        snapshotInfo += "Song Count by Rating:\n";
        for (const auto& pair : stats.songCountByRating) {
            snapshotInfo += "- " + QString::number(pair.first) + " stars: " + QString::number(pair.second) + " songs\n";
        }
        
        snapshotText->setText(snapshotInfo);
        updateCharts(stats);
        statusBar->showMessage("System snapshot generated!", 3000);
    }
    
    void exportSnapshot() {
        QString fileName = QFileDialog::getSaveFileName(this, "Export Snapshot", "playwise_snapshot.txt", "Text Files (*.txt)");
        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                out << snapshotText->toPlainText();
                file.close();
                statusBar->showMessage("Snapshot exported to " + fileName, 3000);
            } else {
                QMessageBox::critical(this, "Error", "Could not save file!");
            }
        }
    }
    
    void updateDisplay() {
        // Update playlist
        playlistWidget->clear();
        std::vector<Song> songs = playlistEngine->getSongs();
        for (const auto& song : songs) {
            playlistWidget->addItem(QString::fromStdString(song.title) + " - " + QString::fromStdString(song.artist) + " (" + QString::fromStdString(song.getFormattedDuration()) + ")");
        }
        
        // Update history
        historyWidget->clear();
        std::vector<Song> history = playbackHistory->getRecentlyPlayed(10);
        for (const auto& song : history) {
            historyWidget->addItem(QString::fromStdString(song.title) + " - " + QString::fromStdString(song.artist));
        }
        
        // Update rating tree
        updateRatingTreeDisplay();
        
        // Update status
        songCountLabel->setText("Songs: " + QString::number(playlistEngine->getSize()));
        
        int totalDuration = 0;
        for (const auto& song : songs) {
            totalDuration += song.duration;
        }
        totalDurationLabel->setText("Duration: " + QString::number(totalDuration / 60) + "m " + QString::number(totalDuration % 60) + "s");
        
        // Update memory usage (simulated)
        int memoryUsage = playlistEngine->getSize() * 100; // Rough estimate
        memoryUsageBar->setValue(qMin(memoryUsage, 100));
    }
    
    void updateRatingTreeDisplay() {
        ratingTreeWidget->clear();
        std::vector<std::pair<int, int>> ratingCounts = ratingTree->getSongCountByRating();
        
        for (const auto& pair : ratingCounts) {
            QTreeWidgetItem* item = new QTreeWidgetItem(ratingTreeWidget);
            item->setText(0, QString::number(pair.first) + " Stars");
            item->setText(1, QString::number(pair.second) + " songs");
        }
    }
    
    void updateCharts(const SystemStats& stats) {
        // Rating distribution pie chart
        QPieSeries* ratingSeries = new QPieSeries();
        for (const auto& pair : stats.songCountByRating) {
            ratingSeries->append(QString::number(pair.first) + " Stars", pair.second);
        }
        
        QChart* ratingChart = new QChart();
        ratingChart->addSeries(ratingSeries);
        ratingChart->setTitle("Song Distribution by Rating");
        ratingChart->legend()->setAlignment(Qt::AlignBottom);
        
        ratingChartView->setChart(ratingChart);
        
        // Duration bar chart
        QBarSeries* durationSeries = new QBarSeries();
        QBarSet* durationSet = new QBarSet("Duration (seconds)");
        
        for (const auto& song : stats.topLongestSongs) {
            *durationSet << song.duration;
        }
        durationSeries->append(durationSet);
        
        QChart* durationChart = new QChart();
        durationChart->addSeries(durationSeries);
        durationChart->setTitle("Top Longest Songs");
        
        QBarCategoryAxis* axisX = new QBarCategoryAxis();
        QStringList categories;
        for (const auto& song : stats.topLongestSongs) {
            categories << QString::fromStdString(song.title);
        }
        axisX->append(categories);
        durationChart->addAxis(axisX, Qt::AlignBottom);
        durationSeries->attachAxis(axisX);
        
        QValueAxis* axisY = new QValueAxis();
        durationChart->addAxis(axisY, Qt::AlignLeft);
        durationSeries->attachAxis(axisY);
        
        durationChartView->setChart(durationChart);
    }

private:
    void initializeSampleData() {
        // Add sample songs to the system
        std::vector<std::tuple<std::string, std::string, int>> sampleSongs = {
            {"Bohemian Rhapsody", "Queen", 354},
            {"Hotel California", "Eagles", 391},
            {"Stairway to Heaven", "Led Zeppelin", 482},
            {"Imagine", "John Lennon", 183},
            {"Hey Jude", "The Beatles", 431},
            {"Sweet Child O' Mine", "Guns N' Roses", 356},
            {"Smells Like Teen Spirit", "Nirvana", 301},
            {"Wonderwall", "Oasis", 258},
            {"Creep", "Radiohead", 239},
            {"Zombie", "The Cranberries", 315}
        };
        
        for (const auto& songData : sampleSongs) {
            std::string title = std::get<0>(songData);
            std::string artist = std::get<1>(songData);
            int duration = std::get<2>(songData);
            
            playlistEngine->addSong(title, artist, duration);
            songLookup->addSong(Song(title, artist, duration));
            ratingTree->insertSong(Song(title, artist, duration), (rand() % 5) + 1);
        }
    }
    
    void setupUI() {
        setWindowTitle("PlayWise - Smart Music Playlist Management Engine");
        setMinimumSize(1200, 800);
        
        // Create central widget
        QWidget* centralWidget = new QWidget();
        setCentralWidget(centralWidget);
        
        // Create main layout
        QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
        
        // Create tab widget
        mainTabWidget = new QTabWidget();
        mainLayout->addWidget(mainTabWidget);
        
        // Create tabs
        createPlaylistTab();
        createHistoryTab();
        createRatingTab();
        createLookupTab();
        createSortingTab();
        createUndoTab();
        createSnapshotTab();
        
        // Create status bar
        statusBar = new QStatusBar();
        setStatusBar(statusBar);
        
        // Create status widgets
        songCountLabel = new QLabel("Songs: 0");
        totalDurationLabel = new QLabel("Duration: 0m 0s");
        memoryUsageBar = new QProgressBar();
        memoryUsageBar->setMaximum(100);
        memoryUsageBar->setValue(0);
        memoryUsageBar->setMaximumWidth(200);
        
        statusBar->addWidget(songCountLabel);
        statusBar->addWidget(totalDurationLabel);
        statusBar->addPermanentWidget(memoryUsageBar);
    }
    
    void createPlaylistTab() {
        QWidget* playlistTab = new QWidget();
        QVBoxLayout* layout = new QVBoxLayout(playlistTab);
        
        // Playlist display
        QGroupBox* playlistGroup = new QGroupBox("Current Playlist");
        QVBoxLayout* playlistLayout = new QVBoxLayout(playlistGroup);
        
        playlistWidget = new QListWidget();
        playlistLayout->addWidget(playlistWidget);
        
        // Playlist controls
        QHBoxLayout* controlsLayout = new QHBoxLayout();
        
        QPushButton* playBtn = new QPushButton("Play Selected");
        QPushButton* moveUpBtn = new QPushButton("Move Up");
        QPushButton* moveDownBtn = new QPushButton("Move Down");
        QPushButton* reverseBtn = new QPushButton("Reverse");
        QPushButton* shuffleBtn = new QPushButton("Shuffle");
        
        controlsLayout->addWidget(playBtn);
        controlsLayout->addWidget(moveUpBtn);
        controlsLayout->addWidget(moveDownBtn);
        controlsLayout->addWidget(reverseBtn);
        controlsLayout->addWidget(shuffleBtn);
        
        playlistLayout->addLayout(controlsLayout);
        layout->addWidget(playlistGroup);
        
        // Add song section
        QGroupBox* addSongGroup = new QGroupBox("Add New Song");
        QGridLayout* addSongLayout = new QGridLayout(addSongGroup);
        
        addSongLayout->addWidget(new QLabel("Title:"), 0, 0);
        titleEdit = new QLineEdit();
        addSongLayout->addWidget(titleEdit, 0, 1);
        
        addSongLayout->addWidget(new QLabel("Artist:"), 1, 0);
        artistEdit = new QLineEdit();
        addSongLayout->addWidget(artistEdit, 1, 1);
        
        addSongLayout->addWidget(new QLabel("Duration (seconds):"), 2, 0);
        durationSpinBox = new QSpinBox();
        durationSpinBox->setRange(1, 3600);
        addSongLayout->addWidget(durationSpinBox, 2, 1);
        
        addSongBtn = new QPushButton("Add Song");
        deleteSongBtn = new QPushButton("Delete Selected");
        addSongLayout->addWidget(addSongBtn, 3, 0);
        addSongLayout->addWidget(deleteSongBtn, 3, 1);
        
        layout->addWidget(addSongGroup);
        
        mainTabWidget->addTab(playlistTab, "Playlist Management");
        
        // Connect signals
        connect(playBtn, &QPushButton::clicked, this, &PlayWiseGUI::playSong);
        connect(moveUpBtn, &QPushButton::clicked, this, &PlayWiseGUI::moveSongUp);
        connect(moveDownBtn, &QPushButton::clicked, this, &PlayWiseGUI::moveSongDown);
        connect(reverseBtn, &QPushButton::clicked, this, &PlayWiseGUI::reversePlaylist);
        connect(shuffleBtn, &QPushButton::clicked, this, &PlayWiseGUI::shuffleWithConstraints);
        connect(addSongBtn, &QPushButton::clicked, this, &PlayWiseGUI::addSong);
        connect(deleteSongBtn, &QPushButton::clicked, this, &PlayWiseGUI::deleteSong);
    }
    
    void createHistoryTab() {
        QWidget* historyTab = new QWidget();
        QVBoxLayout* layout = new QVBoxLayout(historyTab);
        
        QGroupBox* historyGroup = new QGroupBox("Playback History");
        QVBoxLayout* historyLayout = new QVBoxLayout(historyGroup);
        
        historyWidget = new QListWidget();
        historyLayout->addWidget(historyWidget);
        
        QHBoxLayout* historyControls = new QHBoxLayout();
        undoPlayBtn = new QPushButton("Undo Last Play");
        clearHistoryBtn = new QPushButton("Clear History");
        
        historyControls->addWidget(undoPlayBtn);
        historyControls->addWidget(clearHistoryBtn);
        historyLayout->addLayout(historyControls);
        
        layout->addWidget(historyGroup);
        mainTabWidget->addTab(historyTab, "Playback History");
        
        connect(undoPlayBtn, &QPushButton::clicked, this, &PlayWiseGUI::undoPlay);
        connect(clearHistoryBtn, &QPushButton::clicked, this, &PlayWiseGUI::clearHistory);
    }
    
    void createRatingTab() {
        QWidget* ratingTab = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(ratingTab);
        
        // Rating tree display
        QGroupBox* treeGroup = new QGroupBox("Rating Tree");
        QVBoxLayout* treeLayout = new QVBoxLayout(treeGroup);
        
        ratingTreeWidget = new QTreeWidget();
        ratingTreeWidget->setHeaderLabels({"Rating", "Count"});
        treeLayout->addWidget(ratingTreeWidget);
        
        layout->addWidget(treeGroup);
        
        // Rating controls
        QGroupBox* controlsGroup = new QGroupBox("Rating Controls");
        QVBoxLayout* controlsLayout = new QVBoxLayout(controlsGroup);
        
        controlsLayout->addWidget(new QLabel("Rating (1-5):"));
        ratingSpinBox = new QSpinBox();
        ratingSpinBox->setRange(1, 5);
        controlsLayout->addWidget(ratingSpinBox);
        
        addRatingBtn = new QPushButton("Add Rating");
        searchRatingBtn = new QPushButton("Search by Rating");
        controlsLayout->addWidget(addRatingBtn);
        controlsLayout->addWidget(searchRatingBtn);
        
        ratingResultsWidget = new QListWidget();
        controlsLayout->addWidget(new QLabel("Search Results:"));
        controlsLayout->addWidget(ratingResultsWidget);
        
        layout->addWidget(controlsGroup);
        
        mainTabWidget->addTab(ratingTab, "Song Ratings");
        
        connect(addRatingBtn, &QPushButton::clicked, this, &PlayWiseGUI::addRating);
        connect(searchRatingBtn, &QPushButton::clicked, this, &PlayWiseGUI::searchByRating);
    }
    
    void createLookupTab() {
        QWidget* lookupTab = new QWidget();
        QVBoxLayout* layout = new QVBoxLayout(lookupTab);
        
        QGroupBox* searchGroup = new QGroupBox("Song Lookup");
        QGridLayout* searchLayout = new QGridLayout(searchGroup);
        
        searchLayout->addWidget(new QLabel("Search Term:"), 0, 0);
        searchEdit = new QLineEdit();
        searchLayout->addWidget(searchEdit, 0, 1);
        
        searchLayout->addWidget(new QLabel("Search Type:"), 1, 0);
        searchTypeCombo = new QComboBox();
        searchTypeCombo->addItems({"By Title", "By ID"});
        searchLayout->addWidget(searchTypeCombo, 1, 1);
        
        searchBtn = new QPushButton("Search");
        searchLayout->addWidget(searchBtn, 2, 0, 1, 2);
        
        searchResultsWidget = new QListWidget();
        searchLayout->addWidget(new QLabel("Results:"), 3, 0);
        searchLayout->addWidget(searchResultsWidget, 4, 0, 1, 2);
        
        layout->addWidget(searchGroup);
        mainTabWidget->addTab(lookupTab, "Song Lookup");
        
        connect(searchBtn, &QPushButton::clicked, this, &PlayWiseGUI::searchSong);
    }
    
    void createSortingTab() {
        QWidget* sortingTab = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(sortingTab);
        
        // Sorting controls
        QGroupBox* controlsGroup = new QGroupBox("Sorting Options");
        QVBoxLayout* controlsLayout = new QVBoxLayout(controlsGroup);
        
        controlsLayout->addWidget(new QLabel("Sort Type:"));
        sortTypeCombo = new QComboBox();
        sortTypeCombo->addItems({"By Title", "By Duration", "Recently Added"});
        controlsLayout->addWidget(sortTypeCombo);
        
        sortBtn = new QPushButton("Sort Playlist");
        controlsLayout->addWidget(sortBtn);
        
        sortStatsText = new QTextEdit();
        sortStatsText->setMaximumHeight(100);
        controlsLayout->addWidget(new QLabel("Sorting Statistics:"));
        controlsLayout->addWidget(sortStatsText);
        
        layout->addWidget(controlsGroup);
        
        // Sorted playlist display
        QGroupBox* sortedGroup = new QGroupBox("Sorted Playlist");
        QVBoxLayout* sortedLayout = new QVBoxLayout(sortedGroup);
        
        sortedPlaylistWidget = new QListWidget();
        sortedLayout->addWidget(sortedPlaylistWidget);
        
        layout->addWidget(sortedGroup);
        
        mainTabWidget->addTab(sortingTab, "Playlist Sorting");
        
        connect(sortBtn, &QPushButton::clicked, this, &PlayWiseGUI::sortPlaylist);
    }
    
    void createUndoTab() {
        QWidget* undoTab = new QWidget();
        QVBoxLayout* layout = new QVBoxLayout(undoTab);
        
        QGroupBox* undoGroup = new QGroupBox("Undo Operations");
        QVBoxLayout* undoLayout = new QVBoxLayout(undoGroup);
        
        undoLayout->addWidget(new QLabel("Number of edits to undo:"));
        undoCountSpinBox = new QSpinBox();
        undoCountSpinBox->setRange(1, 100);
        undoCountSpinBox->setValue(1);
        undoLayout->addWidget(undoCountSpinBox);
        
        undoBtn = new QPushButton("Undo Last N Edits");
        undoLayout->addWidget(undoBtn);
        
        undoHistoryWidget = new QListWidget();
        undoLayout->addWidget(new QLabel("Undo History:"));
        undoLayout->addWidget(undoHistoryWidget);
        
        layout->addWidget(undoGroup);
        mainTabWidget->addTab(undoTab, "Undo Operations");
        
        connect(undoBtn, &QPushButton::clicked, this, &PlayWiseGUI::undoLastNEdits);
    }
    
    void createSnapshotTab() {
        QWidget* snapshotTab = new QWidget();
        QVBoxLayout* layout = new QVBoxLayout(snapshotTab);
        
        // Snapshot controls
        QHBoxLayout* controlsLayout = new QHBoxLayout();
        generateSnapshotBtn = new QPushButton("Generate Snapshot");
        exportSnapshotBtn = new QPushButton("Export Snapshot");
        controlsLayout->addWidget(generateSnapshotBtn);
        controlsLayout->addWidget(exportSnapshotBtn);
        layout->addLayout(controlsLayout);
        
        // Snapshot display
        QSplitter* splitter = new QSplitter(Qt::Horizontal);
        
        // Text snapshot
        QGroupBox* textGroup = new QGroupBox("System Snapshot");
        QVBoxLayout* textLayout = new QVBoxLayout(textGroup);
        snapshotText = new QTextEdit();
        textLayout->addWidget(snapshotText);
        splitter->addWidget(textGroup);
        
        // Charts
        QGroupBox* chartsGroup = new QGroupBox("Analytics Charts");
        QVBoxLayout* chartsLayout = new QVBoxLayout(chartsGroup);
        
        ratingChartView = new QChartView();
        ratingChartView->setMinimumHeight(200);
        chartsLayout->addWidget(ratingChartView);
        
        durationChartView = new QChartView();
        durationChartView->setMinimumHeight(200);
        chartsLayout->addWidget(durationChartView);
        
        splitter->addWidget(chartsGroup);
        layout->addWidget(splitter);
        
        mainTabWidget->addTab(snapshotTab, "System Snapshot");
        
        connect(generateSnapshotBtn, &QPushButton::clicked, this, &PlayWiseGUI::generateSnapshot);
        connect(exportSnapshotBtn, &QPushButton::clicked, this, &PlayWiseGUI::exportSnapshot);
    }
    
    void setupConnections() {
        // Additional connections can be added here
    }
    
    void setupStyling() {
        // Set application style
        setStyleSheet(R"(
            QMainWindow {
                background-color: #f0f0f0;
            }
            QTabWidget::pane {
                border: 1px solid #c0c0c0;
                background-color: white;
            }
            QTabBar::tab {
                background-color: #e0e0e0;
                padding: 8px 16px;
                margin-right: 2px;
            }
            QTabBar::tab:selected {
                background-color: white;
                border-bottom: 2px solid #0078d4;
            }
            QGroupBox {
                font-weight: bold;
                border: 2px solid #c0c0c0;
                border-radius: 5px;
                margin-top: 10px;
                padding-top: 10px;
            }
            QGroupBox::title {
                subcontrol-origin: margin;
                left: 10px;
                padding: 0 5px 0 5px;
            }
            QPushButton {
                background-color: #0078d4;
                color: white;
                border: none;
                padding: 8px 16px;
                border-radius: 4px;
                font-weight: bold;
            }
            QPushButton:hover {
                background-color: #106ebe;
            }
            QPushButton:pressed {
                background-color: #005a9e;
            }
            QListWidget {
                border: 1px solid #c0c0c0;
                border-radius: 4px;
                background-color: white;
            }
            QListWidget::item {
                padding: 4px;
                border-bottom: 1px solid #f0f0f0;
            }
            QListWidget::item:selected {
                background-color: #0078d4;
                color: white;
            }
            QLineEdit, QSpinBox, QComboBox {
                border: 1px solid #c0c0c0;
                border-radius: 4px;
                padding: 4px;
                background-color: white;
            }
            QTextEdit {
                border: 1px solid #c0c0c0;
                border-radius: 4px;
                background-color: white;
            }
        )");
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    PlayWiseGUI window;
    window.show();
    
    return app.exec();
}

#include "gui_main.moc" 