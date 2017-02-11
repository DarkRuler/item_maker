#pragma once
#include <string>

class io_access
{
public:
	static std::string from_file(std::string filename);	
	static void to_file(std::string filename, std::string data);	
};