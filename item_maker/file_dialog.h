#pragma once
#include <string>
#include <Windows.h>

class file_dialog
{
public:
	static std::string open_file();
	static void save_as(std::string data);
};