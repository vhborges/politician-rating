#include <filesystem>
#include <stdexcept>
#include <cstdlib>
#include <string>

using std::string;

/**
 * Checks if the HOME environment variable is set and create the directory on
 * which the database file will be stored. Do nothing if the directory exists.
 * @return the path to this directory
 */
string check_create_dirs();
