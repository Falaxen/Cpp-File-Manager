#include <iostream>
#include "Plik.h"
namespace fs = std::filesystem;

Plik::Plik(fs::path new_file_path) {
	Element::Set_path(new_file_path);
	file_size = fs::file_size(Element::Get_path());
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