#pragma once
#include <string>	
#include <vector>

class csv_toolset
{
public:
	static std::vector<std::string> delimit_csv(std::string::const_iterator csv_line_begin, std::string::const_iterator csv_line_end, char delimiter);
	static std::string generate_csv(std::vector<std::string>::const_iterator delimited_csv_begin, std::vector<std::string>::const_iterator delimited_csv_end, char delimiter);
};