#include "source_code_provider.h"
#include "item_csv_maker.h"
#include "item_csv_parser.h"

int main(int argc, char** argv)
{	
	std::string source_code = source_code_provider::from_file("ATG_-_Revive.flr");
	
	auto _item_csv_maker = item_csv_maker();
	_item_csv_maker.provide_source_code(std::move(source_code));
	_item_csv_maker.provide_prefixes("function", "addArenaCraft");
	_item_csv_maker.provide_begin_regex("function addArenaCraft");
	_item_csv_maker.provide_end_regex("setArray =");
	auto items_csv = _item_csv_maker.make_items_csv();

	auto _item_csv_parser = item_csv_parser();
	_item_csv_parser.provide_csv(std::move(items_csv));
	_item_csv_parser.restrict_parameter_acquistion(item_csv_parser::restriction_type::whitelist, { "name", "recipeType", "costPixel", "costCraft", "costSpec", "careerExp" });
	auto items_info = _item_csv_parser.parse_csv();
	
	auto refined_items_info = ::items_info(items_info);

	return EXIT_SUCCESS;
}