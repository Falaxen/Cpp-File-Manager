#include <iostream>
#include <filesystem>
#include <memory>
#include <string>
#include "Element.h"
#include "Plik.h"
#include "Katalog.h"
#include "FileManager.h"
#include "ConsoleInterface.h"



namespace fs = std::filesystem;
int main() {
	
	ConsoleInterface konsola;
	konsola.Run();

	return 0;
}