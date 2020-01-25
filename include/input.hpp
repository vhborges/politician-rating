#ifndef INPUT_HPP
#define INPUT_HPP

// Standard libraries
#include <string>

// External libraries
#include <sqlite3.h>

// Local headers
#include <database.hpp>

using std::string;

/**
 * Gets confirmation, from the user, of the desired operation (register, update etc).
 * @return true if the user confirmed, else false
 */
bool confirm_operation(const string& operation);

/**
 * Set and process the subcommands, options and flags passed on the command line.
 * @return 0 on success and any other integer on error.
 */
int process_input(int argc, char** argv, const database& db);

/**
 * Replaces all "\n" of string 'to_replace' with a true newline character.
 */
void replace_newline(string& to_replace);

#endif
