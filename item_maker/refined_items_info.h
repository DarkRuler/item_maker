#pragma once
#include "items_info.h"

class refined_items_info : public items_info
{
private:
	std::string recipe_type_parameter_name = "recipeType";

public:
	refined_items_info() : items_info() { }
	refined_items_info(items_info& _items_info) : items_info(_items_info) { }
	~refined_items_info();

	void rename_parameters(std::unordered_map<std::string, std::string> name_pairs);
	void insert_special_resources_columns(std::unordered_map<std::string, std::vector<std::string>>& special_resource_map, std::string default_special_resource,
		std::string special_resource_name_column, std::string special_resource_value_column);
	void insert_im_exp_per_resource_columns(std::vector<std::string> resources_columns, std::string im_exp_column);

	static std::unordered_map<std::string, std::vector<std::string>> current_special_resource_map();
	static std::vector<std::string> resources_columns_names();

	static std::function<bool(std::string, std::string)> ascending_double_compare_functor();
	static std::function<bool(std::string, std::string)> descending_double_compare_functor();
};