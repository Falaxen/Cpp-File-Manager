#pragma once
#include "Element.h"

class Plik : public Element
{
public:
    Plik(fs::path new_file_path);
    ~Plik();
    uintmax_t Get_size();
    void Show_info() override;
    void Delete_element() override;
private:
    uintmax_t file_size;
};

