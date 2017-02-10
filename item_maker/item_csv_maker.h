#pragma once
#include <string>
#include <unordered_map>

typedef std::string item_type;

class item_csv_maker
{
private:
	std::string source_code;
	std::string declaration_prefix;
	std::string function_prefix;

public:
	item_csv_maker();
	~item_csv_maker();

	struct specific_type_csv
	{
		std::string type_header_csv;
		std::vector<std::string> items_csv;
	};
	
	void provide_source_code(const std::string& source_code);	
	void provide_prefixes(std::string declaration_prefix, std::string function_prefix);

	void provide_begin_regex(std::string begin_regex);
	void provide_end_regex(std::string end_regex);

	std::unordered_map<item_type, specific_type_csv> make_items_csv();
};