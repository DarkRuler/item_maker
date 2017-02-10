#include <fstream>
#include <regex>
#include <string>
#include <iostream>
#include <array>
#include <memory>
#include <unordered_map>
#include <algorithm>

inline void throw_exception(std::string error_message)
{
	throw std::exception(error_message.c_str());
}

std::string import_source_code(std::string source_code_location)
{
	std::fstream source_code;
	source_code.open(source_code_location.c_str(), std::ios::in);

	if (!source_code) throw_exception("Exception @ import_source_code: Opening source file " + source_code_location + " failed.");

	std::string source_code_text;

	source_code.seekg(0, std::ios::end);
	source_code_text.resize(source_code.tellg());
	source_code.seekg(0, std::ios::beg);

	source_code.read(&source_code_text[0], source_code_text.size());
	source_code.close();

	return source_code_text;
}

inline std::string combine_types(std::vector<std::string> type_list)
{
	std::string combined_types = "";
	for (std::string type : type_list) combined_types += type + "|";
	return combined_types.substr(0, combined_types.size() - 1);
}

void trim_by_regex(std::string& _string, std::string& initial_regex, std::string& terminal_regex)
{
	std::smatch regex_match;

	if (!std::regex_search(_string, regex_match, std::regex(initial_regex))) throw_exception("Exception @ trim_by_regex: Initial regex not found.");
	else
	{
		_string.erase(0, regex_match.prefix().str().size());
	}	

	if (!std::regex_search(_string, regex_match, std::regex(terminal_regex))) throw_exception("Exception @ trim_by_regex: Terminal regex not found.");
	else
	{
		_string.erase(_string.size() - regex_match.suffix().str().size(), _string.size());
	}
}

std::unordered_map<std::string, std::string> seek_type_definitions(std::string& source_code, const std::string& function_prefix, std::string& sought_types)
{
	auto definitions_map = std::unordered_map<std::string, std::string>();

	std::smatch regex_match;
	while (std::regex_search(source_code, regex_match, std::regex("function " + function_prefix + "(" + sought_types + ")\\((.*)\\)")))
	{
		definitions_map[regex_match[1].str()] = regex_match[2];
		source_code = regex_match.suffix().str();
	}

	return definitions_map;
}

std::unordered_map<std::string, std::vector<std::string>> seek_item_values(std::string& source_code, const std::string& function_prefix, std::string& sought_types)
{
	auto item_values_map = std::unordered_map<std::string, std::vector<std::string>>();

	std::smatch regex_match;
	while (std::regex_search(source_code, regex_match, std::regex(function_prefix + "(" + sought_types + ")\\((.*)\\)(?=;);$")))
	{
		item_values_map[regex_match[1].str()].push_back(regex_match[2].str());
		source_code = regex_match.suffix().str();
	}

	return item_values_map;
}

struct full_crafting_info_raw
{
	std::vector<std::string> item_types;
	std::unordered_map<std::string, std::string> type_definitions;
	std::unordered_map<std::string, std::vector<std::string>> item_values;
};

full_crafting_info_raw parse_source_code(std::string& source_code)
{
	const std::string function_prefix = "addArenaCraft";
	std::vector<std::string> craft_types = { "Weapon", "Armor", "Accessory", "Enhancer", "Potion", "Chip" };
	std::string combined_types = combine_types(craft_types);

	trim_by_regex(source_code, "function " + function_prefix + "(.*)\\((.*)\\)", std::string("setArray = \\[zeroItem\\];"));

	auto type_definitions = seek_type_definitions(source_code, function_prefix, combined_types);
	auto item_values = seek_item_values(source_code, function_prefix, combined_types);

	return { craft_types, type_definitions, item_values };
}

std::vector<std::string> delimit_csv(const std::string& csv, char delimiter = ',')
{
	auto delimited_values = std::vector<std::string>();
	auto temporary_string = std::string();
	
	bool is_in_string = false;

	for (auto char_iterator = csv.begin(); char_iterator != csv.end(); char_iterator++)
	{
		if (*char_iterator == '\\') char_iterator++;

		else if (*char_iterator == '\'') is_in_string = !is_in_string;

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

struct delimited_crafting_info
{
	std::vector<std::string> item_types;
	std::unordered_map<std::string, std::vector<std::string>> type_definitions;
	std::unordered_map<std::string, std::vector<std::vector<std::string>>> item_values;
};

delimited_crafting_info delimit_crafting_info(const full_crafting_info_raw& raw_crafting_info)
{
	auto delimited_type_definitions = std::unordered_map<std::string, std::vector<std::string>>();

	for (auto definition : raw_crafting_info.type_definitions)
	{
		delimited_type_definitions[definition.first] = delimit_csv(definition.second);
	}

	auto delimited_item_values = std::unordered_map<std::string, std::vector<std::vector<std::string>>>();

	for (auto type_items : raw_crafting_info.item_values)
	{
		for (auto item_csv : type_items.second)
		{
			std::vector<std::string> delimited_item_csv = delimit_csv(item_csv);
			if (delimited_item_csv.size() == 2) continue;

			delimited_item_values[type_items.first].push_back(delimited_item_csv);
		}
	}

	return { raw_crafting_info.item_types, delimited_type_definitions, delimited_item_values };
}

void seek_parameter_index_for_type(std::unordered_map<std::string, std::unordered_map<std::string, size_t>>& parameters_indices, const std::vector<std::string>& sought_parameters,
	std::string& item_type, const std::unordered_map<std::string, std::vector<std::string>>& type_definitions)
{
	const std::vector<std::string>& type_definition = type_definitions.at(item_type);

	for (size_t index = 0; index < type_definition.size(); index++)
	{
		const std::string& parameter_definition = type_definition[index];
		auto found_parameter = std::find(sought_parameters.begin(), sought_parameters.end(), parameter_definition);

		if (found_parameter != sought_parameters.end())
		{
			std::unordered_map<std::string, size_t>& type_indices = parameters_indices[item_type];
			type_indices[*found_parameter] = index;
		}
	}
}

void seek_item_values_for_type(std::vector<std::unordered_map<std::string, std::string>>& _sought_item_values, const std::unordered_map<std::string, std::unordered_map<std::string, size_t>>& parameters_indices,
	std::string& item_type, const std::unordered_map<std::string, std::vector<std::vector<std::string>>>& item_values)
{
	const std::vector<std::vector<std::string>> item_values_set = item_values.at(item_type);

	for (auto item_values : item_values_set)
	{
		auto parameter_value_map = std::unordered_map<std::string, std::string>();

		for (auto parameter_index_iterator : parameters_indices.at(item_type))
		{
			const std::string& parameter_name = parameter_index_iterator.first;
			const size_t& parameter_index = parameter_index_iterator.second;

			parameter_value_map[parameter_name] = item_values[parameter_index];
		}

		_sought_item_values.push_back(parameter_value_map);
	}
}

typedef std::vector<std::unordered_map<std::string, std::string>> sought_item_values;
sought_item_values seek_item_values(const delimited_crafting_info& crafting_info, const std::vector<std::string>& sought_parameters)
{
	auto parameters_indices = std::unordered_map<std::string, std::unordered_map<std::string, size_t>>();
	auto _refined_item_values = std::vector<std::unordered_map<std::string, std::string>>();

	for (auto item_type : crafting_info.item_types)
	{
		seek_parameter_index_for_type(parameters_indices, sought_parameters, item_type, crafting_info.type_definitions);
		seek_item_values_for_type(_refined_item_values, parameters_indices, item_type, crafting_info.item_values);	
	}

	return _refined_item_values;
}

const std::vector<std::string> list_resouce_types(std::unordered_map<std::string, std::string> resource_type_map)
{
	auto resource_types = std::vector<std::string>();

	for (auto key_value : resource_type_map)
	{
		std::string resource_name = key_value.second;
		if (std::find(resource_types.begin(), resource_types.end(), resource_name) == resource_types.end())	resource_types.push_back(key_value.second);
	}

	return resource_types;
}

typedef std::unordered_map<std::string, std::unordered_map<std::string, std::string>> refined_item_values;
refined_item_values refine_item_values(const sought_item_values& sought_values, const std::vector<std::string>& special_resource_list, const std::vector<std::string>& item_value_header, 
	const std::unordered_map<std::string, std::string>& resource_type_map, const std::string& resource_name_parameter)
{
	auto refined_item_values_set = std::unordered_map<std::string, std::unordered_map<std::string, std::string>>();

	for (auto item_value_map : sought_values)
	{
		std::string resource_type_code = item_value_map[resource_name_parameter];

		std::string special_resource_for_item;
		if (resource_type_map.find(resource_type_code) != resource_type_map.end()) special_resource_for_item = resource_type_map.at(resource_type_code);
		else special_resource_for_item = "Superior Crafting Material";

		auto refined_item_value = std::unordered_map<std::string, std::string>();
		refined_item_value.reserve(item_value_header.size());

		refined_item_value["Pixel"] = item_value_map["costPixel"];
		refined_item_value["Crafting Material"] = item_value_map["costCraft"];
		refined_item_value["IM EXP"] = item_value_map["careerExp"];

		for (auto special_resource_name : special_resource_list)
		{
			refined_item_value[special_resource_name] = "";
		}
		refined_item_value[special_resource_for_item] = item_value_map["costSpec"];

		refined_item_values_set[item_value_map["name"]] = refined_item_value;
	}

	return refined_item_values_set;
}

int main(int argc, char** argv)
{	
	int exit_code = EXIT_SUCCESS;
	
	try
	{
		std::string source_code = import_source_code("ATG_-_Revive.flr");
		full_crafting_info_raw crafting_info_raw = parse_source_code(source_code);
		delimited_crafting_info crafting_info = delimit_crafting_info(crafting_info_raw);

		const std::vector<std::string> sought_parameters = { "name", "costPixel", "costCraft", "recipeType", "costSpec", "careerExp" };
		sought_item_values sought_values = seek_item_values(crafting_info, sought_parameters);

		const std::unordered_map<std::string, std::string> resource_type_map =
		{
			{ "19", "Superior Crafting Material" },
			{ "18", "Bacon/Broccoli" },
			{ "17", "Enhancer Fragment" },
			{ "16", "Unobtainium" },
			{ "15", "Chaotic Fragment of Chaos" },
			{ "14", "Proof of Mission" },
			{ "13", "Proof of Mission" },
			{ "12", "Proof of Mission" },
			{ "11", "Proof of Mission" },
			{ "9", "Proof of Training" },
			{ "8", "Proof of Training" },
			{ "7", "Proof of Mission" }
		};

		auto special_resource_list = list_resouce_types(resource_type_map);
		auto item_value_header = special_resource_list;
		item_value_header.insert(item_value_header.begin(), { "Pixel", "Crafting Material" });
		item_value_header.insert(item_value_header.end(), { "IM EXP" });

		refined_item_values _refined_item_values = refine_item_values(sought_values, special_resource_list, item_value_header, resource_type_map, std::string("recipeType"));
	}
		
	catch (std::exception _exception)
	{
		std::cerr << _exception.what() << std::endl;
		exit_code = EXIT_FAILURE;
	}
	
	std::cin.get();
	return exit_code;
}