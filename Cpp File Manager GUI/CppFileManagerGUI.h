#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CppFileManagerGUI.h"
#include "FileManager.h"

class CppFileManagerGUI : public QMainWindow
{
    Q_OBJECT

public:
    CppFileManagerGUI(QWidget *parent = nullptr);
    ~CppFileManagerGUI();

private:
    Ui::CppFileManagerGUIClass ui;
    FileManager manager;
    void refreshFilelist();
    void onElementDoubleClicked(QListWidgetItem* item);
    void onPathChanged();
    bool isSearching = false;
    void onCreateFolderClicked();
    void onCreateFileClicked();
    void onSearchRequested();
    void onDeleteClicked();
    void onOpenFolderClicked();
};

