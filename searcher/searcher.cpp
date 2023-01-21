#include <iostream>
#include <filesystem>
#include <string>
#include <sstream>
#include <fstream>
#include <ShlObj_core.h>

#include "console_color.h"

void open_folder(std::filesystem::path path) {
	ITEMIDLIST* dir = ILCreateFromPathA(path.parent_path().string().data());
	ITEMIDLIST* item = ILCreateFromPathA(path.string().data());

	SHOpenFolderAndSelectItems(dir, 1, (LPCITEMIDLIST*)&item, 0);

	ILFree(item);
	ILFree(dir);
}

// Cleans up a dragged in filepath ( "C:\" -> C:\ )
void normalize_path(std::string& path) {
	auto pos = path.find("\"");
	while (pos != std::string::npos) {
		path.erase(pos, 1);
		pos = path.find("\"");
	}
}

void map_and_compare(std::filesystem::path path, std::string_view search_word) {
	std::ifstream temp(path);
	std::stringstream buffer;
	buffer << temp.rdbuf();

	if (buffer.str().find(search_word) != std::string::npos) {
		std::cout << dye::green("[+] Found file: " + path.string()) << std::endl;
		open_folder(path);
	}
}

void find_files(std::string path, std::string_view search_word) {
	for (const auto& entry : std::filesystem::directory_iterator(path, 
		std::filesystem::directory_options::skip_permission_denied)) {
		if (entry.is_directory())
			find_files(entry.path().string(), search_word);
		else
			map_and_compare(entry.path(), search_word);
	}
}

void get_user_input(std::string &path) {
	std::cout << "Please input your path:\n";
	while (true) {
		std::getline(std::cin, path);

		normalize_path(path);

		if (std::filesystem::is_directory(path))
			break;

		std::cout << dye::red("\nPath does not exist. Please input a valid path.\n");
	}
}

int main(int argc, char* argv[])
{
	// Needed for SHOpenFolderAndSelectItems to work
	CoInitialize(NULL);

	std::string path;

	if (argc > 1) 
		path = argv[1];
	else
		get_user_input(path);

	system("CLS");
	std::cout << "Searching in: " << path << "\n\nInput search keyword:\n\n";

	std::string keyword;
	std::getline(std::cin, keyword);

	std::cout << "\nStarting search...\n\n";
	find_files(path, keyword);

	std::cout << "\nFinished searching.\n";
	system("pause");
	return 0;
}