#pragma once
#include "item_csv_maker.h"
#include "items_info.h"

class item_csv_parser
{
private:
	struct specific_type_delimited_csv
	{
		std::vector<std::string> type_header;
		std::vector<std::vector<std::string>> items_info;
	};

	static std::vector<std::string> delimit_csv_line(const std::string& csv, char delimiter = ',');
	static specific_type_delimited_csv delimit_csv_of_specific_type(const item_csv_maker::specific_type_csv& specific_type_csv);

	typedef std::unordered_map<std::string, specific_type_delimited_csv> delimited_csv;
	static delimited_csv delimit_csv(const std::unordered_map<std::string, item_csv_maker::specific_type_csv>& csv);

	std::unordered_map<std::string, item_csv_maker::specific_type_csv> csv;
public:
	enum restriction_type { whitelist, blacklist };
private:
	std::vector<std::string> restricted_parameters;	
	restriction_type _restriction_type;

	static std::vector<std::string> join_headers(const delimited_csv& csv);
	std::vector<std::string> restrict_header(const std::vector<std::string>& header);
	static std::vector<std::unordered_map<std::string, std::string>> map_parameters_of_items(const delimited_csv& delimited_csv);

public:
	item_csv_parser();
	~item_csv_parser();
	
	void provide_csv(std::unordered_map<std::string, item_csv_maker::specific_type_csv>& csv);
	void restrict_parameter_acquistion(restriction_type _restriction_type, std::vector<std::string> parameter_names);	

	items_info parse_csv();
};