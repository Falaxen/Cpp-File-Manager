#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <filesystem>
#include <string>

#include "Element.h"
#include "Plik.h"
#include "Katalog.h"

namespace fs = std::filesystem;

class FileManager
{
private:
	fs::path working_path;
	std::vector<std::unique_ptr<Element>> list_of_elements;
public:
	FileManager();
	void OpenFolder(std::string new_path);
	void ShowContents();
	void DeleteElement(std::string name);
	void CreateNewFile(std::string name);
	void CreateNewFolder(std::string name);
	void SearchRecursive(std::string name, fs::path starting_path);
	void Search(std::string name);
	void SearchByFunction(std::string name);
	void Execute(fs::path file_path);
	void FastSwitchFolder();
	void FastSwitchFolderUp();
	void FastSwitchFolderDown();
};

