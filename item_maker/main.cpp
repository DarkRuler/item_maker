#include "io_access.h"
#include "item_csv_maker.h"
#include "item_csv_parser.h"
#include "refined_items_info.h"

int main(int argc, char** argv)
{
	std::string source_code = io_access::from_file("ATG_-_Revive.flr");

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

	refined_items_info _refined_items_info = items_info;
	_refined_items_info.rename_parameters({ {"recipeType", "Recipe Type"}, {"costPixel", "Pixel"}, {"costCraft", "Crafting Material"}, {"costSpec", "Special Resource"}, {"careerExp", "IM exp"}, {"name", "Item Name"} });	
	_refined_items_info.insert_special_resources_columns(refined_items_info::current_special_resource_map(), "Superior Crafting Material", "Special Resource Name", "Special Resource");
	_refined_items_info.remove_parameter_column("Special Resource");
	_refined_items_info.insert_im_exp_per_resource_columns(refined_items_info::resources_columns_names(), "IM exp");
	//_refined_items_info.sort("IM exp per Pixel", refined_items_info::descending_double_compare_functor());
	
	std::string csv = _refined_items_info.generate_csv('`');
	io_access::to_file("refined_items_info.csv", csv);

	return EXIT_SUCCESS;
}