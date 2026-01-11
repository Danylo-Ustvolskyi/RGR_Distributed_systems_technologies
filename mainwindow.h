#pragma once

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QFutureWatcher>

#include "searchengine.h"

// The MainWindow class provides a simple graphical user interface for
// specifying a directory and search query and presenting the results. It
// leverages QtConcurrent to run the file search in a background thread
// while keeping the GUI responsive. Once the search completes, the
// results are displayed in a QListWidget.

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void selectDirectory();
    void startSearch();
    void handleSearchFinished();

private:
    QLineEdit* searchLineEdit;
    QLineEdit* directoryLineEdit;
    QPushButton* browseButton;
    QPushButton* searchButton;
    QListWidget* resultList;
    QFutureWatcher<std::vector<SearchResult>> watcher;
};