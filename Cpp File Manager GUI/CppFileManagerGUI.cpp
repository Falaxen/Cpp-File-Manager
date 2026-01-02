#include "CppFileManagerGUI.h"
#include "qfileiconprovider.h"
#include "Element.h"
#include <QLineEdit>
#include <QInputDialog>
#include <QMessageBox>

CppFileManagerGUI::CppFileManagerGUI(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.createFolderButton->setIcon(style()->standardIcon(QStyle::SP_DirIcon));
    ui.createFileButton->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
    ui.deleteButton->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton));
    connect(ui.pathLineEdit, &QLineEdit::returnPressed, this, &CppFileManagerGUI::onPathChanged);
    connect(ui.fileList, &QListWidget::itemDoubleClicked, this, &CppFileManagerGUI::onElementDoubleClicked);
    connect(ui.fileLineEdit, &QLineEdit::returnPressed, this, &CppFileManagerGUI::onSearchRequested);
    connect(ui.createFolderButton, &QPushButton::clicked, this, &CppFileManagerGUI::onCreateFolderClicked);
    connect(ui.createFileButton, &QPushButton::clicked, this, &CppFileManagerGUI::onCreateFileClicked);
    connect(ui.deleteButton, &QPushButton::clicked, this, &CppFileManagerGUI::onDeleteClicked);
    connect(ui.openFolder, &QPushButton::clicked, this, &CppFileManagerGUI::onOpenFolderClicked);
    refreshFilelist();
}

CppFileManagerGUI::~CppFileManagerGUI()
{}

void CppFileManagerGUI::refreshFilelist() {

    ui.fileList->clear();

    ui.pathLineEdit->setText(QString::fromStdWString(manager.GetWorkingPath().wstring()));
    
    ui.fileList->addItem("..");

    const auto& elements = manager.Get_elements();

    QFileIconProvider iconProvider;

    for (const auto& el : elements) {
        QListWidgetItem* item = new QListWidgetItem(
            QString::fromStdWString(el->Get_path().filename().wstring()),ui.fileList);

        if (dynamic_cast<Katalog*>(el.get())) {
            item->setIcon(iconProvider.icon(QFileIconProvider::Folder));
        }
        else {
            item->setIcon(iconProvider.icon(QFileIconProvider::File));
        }
    }
}

void CppFileManagerGUI::onElementDoubleClicked(QListWidgetItem* item) {
    int row = ui.fileList->row(item);

    if (item->text() == "..") {
        if (isSearching) {
            isSearching = false;
            manager.OpenFolder(manager.GetWorkingPath().string());
            refreshFilelist();
        }
        else {
            manager.FastSwitchFolderUp();
            refreshFilelist();
        }
        return;
    }

    auto& clicked = manager.Get_elements()[row-1];

    if (Katalog* kat = dynamic_cast<Katalog*>(clicked.get())) {
        manager.OpenFolder(kat -> Get_path().string());
        refreshFilelist();
    }
    else {
        manager.Execute(clicked -> Get_path());
        refreshFilelist();
    }
}

void CppFileManagerGUI::onPathChanged() {
    std::string newPath = ui.pathLineEdit->text().toStdString();
    isSearching = false;

    try {
        manager.OpenFolder(newPath);
        refreshFilelist();
    }
    catch (...) {
        ui.pathLineEdit->setText(QString::fromStdWString(manager.GetWorkingPath().wstring()));
    }
}

void CppFileManagerGUI::onSearchRequested() {
    std::string searched = ui.fileLineEdit->text().toStdString();

    if (searched.empty()) {
        isSearching = false;
        refreshFilelist();
        return;
    }

    manager.Search(searched);
    isSearching = true;
    
    refreshFilelist();
}

void CppFileManagerGUI::onCreateFolderClicked() {
    bool ok;
    QString name = QInputDialog::getText(this, "Nowy folder", "Podaj nazwê folderu: ", QLineEdit::Normal, "Nowy folder", &ok);

    if (ok && !name.isEmpty()) {
        manager.CreateNewFolder(name.toStdString());
        refreshFilelist();
    }
}

void CppFileManagerGUI::onCreateFileClicked() {
    bool ok;
    QString name = QInputDialog::getText(this, "Nowy plik", "Podaj nazwê pliku: ", QLineEdit::Normal, "Nowy plik", &ok);

    if (ok && !name.isEmpty()) {
        manager.CreateNewFile(name.toStdString());
        refreshFilelist();
    }
}

void CppFileManagerGUI::onDeleteClicked() {
    QListWidgetItem* item = ui.fileList->currentItem();
    if (!item) return;

    std::string name = item->text().toStdString();

    if (name == "..") {
        QMessageBox::warning(this, "ERROR", "Nie mozna usunac folderu nadrzednego!");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Potwierdzenie", QString("Czy na pewno chcesz usunac: %1").arg(item->text()), QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        manager.DeleteElement(name);
        refreshFilelist();
    }
    else { return; }
}

void CppFileManagerGUI::onOpenFolderClicked() {
    manager.Execute2(manager.GetWorkingPath());
}