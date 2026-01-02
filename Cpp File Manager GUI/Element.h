#pragma once
#include <filesystem>
namespace fs = std::filesystem;

class Element
{
public:
	virtual ~Element();
	fs::path Get_path();
	void Set_path(fs::path new_file_path);
	virtual void Show_info() = 0;
	virtual void Delete_element() = 0;
	std::string Get_name();
private:
	fs::path file_path;
};

