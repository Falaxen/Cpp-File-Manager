#pragma once
#include <map>
#include <functional>
#include <string>
#include "FileManager.h"

class ConsoleInterface
{
private:
	FileManager manager;
	std::map<int, std::function<void()>>menu_actions;
	void CreateNewFileInterface();
	void CreateNewFolderInterface();
	void DeleteElementInterface();
	void ShowContentsInterface();
	void OpenFolderInterface();
	void SearchInterface();
	void SearchByFunctionInterface();
	void FastSwitchFolderInterface();
	void FastSwitchFolderUpInterface();
	void FastSwitchFolderDownInterface();
public:
	ConsoleInterface();
	void Run();
};

