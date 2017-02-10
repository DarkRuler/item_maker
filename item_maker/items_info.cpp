#include "items_info.h"

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
			matched_names.push_back(pair.first);
		}
	}

	for (auto item : items)
	{
		for (auto name : matched_names)
		{
			std::string parameter_value = item[name];
			item.erase(name);
			item[name_pairs[name]] = parameter_value;
		}
	}
}

void items_info::insert_parameter_column(std::string name, std::function<std::string(std::unordered_map<std::string, std::string>)> fill_functor)
{
	items_header.push_back(name);

	for (auto item : items)
	{
		item[name] = fill_functor(item);
	}
}
