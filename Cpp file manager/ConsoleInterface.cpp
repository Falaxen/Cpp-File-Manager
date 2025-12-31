#include "ConsoleInterface.h"
#include <iostream>
#include <cstdlib>

ConsoleInterface::ConsoleInterface() {
	menu_actions[1] = [this]() {OpenFolderInterface();};
	menu_actions[2] = [this]() {ShowContentsInterface();};
	menu_actions[3] = [this]() {CreateNewFileInterface();};
	menu_actions[4] = [this]() {CreateNewFolderInterface();};
	menu_actions[5] = [this]() {DeleteElementInterface();};
	menu_actions[6] = [this]() {SearchInterface();};
	menu_actions[7] = [this]() {SearchByFunctionInterface();};
	menu_actions[8] = [this]() {FastSwitchFolderInterface();};
	menu_actions[9] = [this]() {FastSwitchFolderUpInterface();};
	menu_actions[10] = [this]() {FastSwitchFolderDownInterface();};
}

void ConsoleInterface::Run() {
	int option = 1;
	while (option != 0) {
		std::cout << "<-------------------------------------->\n";
		std::cout << "                 Opcje                  \n";
		std::cout << "1. Wybierz folder \n";
		std::cout << "2. Wyswietl aktualny folder \n";
		std::cout << "3. Stworz nowy plik \n";
		std::cout << "4. Stworz nowy folder \n";
		std::cout << "5. Usun plik/folder \n";
		std::cout << "6. Znajdz plik rekursywnie \n";
		std::cout << "7. Znajdz plik \n";
		std::cout << "8. Szybka zmiana folderu \n";
		std::cout << "<-------------------------------------->\n";
		

		std::cin >> option;
		std::cin.ignore(1000, '\n');

		if (menu_actions.find(option) != menu_actions.end()) {
			menu_actions[option]();
		}

		system("pause");
		system("cls");
	}
}

void ConsoleInterface::OpenFolderInterface() {
	std::string path;
	std::cout << "Podaj sciezke do folderu: \n";
	std::getline(std::cin, path);
	manager.OpenFolder(path);
}

void ConsoleInterface::ShowContentsInterface() {
	manager.ShowContents();
}

void ConsoleInterface::CreateNewFileInterface() {
	std::string name;
	std::cout << "Podaj nazwe pliku: \n";
	std::getline(std::cin, name);
	manager.CreateNewFile(name);
}

void ConsoleInterface::CreateNewFolderInterface() {
	std::string name;
	std::cout << "Podaj nazwe folderu: \n";
	std::getline(std::cin, name);
	manager.CreateNewFolder(name);
}

void ConsoleInterface::DeleteElementInterface() {
	std::string name;
	std::cout << "Podaj nazwe pliku/folderu: \n";
	std::getline(std::cin, name);
	manager.DeleteElement(name);
}

void ConsoleInterface::SearchInterface() {
	std::string name;
	std::cout << "Podaj nazwe pliku: \n";
	std::getline(std::cin, name);
	manager.Search(name);
}

void ConsoleInterface::SearchByFunctionInterface() {
	std::string name;
	std::cout << "Podaj nazwe pliku: \n";
	std::getline(std::cin, name);
	manager.SearchByFunction(name);
}

void ConsoleInterface::FastSwitchFolderInterface() {
	manager.FastSwitchFolder();
}

void ConsoleInterface::FastSwitchFolderUpInterface() {
	manager.FastSwitchFolderUp();
}

void ConsoleInterface::FastSwitchFolderDownInterface() {
	manager.FastSwitchFolderDown();
}