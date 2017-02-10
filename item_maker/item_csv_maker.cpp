#include "item_csv_maker.h"
#include <regex>

item_csv_maker::item_csv_maker()
{
	
}

item_csv_maker::~item_csv_maker()
{

}

void item_csv_maker::provide_begin_regex(std::string begin_regex)
{
	std::smatch regex_match;

	if (std::regex_search(source_code, regex_match, std::regex(begin_regex)))
	{
		source_code.erase(0, regex_match.prefix().str().size());
	}
}

void item_csv_maker::provide_end_regex(std::string end_regex)
{
	std::smatch regex_match;

	if (std::regex_search(source_code, regex_match, std::regex(end_regex)))
	{
		source_code.erase(source_code.size() - regex_match.suffix().str().size(), source_code.size());
	}
}

void item_csv_maker::provide_prefixes(std::string declaration_prefix, std::string function_prefix)
{
	this->declaration_prefix = std::string(declaration_prefix);
	this->function_prefix = std::string(function_prefix);
}

void item_csv_maker::provide_source_code(const std::string& source_code)
{
	this->source_code = std::string(source_code);
}

std::unordered_map<item_type, item_csv_maker::specific_type_csv> item_csv_maker::make_items_csv()
{
	auto items_csv = std::unordered_map<item_type, specific_type_csv>();
	std::smatch regex_match;

	while (std::regex_search(source_code, regex_match, std::regex(declaration_prefix + " " + function_prefix + "(.*?)\\((.*)\\)")))
	{
		items_csv[regex_match[1].str()].type_header_csv = regex_match[2];
		source_code = regex_match.suffix().str();
	}

	while (std::regex_search(source_code, regex_match, std::regex(function_prefix + "(.*?)\\((.*)\\)(?=;);$")))
	{
		items_csv[regex_match[1].str()].items_csv.push_back(regex_match[2].str());
		source_code = regex_match.suffix().str();
	}

	return items_csv;
}
