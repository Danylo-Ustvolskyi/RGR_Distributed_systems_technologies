#include "mainwindow.h"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QtConcurrent>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , searchLineEdit(new QLineEdit)
    , directoryLineEdit(new QLineEdit)
    , browseButton(new QPushButton(tr("Select Folder")))
    , searchButton(new QPushButton(tr("Start Search")))
    , resultList(new QListWidget)
{
    // Set up the GUI layout. We use a horizontal layout for the top
    // control bar containing the search query input, directory display,
    // browse button and search button. Below that we place the results
    // list in a vertical layout.
    QWidget* central = new QWidget;
    auto* mainLayout = new QVBoxLayout;
    auto* controlLayout = new QHBoxLayout;

    searchLineEdit->setPlaceholderText(tr("Enter word or phrase"));
    directoryLineEdit->setReadOnly(true);

    controlLayout->addWidget(searchLineEdit, /*stretch=*/2);
    controlLayout->addWidget(directoryLineEdit, /*stretch=*/3);
    controlLayout->addWidget(browseButton);
    controlLayout->addWidget(searchButton);

    mainLayout->addLayout(controlLayout);
    mainLayout->addWidget(resultList);
    central->setLayout(mainLayout);
    setCentralWidget(central);

    setWindowTitle(tr("Multithreaded Text Search"));

    // Connect signals to slots. When the browse button is clicked the user
    // can choose a directory. When the search button is clicked we
    // initiate the background search. When the QFutureWatcher signals
    // finished we update the UI with results.
    connect(browseButton, &QPushButton::clicked,
            this, &MainWindow::selectDirectory);
    connect(searchButton, &QPushButton::clicked,
            this, &MainWindow::startSearch);
    connect(&watcher, &QFutureWatcher<std::vector<SearchResult>>::finished,
            this, &MainWindow::handleSearchFinished);
}

void MainWindow::selectDirectory() {
    const QString dir = QFileDialog::getExistingDirectory(this, tr("Select Directory"));
    if (!dir.isEmpty()) {
        directoryLineEdit->setText(dir);
    }
}

void MainWindow::startSearch() {
    resultList->clear();
    const QString dir = directoryLineEdit->text();
    const QString query = searchLineEdit->text();
    if (dir.isEmpty() || query.isEmpty()) {
        QMessageBox::warning(this, tr("Missing Information"),
                             tr("Please select a directory and enter a search query."));
        return;
    }
    // Disable controls to prevent concurrent searches and directory changes
    browseButton->setEnabled(false);
    searchButton->setEnabled(false);

    // Launch the search engine in a separate thread using QtConcurrent.
    // The lambda captures the directory and query by value. Note that
    // SearchEngine::search is a blocking call. The future watcher will
    // notify us when it completes.
    auto future = QtConcurrent::run([dir, query]() {
        return SearchEngine::search(dir.toStdString(), query.toStdString());
    });
    watcher.setFuture(future);
}

void MainWindow::handleSearchFinished() {
    // Retrieve the results from the future watcher
    const auto results = watcher.result();
    for (const auto& res : results) {
        const QString itemText = QString::fromStdString(res.filePath) +
                                 " (" + QString::number(res.lineNumber) + "): " +
                                 QString::fromStdString(res.lineText);
        resultList->addItem(itemText);
    }
    // Re‑enable controls after search completes
    browseButton->setEnabled(true);
    searchButton->setEnabled(true);
}