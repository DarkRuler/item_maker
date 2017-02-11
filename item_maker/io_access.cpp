#include "io_access.h"
#include <fstream>

std::string io_access::from_file(std::string filename)
{
	std::fstream source_code;
	source_code.open(filename.c_str(), std::ios::in);
	if (!source_code) return "";

	std::string source_code_text;

	source_code.seekg(0, std::ios::end);
	source_code_text.resize(source_code.tellg());
	source_code.seekg(0, std::ios::beg);

	source_code.read(&source_code_text[0], source_code_text.size());
	source_code.close();

	return source_code_text;
}

void io_access::to_file(std::string filename, std::string data)
{
	std::fstream output_file;
	output_file.open(filename.c_str(), std::ios::out);
	if (!output_file) return;

	output_file.write(data.c_str(), data.size());
	output_file.close();
}
