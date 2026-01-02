#include "Element.h"
#include <iostream>

Element::~Element() {}

void Element::Set_path(fs::path new_file_path) {
	file_path = new_file_path;
}

fs::path Element::Get_path() {
	return file_path;
}

std::string Element::Get_name() {
	return file_path.filename().string();
}