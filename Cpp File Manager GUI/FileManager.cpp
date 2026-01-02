#include "FileManager.h"
#include <fstream>
#include <Windows.h>

const std::vector<std::unique_ptr<Element>>& FileManager::Get_elements() const {
	return list_of_elements;
}

FileManager::FileManager() {
	std::string system_drive = "C:\\";
	char* pValue = nullptr;
	size_t len = 0;
	_dupenv_s(&pValue, &len, "SystemDrive");
	if (pValue != nullptr) {
		system_drive = std::string(pValue) + "\\";
		free(pValue);
	}
	std::vector<std::string> available_drives;
	unsigned long maska = GetLogicalDrives();

	for (int i = 0; i < 26; ++i) {
		if (maska & (1 << i)) {
			std::string sciezka = std::string(1, 'A' + i) + ":\\";
			if (GetDriveTypeA(sciezka.c_str()) == DRIVE_FIXED) {
				available_drives.push_back(sciezka);
			}
		}
	}

	std::string default_drive = system_drive;

	for (const auto& d : available_drives) {
		if (d != system_drive) {
			default_drive = d;
			break;
		}
	}

	this->working_path = default_drive;

	OpenFolder(working_path.string());
}

fs::path FileManager::GetWorkingPath() {
	return working_path;
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
		return;
	}

	fs::create_directory(creating_path);
	list_of_elements.push_back(std::make_unique<Katalog>(creating_path));
}

void FileManager::SearchRecursive(std::string name, fs::path starting_path, std::vector<fs::path>& found_paths) {
	std::error_code ec;
	auto it = fs::directory_iterator(starting_path, ec);

	if (ec) return;

	while (it != fs::directory_iterator()) {
		std::error_code ec_loop;

		try {
			fs::path current_path = it->path();
			std::string current_filename = current_path.filename().string();

			if (current_filename.find(name) != std::string::npos) {
				found_paths.push_back(current_path);
			}

			std::error_code ec_dir;
			fs::file_status s = fs::symlink_status(current_path, ec_dir);

			bool is_dir = (!ec_dir && fs::is_directory(s));

			it.increment(ec_loop);

			if (is_dir) {
				SearchRecursive(name, current_path, found_paths);
			}
		}
		catch (...) {
			it.increment(ec_loop);
		}

		if (ec_loop) break;
	}
}

void FileManager::Search(std::string name) {
	std::vector<fs::path> found_paths;
	list_of_elements.clear();
	SearchRecursive(name, working_path, found_paths);

	for (const auto& p : found_paths) {
		std::error_code ec;
		if (fs::is_directory(p, ec)) {
			list_of_elements.push_back(std::make_unique<Katalog>(p));
		}
		else {
			list_of_elements.push_back(std::make_unique<Plik>(p));
		}
	}

}

void FileManager::SearchByFunction(std::string name) {
	std::vector<std::unique_ptr<Element>> found_elements;
	try {
		std::error_code ec;
		for (auto it = fs::recursive_directory_iterator(working_path, fs::directory_options::skip_permission_denied, ec);
			it != fs::recursive_directory_iterator();
			it.increment(ec)) {

			if (ec) {
				std::cerr << "B³¹d dostêpu: " << ec.message() << std::endl;
				ec.clear();
				continue;
			}

			try {
				const auto& entry = *it;
				std::string filename = entry.path().filename().string();

				if (filename.find(name) != std::string::npos) {
					if (entry.is_directory()) {
						found_elements.push_back(std::make_unique<Katalog>(entry.path()));
					}
					else if (entry.is_regular_file()) {
						found_elements.push_back(std::make_unique<Plik>(entry.path()));
					}
				}
			}
			catch (const std::exception& e) {
				continue;
			}
		}
	}
	catch (fs::filesystem_error& e) {
		std::cout << "ERROR: " << e.what() << "\n";
		return;
	}

	if (found_elements.empty()) {
		std::cout << "Nie znaleziono zadnych wynikow.\n";
		return;
	}

	for (int i = 0; i < found_elements.size(); i++) {
		std::cout << "[" << i + 1 << "] ";
		found_elements[i]->Show_info();
	}
	int choice;
	std::cout << "Wybierz szukany plik/folder \n";
	std::cin >> choice;
	if (choice >= 1 && choice <= found_elements.size()) {
		Execute(found_elements[choice - 1]->Get_path());
	}
	else {
		std::cout << "Nieprawidlowy numer wyboru!\n";
	}
}

void FileManager::Execute(fs::path file_path) {
	ShellExecuteA(NULL, "open", file_path.string().c_str(), NULL, NULL, SW_SHOW);
}

void FileManager::Execute2(fs::path file_path) {
	ShellExecuteA(NULL, "explore", file_path.string().c_str(), NULL, NULL, SW_SHOW);
}

void FileManager::FastSwitchFolder() {
	std::string dwa;
	ShowContents();
	std::cout << "Gdzie chcesz przejsc? \n";
	std::cout << "Folder  wyzej -> .. | Folder nizej -> nazwa \n";
	std::getline(std::cin, dwa);
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
			std::cout << "ERROR: " << e.what() << "\n";
			return;
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
	std::getline(std::cin, dwa);
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