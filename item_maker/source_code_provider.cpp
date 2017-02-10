#include "source_code_provider.h"
#include <fstream>

std::string source_code_provider::from_file(std::string filename)
{
	std::fstream source_code;
	source_code.open(filename.c_str(), std::ios::in);

	if (!source_code) throw std::exception(std::string("Exception @ import_source_code: Opening source file " + filename + " failed.").c_str());

	std::string source_code_text;

	source_code.seekg(0, std::ios::end);
	source_code_text.resize(source_code.tellg());
	source_code.seekg(0, std::ios::beg);

	source_code.read(&source_code_text[0], source_code_text.size());
	source_code.close();

	return source_code_text;
}