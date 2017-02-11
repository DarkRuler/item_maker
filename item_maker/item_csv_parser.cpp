#include "item_csv_parser.h"
#include "csv_toolset.h"

#include <algorithm>
#include <functional>
#include <set>

item_csv_parser::specific_type_delimited_csv item_csv_parser::delimit_csv_of_specific_type(const item_csv_maker::specific_type_csv& specific_type_csv)
{
	std::vector<std::vector<std::string>> delimited_items_info;
	
	for (auto item_csv : specific_type_csv.items_csv)
	{
		delimited_items_info.push_back(csv_toolset::delimit_csv(item_csv.begin(), item_csv.end(), ','));
	}

	return { csv_toolset::delimit_csv(specific_type_csv.type_header_csv.begin(), specific_type_csv.type_header_csv.end(), ','), delimited_items_info };
}

item_csv_parser::delimited_csv item_csv_parser::delimit_csv(const std::unordered_map<std::string, item_csv_maker::specific_type_csv>& csv)
{
	auto delimited_csv = std::unordered_map<std::string, specific_type_delimited_csv>();

	for (auto csv_iterator : csv)
	{
		delimited_csv[csv_iterator.first] = delimit_csv_of_specific_type(csv_iterator.second);
	}

	return delimited_csv;
}

std::vector<std::string> item_csv_parser::join_headers(const item_csv_parser::delimited_csv& delimited_csv)
{
	auto uniquified_header_set = std::set<std::string>();
	for (auto specific_type_csv : delimited_csv)
	{
		for (auto parameter : specific_type_csv.second.type_header)
		{
			uniquified_header_set.insert(parameter);
		}
	}

	auto uniquified_header = std::vector<std::string>();
	uniquified_header.assign(uniquified_header_set.begin(), uniquified_header_set.end());

	return uniquified_header;
}

std::vector<std::string> item_csv_parser::restrict_header(const std::vector<std::string>& header)
{
	if (_restriction_type == restriction_type::whitelist)
	{
		return restricted_parameters;
	}
	else
	{
		auto header_copy = header;

		for (auto permittable_parameter : header)
		{
			if (std::find(restricted_parameters.begin(), restricted_parameters.end(), permittable_parameter) == restricted_parameters.end())
				header_copy.erase(std::remove(header_copy.begin(), header_copy.end(), permittable_parameter), header_copy.end());
		}

		return header_copy;
	}
}

std::vector<std::unordered_map<std::string, std::string>> item_csv_parser::map_parameters_of_items(const item_csv_parser::delimited_csv& delimited_csv)
{
	auto items_parameters_map = std::vector<std::unordered_map<std::string, std::string>>();

	for (auto specific_type_iterator : delimited_csv)
	{
		for (auto item : specific_type_iterator.second.items_info)
		{
			if (item.size() == 2) continue;

			auto item_parameter_map = std::unordered_map<std::string, std::string>();			
			for (size_t parameter_index = 0; parameter_index < item.size(); parameter_index++)
			{
				item_parameter_map[specific_type_iterator.second.type_header[parameter_index]] = item[parameter_index];
			}
			items_parameters_map.push_back(item_parameter_map);
		}
	}

	return items_parameters_map;
}

item_csv_parser::item_csv_parser()
{

}

item_csv_parser::~item_csv_parser()
{

}

void item_csv_parser::provide_csv(std::unordered_map<std::string, item_csv_maker::specific_type_csv>& csv)
{
	this->csv = csv;
}

void item_csv_parser::restrict_parameter_acquistion(item_csv_parser::restriction_type _restriction_type, std::vector<std::string> restricted_parameters)
{
	this->restricted_parameters = restricted_parameters;
	this->_restriction_type = _restriction_type;
}

items_info item_csv_parser::parse_csv()
{
	auto _delimited_csv = delimit_csv(this->csv);
	auto joined_header = restrict_header(join_headers(_delimited_csv));
	auto items_parameters_map = map_parameters_of_items(_delimited_csv);

	auto final_items_map = std::vector<std::unordered_map<std::string, std::string>>();
	for (auto item : items_parameters_map)
	{
		auto final_item = std::unordered_map<std::string, std::string>();		

		for (auto parameter_name : joined_header)
		{
			try
			{
				auto parameter_value = item.at(parameter_name);
				final_item[parameter_name] = parameter_value;
			}
			catch (std::out_of_range)
			{
				final_item[parameter_name] = "";
			}
		}

		final_items_map.push_back(final_item);
	}

	return { joined_header, final_items_map };
}
