#include "csv_toolset.h"

std::vector<std::string> csv_toolset::delimit_csv(std::string::const_iterator csv_line_begin, std::string::const_iterator csv_line_end, char delimiter)
{
	auto delimited_values = std::vector<std::string>();
	auto temporary_string = std::string();

	bool is_in_string = false;

	for (auto char_iterator = csv_line_begin; char_iterator != csv_line_end; char_iterator++)
	{
		if (*char_iterator == '\\') char_iterator++;

		else if (*char_iterator == '\'' || *char_iterator == '\"') is_in_string = !is_in_string;

		else if (*char_iterator == delimiter && !is_in_string)
		{
			delimited_values.push_back(temporary_string);
			temporary_string.clear();
		}

		else if (*char_iterator == ' ' && !is_in_string) continue;

		else temporary_string += *char_iterator;
	}

	delimited_values.push_back(temporary_string);

	return delimited_values;
}

std::string csv_toolset::generate_csv(std::vector<std::string>::const_iterator delimited_csv_begin, std::vector<std::string>::const_iterator delimited_csv_end, char delimiter)
{
	std::string csv;

	for (auto value_iterator = delimited_csv_begin; value_iterator != delimited_csv_end; value_iterator++)
	{
		csv += *value_iterator + delimiter;
	}

	csv.erase(csv.end());
	return csv;
}
