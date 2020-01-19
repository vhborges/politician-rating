#ifndef DATA_BASE_HPP
#define DATA_BASE_HPP

// Standard libraries
#include <vector>

// External libraries
#include <sqlite3.h>

// Local headers
#include <politician.hpp>

using std::string;
using std::vector;

// Path of the data base file
extern const string DB_PATH;

namespace sql_strings
{
	extern const char* create_tables;

	extern const char* insert_to_politician;

	extern const char* insert_to_ratings;

	extern const char* search_by_name;

	extern const char* search_by_party;

	extern const char* show_ratings;

	extern const char* show_politicians;

	extern const char* show_politicians_compact;

	extern const char* update_party;

	extern const char* delete_politician;
}

/**
 * Initialize the sqlite3 object and create data-base file if it doesn't exists.
 */
void init_db(sqlite3** db);

/**
 * Create the politician and ratings tables if they don't already exist.
 */
void create_tables(sqlite3* db);

/**
 * Inserts a new politician to the data base.
 * @return the number of affected rows
 */
int insert_to_politician(sqlite3* db, const politician& p);

/**
 * Inserts a new rating to the data base.
 * @return the number of affected rows
 */
int insert_to_ratings(sqlite3* db, const rating& r);

/**
 * Update the party of a politician.
 * @return the number of affected rows
 */
int update_party(sqlite3* db, const politician_update& p);

/**
 * Delete a politician from the data base.
 * @return the number of affected rows
 */
int delete_politician(sqlite3* db, const politician_core& p);

/**
 * Retrives all politician which matches the 'name'.
 * @return a vector of all politicians retrieved.
 */
const vector<politician> get_politician_by_name(sqlite3* db, string name);

/**
 * Retrives all politicians belonging to a party.
 * @return a vector of all politicians belonging to party.
 */
const vector<politician> get_politicians_by_party(sqlite3* db, string party);

/**
 * Retrives all the ratings belonging to a politician.
 * @return a vector of all ratings belonging to politician.
 */
const vector<rating> get_politician_ratings(sqlite3* db, const politician_core& p);

/**
 * Retrives all politicians registered in the data base.
 * @param order the order on which the politicians will be inserted in the vector
 * based on the ratings points ("DESC" or "ASC").
 * @return a vector of all politicians registered in the data base.
 */
const vector<politician> get_all_politicians(sqlite3* db, const string& order = "DESC");

/**
 * Compact version of function 'get_all_politicians'.
 * Only returns the names and parties of the politicians.
 * @param order the order on which the politicians will be inserted in the vector
 * based on the ratings points ("DESC" or "ASC").
 * @return a vector of all politician's names and parties registered in the data base.
 */
const vector<politician_core> get_politicians_compact(sqlite3* db, const string& order = "DESC");

#endif
