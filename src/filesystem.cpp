#include <filesystem.hpp>

using std::filesystem::create_directories;
using std::string;
using std::getenv;

string check_create_dirs()
{
	const char* home_dir = getenv("HOME");
	if(home_dir == nullptr)
		throw std::runtime_error("HOME environment variable not set");
	string _path(string(home_dir) + "/.local/share/politician/");
	create_directories(_path);
	return _path;
}
