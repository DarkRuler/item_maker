#include "file_dialog.h"
#include "io_access.h"

std::string file_dialog::open_file()
{
	OPENFILENAME open_filename;
	char szFileName[MAX_PATH] = "";

	ZeroMemory(&open_filename, sizeof(open_filename));

	open_filename.lStructSize = sizeof(open_filename);
	open_filename.hwndOwner = NULL;
	open_filename.lpstrFilter = "All Files (*.*)\0*.*\0";
	open_filename.lpstrFile = szFileName;
	open_filename.nMaxFile = MAX_PATH;
	open_filename.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	open_filename.lpstrDefExt = "*";

	if (GetOpenFileName(&open_filename))
	{
		return io_access::from_file(std::string(szFileName));
	}
	else return "";
}

void file_dialog::save_as(std::string data)
{
	OPENFILENAME open_filename;
	char szFileName[MAX_PATH] = "";

	ZeroMemory(&open_filename, sizeof(open_filename));

	open_filename.lStructSize = sizeof(open_filename);
	open_filename.hwndOwner = NULL;
	open_filename.lpstrFilter = "Comma-Separated Values (*.csv)\0*.csv\0All Files (*.*)\0*.*\0";
	open_filename.lpstrFile = szFileName;
	open_filename.nMaxFile = MAX_PATH;
	open_filename.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	open_filename.lpstrDefExt = "csv";

	if (GetOpenFileName(&open_filename))
	{
		io_access::to_file(std::string(szFileName), data);
	}
}
