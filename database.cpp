#include "database.h"
#include "ui_database.h"
#include "demo.h"  // Include Demo header file

#include <QMessageBox>
#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QMimeData>

Database::Database(Demo *demo, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Database)
    , _demo(demo)  // Initialize Demo pointer
{
    ui->setupUi(this);
    setAcceptDrops(true);  // Enable drag-and-drop for the dialog

    // Set up the database folder path
    QDir appDir(QCoreApplication::applicationDirPath());
    appDir.cdUp();  // Go one level up
    databaseFolderPath = appDir.absolutePath() + "/database";

    // Create the database folder if it doesn't exist
    QDir().mkpath(databaseFolderPath);

    // Load files into the list widget
    loadFilesIntoList();
}

Database::~Database()
{
    delete ui;
}

// Helper function to load files into the QListWidget
void Database::loadFilesIntoList()
{
    ui->availableFiles->clear();  // Clear the list first
    QDir dir(databaseFolderPath);
    QStringList files = dir.entryList(QDir::Files);
    foreach (const QString &fileName, files) {
        ui->availableFiles->addItem(fileName);  // Add each file to the list widget
    }
}

// Handle drag-enter event
void Database::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();  // Accept the drag event
    }
}

// Handle drop event
void Database::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        QList<QUrl> urlList = mimeData->urls();
        if (!urlList.isEmpty()) {
            tempFilePath = urlList.first().toLocalFile();
            ui->AddFile->setText(tempFilePath);  // Show the file path in the QLineEdit
        }
    }
}

// Handle the "Add File" button
void Database::on_Addbutton_clicked()
{
    if (tempFilePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No file selected. Drag and drop a file first.");
        return;
    }

    QFileInfo fileInfo(tempFilePath);
    QString targetFilePath = databaseFolderPath + "/" + fileInfo.fileName();

    QFile file(tempFilePath);
    if (file.copy(targetFilePath)) {
        QMessageBox::information(this, "Success", "File added to database.");
        loadFilesIntoList();  // Refresh the list
        tempFilePath.clear();  // Clear the temp file path
        ui->AddFile->clear();  // Clear the QLineEdit
    } else {
        QMessageBox::warning(this, "Error", "Failed to add file. It may already exist.");
    }
}

// Handle the "Remove File" button
void Database::on_removeButton_clicked()
{
    QListWidgetItem *selectedItem = ui->availableFiles->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "Error", "No file selected. Select a file to remove.");
        return;
    }

    QString fileName = selectedItem->text();
    QString filePath = databaseFolderPath + "/" + fileName;

    if (QFile::remove(filePath)) {
        QMessageBox::information(this, "Success", "File removed.");
        loadFilesIntoList();  // Refresh the list
    } else {
        QMessageBox::warning(this, "Error", "Failed to remove file.");
    }
}

// Handle the "BACK" button
void Database::on_pushButton_clicked()
{
    this->close();  // Close the dialog
}

// Handle the "back" button (to return to the parent window)
void Database::on_back_clicked()
{
    this->hide();      // Hide the current window (Database)
    _demo->show();     // Show the Demo window again
}
