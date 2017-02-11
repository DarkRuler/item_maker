#include "refined_items_info.h"
#include <cmath>

refined_items_info::~refined_items_info()
{

}

void refined_items_info::rename_parameters(std::unordered_map<std::string, std::string> name_pairs)
{
	items_info::rename_parameters(name_pairs);
	if (name_pairs.count(recipe_type_parameter_name) != 0) recipe_type_parameter_name = name_pairs[recipe_type_parameter_name];
}

void refined_items_info::insert_special_resources_columns(std::unordered_map<std::string, std::vector<std::string>>& special_resource_map, std::string default_special_resource,
	std::string special_resource_name_column, std::string special_resource_value_column)
{
	auto _special_resource_map = special_resource_map;
	_special_resource_map.insert(_special_resource_map.end(), { default_special_resource, {} });

	this->insert_parameter_column(special_resource_name_column,
		[_special_resource_map, default_special_resource, this](std::unordered_map<std::string, std::string>& item) -> std::string
	{
		for (auto special_resource : _special_resource_map)
		{
			if (std::find(special_resource.second.begin(), special_resource.second.end(), item[this->recipe_type_parameter_name]) != special_resource.second.end()) return special_resource.first;			
		}
		return default_special_resource;
	});

	for (auto special_resource : _special_resource_map)
	{
		this->insert_parameter_column(special_resource.first,
			[special_resource, special_resource_name_column, special_resource_value_column](std::unordered_map<std::string, std::string>& item) -> std::string
		{
			if (item[special_resource_name_column] == special_resource.first) return item[special_resource_value_column];
			else return "";
		});
	}
}

void refined_items_info::insert_im_exp_per_resource_columns(std::vector<std::string> resources_columns, std::string im_exp_column)
{
	for (auto resource : resources_columns)
	{
		this->insert_parameter_column("IM exp per " + resource,
			[im_exp_column, resource](std::unordered_map<std::string, std::string>& item) -> std::string
		{
			if (item[resource] != "" && item[resource] != "0" && item[im_exp_column] != "" && item[im_exp_column] != "0") return std::to_string((double)(std::stold(item[im_exp_column]) / std::stold(item[resource])));
			else return "";
		});
	}
}

std::unordered_map<std::string, std::vector<std::string>> refined_items_info::current_special_resource_map()
{
	return
	{		
		{ "Superior Crafting Material", { "19" } },
		{ "Bacon/Broccoli", { "18" } },
		{ "Enhancer Fragment", { "17" } },
		{ "Unobtanium", { "16" } },
		{ "Chaotic Fragment of Chaos", { "15" } },
		{ "Proof of Mission", { "7", "11", "12", "13", "14" } },
		{ "Proof of Training", { "8", "9" } }		
	};
}

std::vector<std::string> refined_items_info::resources_columns_names()
{
	return
	{
		"Pixel",
		"Crafting Material",
		"Superior Crafting Material",
		"Bacon/Broccoli",
		"Enhancer Fragment",
		"Unobtanium",
		"Chaotic Fragment of Chaos",
		"Proof of Mission",
		"Proof of Training"
	};
}

std::function<bool(std::string, std::string)> refined_items_info::ascending_double_compare_functor()
{
	return std::function<bool(std::string, std::string)>([](std::string first, std::string last) -> bool
	{
		if (first == "" && last != "") return 1;
		else if (first != "" && last == "") return 0;
		else if (first == "" && last == "") return 0;

		return std::stold(first) <= std::stold(last);
	});
}

std::function<bool(std::string, std::string)> refined_items_info::descending_double_compare_functor()
{
	return std::function<bool(std::string, std::string)>([](std::string first, std::string last) -> bool
	{
		if (first == "" && last != "") return 1;
		else if (first != "" && last == "") return 0;
		else if (first == "" && last == "") return 0;

		return std::stold(first) >= std::stold(last);
	});
}
