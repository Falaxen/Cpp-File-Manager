#pragma once
#include "Element.h"

class Katalog : public Element
{
public:
	Katalog(fs::path directory_path);
	~Katalog();
	void Show_info() override;
	void Delete_element() override;
};

