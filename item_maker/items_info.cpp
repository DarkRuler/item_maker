#include "items_info.h"
#include "csv_toolset.h"
#include <algorithm>

items_info::items_info()
{

}

items_info::~items_info()
{

}

const items_info items_info::operator+(items_info rvalue)
{
	auto result_items_header = items_header;
	result_items_header.insert(result_items_header.end(), rvalue.items_header.begin(), rvalue.items_header.end());

	auto result_items = items;
	result_items.insert(result_items.end(), rvalue.items.begin(), rvalue.items.end());

	return { result_items_header, result_items };
}

void items_info::rename_parameters(std::unordered_map<std::string, std::string> name_pairs)
{
	auto matched_names = std::vector<std::string>();

	for (auto pair : name_pairs)
	{
		if (std::find(items_header.begin(), items_header.end(), pair.first) != items_header.end())
		{
			std::replace(items_header.begin(), items_header.end(), pair.first, pair.second);
			matched_names.push_back(pair.first);
		}
	}

	for (auto& item : items)
	{
		for (auto name : matched_names)
		{
			std::string parameter_value = item[name];
			item.erase(name);
			item[name_pairs[name]] = parameter_value;
		}
	}
}

void items_info::insert_parameter_column(std::string name, std::function<std::string(std::unordered_map<std::string, std::string>&)> fill_functor)
{
	items_header.push_back(name);

	for (auto& item : items)
	{
		item[name] = fill_functor(item);
	}
}

void items_info::remove_parameter_column(std::string name)
{
	items_header.erase(std::remove(items_header.begin(), items_header.end(), name), items_header.end());

	for (auto& item : items)
	{
		item.erase(name);
	}
}

void items_info::sort(std::string sorted_values_column, std::function<bool(std::string, std::string)> compare_functor)
{	
	std::sort(items.begin(), items.end(), [compare_functor, sorted_values_column](auto& first, auto& last) -> bool
	{
		return compare_functor(first[sorted_values_column], last[sorted_values_column]);
	});
}

std::string items_info::generate_csv(char delimiter)
{
	std::string items_info_csv = "";

	items_info_csv += csv_toolset::generate_csv(items_header.begin(), items_header.end(), delimiter) + '\n';

	for (auto item : items)
	{
		auto item_values = std::vector<std::string>();
		for (auto value_name : items_header) item_values.push_back(item[value_name]);		

		items_info_csv += csv_toolset::generate_csv(item_values.begin(), item_values.end(), delimiter) + '\n';
	}

	return items_info_csv;
}