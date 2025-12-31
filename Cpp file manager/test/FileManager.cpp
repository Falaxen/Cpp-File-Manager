#include "FileManager.h"
#include <fstream>
#include <Windows.h>

FileManager::FileManager() {
	working_path = "D:/";
	OpenFolder(working_path.string());
}

void FileManager::OpenFolder(std::string new_path)
 {
	 working_path = new_path;

	 list_of_elements.clear();

	 if (!fs::exists(working_path)) {
		 std::cout << "ERROR: Nie istniejaca sciezka! \n";
		 return;
	 }

	 for (auto entry : fs::directory_iterator(working_path)) {
		 if (entry.is_directory() == true) {
			 list_of_elements.push_back(std::make_unique<Katalog>(entry.path()));
		 }
		 else {
			 list_of_elements.push_back(std::make_unique<Plik>(entry.path()));
		 }
	 }

	 std::cout << "Wczytano: " << working_path << "\n";
	}

void FileManager::ShowContents() {
	if (list_of_elements.empty()) {
		std::cout << "Katalog jest pusty / Brak katalogu \n";
		return;
	}
	std::cout << "\n-------------- KATALOG --------------\n \n";
	std::cout << working_path.make_preferred().string();
	std::cout << "\n \n------------- ZAWARTOSC -------------\n";
	for (const auto& entry : list_of_elements) {
		entry->Show_info();
	}
	std::cout << "-------------------------------------\n";
}

void FileManager::DeleteElement(std::string name) {
	for (auto element = list_of_elements.begin(); element != list_of_elements.end(); ++element) {
		if ((*element)->Get_path().filename().string() == name) {
			try {
				(*element)->Delete_element();
			}
			catch (const std::exception& e) {
				std::cout << "ERROR: " << e.what() << "\n";
				return;
			}
			list_of_elements.erase(element);
			return;
		}
	}
	std::cout << "Nie znaleziono " << name << "\n";
}

void FileManager::CreateNewFile(std::string name) {
	fs::path creating_path;
	creating_path = working_path / name;

	if (fs::exists(creating_path)) {
		std::cout << "Taki plik juz istnieje! \n";
		return;
	}

	std::ofstream file(creating_path);

	if (fs::exists(creating_path)) {
		std::cout << "Utworzono plik: " << name << "\n";
		list_of_elements.push_back(std::make_unique<Plik>(creating_path));
	}
	else {
		std::cout << "ERROR: Tworzenie pliku nie powiodlo sie! \n";
	}
}

void FileManager::CreateNewFolder(std::string name) {
	fs::path creating_path = working_path / name;

	if (fs::exists(creating_path)) {
		std::cout << "Taki katalog juz istnieje! \n";
		return;
	}

	fs::create_directory(creating_path);

	if (fs::exists(creating_path)) {
		std::cout << "Utworzono katalog: " << name << "\n";
		list_of_elements.push_back(std::make_unique<Katalog>(creating_path));
	}
	else {
		std::cout << "ERROR: Tworzenie katalogu nie powiodlo sie! \n";
	}
}

void FileManager::SearchRecursive(std::string name, fs::path starting_path) {
	try {
		for (auto entry : fs::directory_iterator(starting_path)) {
			if (entry.path().filename() == name) {
				std::cout << "Znaleziono plik: " << name << "\nSciezka do pliku: " << entry.path() << "\n";
				OpenFolder(entry.path().parent_path().string());
				Execute(entry.path());
				return;
			}
			else if (entry.is_directory() == true) {
				SearchRecursive(name, entry.path());
			}
		}
	}
	catch(fs::filesystem_error& e){
		std::cout << "ERROR: " << e.what() << "\n";
		return;
	}
}

void FileManager::Search(std::string name) {
	std::cout << "Szukam " << name << " w " << working_path << "... \n";
	SearchRecursive(name, working_path);
}

void FileManager::SearchByFunction(std::string name) {
	for (const auto& entry : fs::recursive_directory_iterator(working_path, fs::directory_options::skip_permission_denied)) {
		if (entry.path().filename() == name) {
			std::cout << "Znaleziono plik: " << name << "\nSciezka do pliku: " << entry.path() << "\n";
			OpenFolder(entry.path().parent_path().string());
			Execute(entry.path());
		}
	}
}

void FileManager::Execute(fs::path file_path) {
	int wybor  = 2;
	std::cout << "Co chcesz zrobiæ? :\n";
	std::cout << "1. Uruchomic plik \n";
	std::cout << "2. Otworzyc eksplorator plikow \n";
	std::cin >> wybor;
	std::cin.ignore();
	switch (wybor) {
	case 1:
		ShellExecuteA(NULL, "open", file_path.string().c_str(), NULL, NULL, SW_SHOW);
		break;
	case 2:
		ShellExecuteA(NULL, "explore", file_path.parent_path().string().c_str(), NULL, NULL, SW_SHOW);
		break;
	default:
		return;
	}
}

void FileManager::FastSwitchFolder() {
	std::string dwa;
	ShowContents();
	std::cout << "Gdzie chcesz przejsc? \n";
	std::cout << "Folder  wyzej -> .. | Folder nizej -> nazwa \n";
	std::cin >> dwa;
	std::cin.ignore();
	if (dwa == "..") {
		working_path = working_path.parent_path();
		try {
			OpenFolder(working_path.string());
		}
		catch (fs::filesystem_error& e) {
			std::cout << "ERROR: " << e.what() << "\n";
			return;
		}
	}
	else {
		working_path = working_path / dwa;
		try {
			OpenFolder(working_path.string());
		}
		catch (fs::filesystem_error& e) {
		}
	}
}

void FileManager::FastSwitchFolderUp() {
	working_path = working_path.parent_path();
	try {
		OpenFolder(working_path.string());
	}
	catch (fs::filesystem_error& e) {
		std::cout << "ERROR: " << e.what() << "\n";
		return;
	}
}

void FileManager::FastSwitchFolderDown() {
	std::string dwa;
	ShowContents();
	std::cout << "Gdzie chcesz przejsc? \n";
	std::cout << "Folder  wyzej -> .. | Folder nizej -> nazwa \n";
	std::cin >> dwa;
	std::cin.ignore();
	working_path = working_path / dwa;
	try {
		OpenFolder(working_path.string());
	}
	catch (fs::filesystem_error& e) {
		std::cout << "ERROR: " << e.what() << "\n";
		return;
	}
}