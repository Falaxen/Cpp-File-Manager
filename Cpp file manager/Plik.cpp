#include <iostream>
#include "Plik.h"
namespace fs = std::filesystem;

Plik::Plik(fs::path new_file_path) {
    Element::Set_path(new_file_path);
    try {
        this->file_size = fs::file_size(new_file_path);
    }
    catch (const fs::filesystem_error& e) {
        this->file_size = 0;
    }
}

Plik::~Plik() {
	//std::cout << "Destruktor pliku: " << Get_path().make_preferred().string() << "\n";
}

uintmax_t Plik::Get_size() {
	return file_size;
}

void Plik::Show_info() {
	std::cout << "          " << Get_path().filename().string() << " - " << Get_size() << " bytes \n";
}

void Plik::Delete_element() {
	std::cout << "Usuwanie pliku " << Get_path().make_preferred().string() << "\n";
	fs::remove(Get_path());
}