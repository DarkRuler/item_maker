#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>

class items_info
{
private:

public:
	items_info();
	~items_info();

	std::vector<std::string> items_header;
	std::vector<std::unordered_map<std::string, std::string>> items;

	items_info(std::vector<std::string> _items_header, std::vector<std::unordered_map<std::string, std::string>> _items) : items_header(_items_header), items(_items) { }
	const items_info operator+(items_info rvalue);

	void rename_parameters(std::unordered_map<std::string, std::string> name_pairs);
	void insert_parameter_column(std::string name, std::function<std::string(std::unordered_map<std::string, std::string>&)> fill_functor);
	void remove_parameter_column(std::string name);

	void sort(std::string sorted_values_column, std::function<bool(std::string, std::string)> compare_functor);

	std::string generate_csv(char delimiter);
};