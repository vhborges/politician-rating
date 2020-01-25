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

struct database
{
	sqlite3* connection;

	// Name of the database file
	static const string DB_FILE;

	// Where the database file will be located, after the HOME dir
	static const string DB_PATH;

	/**
	 * Class constructor.
	 * Create the database file and tables and open a connection to the database.
	 */
	database();

	/**
	 * Class destructor.
	 * Close the database connection.
	 */
	~database();

	/**
	 * Inserts a new politician to the database.
	 * @return the number of affected rows
	 */
	int insert_to_politician(const politician& p) const;

	/**
	 * Inserts a new rating to the database.
	 * @return the number of affected rows
	 */
	int insert_to_ratings(const rating& r) const;

	/**
	 * Update the party of a politician.
	 * @return the number of affected rows
	 */
	int update_party(const politician_update& p) const;

	/**
	 * Delete a politician from the database.
	 * @return the number of affected rows
	 */
	int delete_politician(const politician_core& p) const;

	/**
	 * Retrives all politician which matches the 'name'.
	 * @return a vector of all politicians retrieved.
	 */
	const vector<politician> get_politician_by_name(const string& name) const;

	/**
	 * Retrives all politicians belonging to a party.
	 * @return a vector of all politicians belonging to party.
	 */
	const vector<politician> get_politicians_by_party(const string& party) const;

	/**
	 * Retrives all the ratings belonging to a politician.
	 * @return a vector of all ratings belonging to politician.
	 */
	const vector<rating> get_politician_ratings(const politician_core& p) const;

	/**
	 * Retrives all politicians registered in the database.
	 * @param order the order on which the politicians will be inserted in the vector
	 * based on the ratings points ("DESC" or "ASC").
	 * @return a vector of all politicians registered in the database.
	 */
	const vector<politician> get_all_politicians(const string& order = "DESC") const;

	/**
	 * Compact version of function 'get_all_politicians'.
	 * Only returns the names and parties of the politicians.
	 * @param order the order on which the politicians will be inserted in the vector
	 * based on the ratings points ("DESC" or "ASC").
	 * @return a vector of all politician's names and parties registered in the database.
	 */
	const vector<politician_core> get_politicians_compact(
			const string& order = "DESC") const;
};

struct sqlite_stmt_obj
{
	sqlite3_stmt* ppStmt;

	const string& function_name;

	/**
	 * Class constructor.
	 * Initialize the prepare statement with the provided SQL query.
	 */
	sqlite_stmt_obj(sqlite3* connection, const char* query, const string& function_name);

	/**
	 * Class destructor.
	 * Finalize the statement object.
	 */
	~sqlite_stmt_obj();
};

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

#endif
