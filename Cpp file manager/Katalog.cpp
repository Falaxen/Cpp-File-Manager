#include "Katalog.h"
#include <iostream>
#include <filesystem>

Katalog::Katalog(fs::path directory_path) {
	Element::Set_path(directory_path);
}

Katalog::~Katalog() {
	//std::cout << "Destruktor katalogu " << Get_path().make_preferred().string() <<"\n";
}

void Katalog::Show_info() {
	std::cout << "[KATALOG] " << Get_path().filename().string() << "\n";
}

void Katalog::Delete_element() {
	std::cout << "Usuwanie folderu " << Get_path().make_preferred().string() << "\n";
	fs::remove_all(Get_path());
}