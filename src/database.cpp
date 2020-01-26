// Standard libraries
#include <cstdlib>
#include <iostream>

// Local headers
#include <database.hpp>
#include <exceptions.hpp>
#include <filesystem.hpp>

using std::string;
using std::move;

/**
 * Checks if the return_code differs from expected_code,
 * if so, throws an Except exception passing the operation being performed,
 * the function in which it occured and the error message (errmsg) provided
 * from the database.
 */
template<class Except>
inline void check_return(
		const int return_code, const int expected_code,
		string operation, string function_name, const char* errmsg)
{
	if(return_code != expected_code)
		throw Except(move(operation), move(function_name), return_code, errmsg);
}

database::database()
{
	string db_path = check_create_dirs();
	db_path += DB_FILE;

	// Initialize the sqlite3 object and create the database file if it doesn't exists.
	int ret = sqlite3_open(db_path.c_str(), &connection);
	check_return<db_exception>(ret, SQLITE_OK, "Database opening",
			"database constructor", sqlite3_errmsg(connection));

	// Extend the sqlite result codes to better report database errors
	ret = sqlite3_extended_result_codes(connection, 1);
#ifdef DEBUG
	check_return<db_exception>(ret, SQLITE_OK, "Extend result codes",
			"database constructor", sqlite3_errmsg(connection));
#endif

	// Create the politician and ratings tables if they don't already exists.
	char* errmsg;
	ret = sqlite3_exec(connection, sql_strings::create_tables, nullptr,
			nullptr, &errmsg);
#ifdef DEBUG
	check_return<db_exception>(ret, SQLITE_OK, "Create tables",
			"database constructor", errmsg);
#endif
	sqlite3_free(errmsg);
}

database::~database()
{
	int ret = sqlite3_close(connection);
	if(ret != SQLITE_OK)
	{
		std::cerr << "Database error: Could not close the database file\n"
		             "Return code: " << ret << "\n";
	}
}

int database::insert_to_politician(const politician& p) const
{
	const string function_name = "insert_to_politician";

	sqlite_stmt_obj stmt(connection, sql_strings::insert_to_politician, function_name);

	int ret = sqlite3_bind_text(stmt.ppStmt, 1, p.name.c_str(), -1, SQLITE_STATIC);
#ifdef DEBUG
	check_return<db_exception>(
			ret, SQLITE_OK, "Bind name", function_name, sqlite3_errmsg(connection));
#endif

	ret = sqlite3_bind_text(stmt.ppStmt, 2, p.party.c_str(), -1, SQLITE_STATIC);
#ifdef DEBUG
	check_return<db_exception>(
			ret, SQLITE_OK, "Bind party", function_name, sqlite3_errmsg(connection));
#endif

	ret = sqlite3_bind_text(stmt.ppStmt, 3, p.info.c_str(), -1, SQLITE_STATIC);
#ifdef DEBUG
	check_return<db_exception>(
			ret, SQLITE_OK, "Bind info", function_name, sqlite3_errmsg(connection));
#endif

	ret = sqlite3_step(stmt.ppStmt);
	check_return<politician_op_exception>(
			ret, SQLITE_DONE, "Insert", function_name, sqlite3_errmsg(connection));

	return sqlite3_changes(connection);
}

int database::insert_to_ratings(const rating& r) const
{
	const string function_name = "insert_to_ratings";

	sqlite_stmt_obj stmt(connection, sql_strings::insert_to_ratings, function_name);

	int ret = sqlite3_bind_text(stmt.ppStmt, 1, r.name.c_str(), -1, SQLITE_STATIC);
#ifdef DEBUG
	check_return<db_exception>(
			ret, SQLITE_OK, "Bind name", function_name, sqlite3_errmsg(connection));
#endif

	ret = sqlite3_bind_text(stmt.ppStmt, 2, r.party.c_str(), -1, SQLITE_STATIC);
#ifdef DEBUG
	check_return<db_exception>(
			ret, SQLITE_OK, "Bind party", function_name, sqlite3_errmsg(connection));
#endif

	ret = sqlite3_bind_int(stmt.ppStmt, 3, r.points);
#ifdef DEBUG
	check_return<db_exception>(
			ret, SQLITE_OK, "Bind points", function_name, sqlite3_errmsg(connection));
#endif

	ret = sqlite3_bind_text(stmt.ppStmt, 4, r.description.c_str(), -1, SQLITE_STATIC);
#ifdef DEBUG
	check_return<db_exception>(
			ret, SQLITE_OK, "Bind description", function_name, sqlite3_errmsg(connection));
#endif

	ret = sqlite3_step(stmt.ppStmt);
	check_return<rating_op_exception>(
			ret, SQLITE_DONE, "Insert", function_name, sqlite3_errmsg(connection));

	return sqlite3_changes(connection);
}

int database::update_party(const politician_update& p) const
{
	const string function_name = "update_party";

	sqlite_stmt_obj stmt(connection, sql_strings::update_party, function_name);

	int ret = sqlite3_bind_text(stmt.ppStmt, 1, p.new_party.c_str(), -1, SQLITE_STATIC);
#ifdef DEBUG
	check_return<db_exception>(
			ret, SQLITE_OK, "Bind new party", function_name, sqlite3_errmsg(connection));
#endif

	ret = sqlite3_bind_text(stmt.ppStmt, 2, p.name.c_str(), -1, SQLITE_STATIC);
#ifdef DEBUG
	check_return<db_exception>(
			ret, SQLITE_OK, "Bind name", function_name, sqlite3_errmsg(connection));
#endif

	ret = sqlite3_bind_text(stmt.ppStmt, 3, p.party.c_str(), -1, SQLITE_STATIC);
#ifdef DEBUG
	check_return<db_exception>(
			ret, SQLITE_OK, "Bind party", function_name, sqlite3_errmsg(connection));
#endif

	ret = sqlite3_step(stmt.ppStmt);
	check_return<db_exception>(
			ret, SQLITE_DONE, "Update", function_name, sqlite3_errmsg(connection));

	return sqlite3_changes(connection);
}

int database::delete_politician(const politician_core& p) const
{
	const string function_name = "delete_politician";

	sqlite_stmt_obj stmt(connection, sql_strings::delete_politician, function_name);

	int ret = sqlite3_bind_text(stmt.ppStmt, 1, p.name.c_str(), -1, SQLITE_STATIC);
#ifdef DEBUG
	check_return<db_exception>(
			ret, SQLITE_OK, "Bind name", function_name, sqlite3_errmsg(connection));
#endif

	ret = sqlite3_bind_text(stmt.ppStmt, 2, p.party.c_str(), -1, SQLITE_STATIC);
#ifdef DEBUG
	check_return<db_exception>(
			ret, SQLITE_OK, "Bind party", function_name, sqlite3_errmsg(connection));
#endif

	ret = sqlite3_step(stmt.ppStmt);
	check_return<politician_op_exception>(
			ret, SQLITE_DONE, "Delete", function_name, sqlite3_errmsg(connection));

	return sqlite3_changes(connection);
}

const vector<politician> database::get_politician_by_name(const string& name) const
{
	const string function_name = "get_politician_by_name";

	sqlite_stmt_obj stmt(connection, sql_strings::search_by_name, function_name);

	vector<politician> politicians;

	int ret = sqlite3_bind_text(stmt.ppStmt, 1, name.c_str(), -1, SQLITE_STATIC);
#ifdef DEBUG
	check_return<db_exception>(
			ret, SQLITE_OK, "Bind name", function_name, sqlite3_errmsg(connection));
#endif

	while((ret = sqlite3_step(stmt.ppStmt)) == SQLITE_ROW)
	{
		string name = (const char*) sqlite3_column_text(stmt.ppStmt, 0);
		string party = (const char*) sqlite3_column_text(stmt.ppStmt, 1);
		string info = (const char*) sqlite3_column_text(stmt.ppStmt, 2);
		int rating = sqlite3_column_int(stmt.ppStmt, 3);

		politicians.emplace_back(move(name), move(party), move(info), rating);
	}
	check_return<db_exception>(
			ret, SQLITE_DONE, "Search", function_name, sqlite3_errmsg(connection));

	return politicians;
}

const vector<politician> database::get_politicians_by_party(const string& party) const
{
	const string function_name = "get_politicians_by_party";

	sqlite_stmt_obj stmt(connection, sql_strings::search_by_party, function_name);

	vector<politician> politicians;

	int ret = sqlite3_bind_text(stmt.ppStmt, 1, party.c_str(), -1, SQLITE_STATIC);
#ifdef DEBUG
	check_return<db_exception>(
			ret, SQLITE_OK, "Bind party", function_name, sqlite3_errmsg(connection));
#endif

	while((ret = sqlite3_step(stmt.ppStmt)) == SQLITE_ROW)
	{
		string name = (const char*) sqlite3_column_text(stmt.ppStmt, 0);
		string party = (const char*) sqlite3_column_text(stmt.ppStmt, 1);
		string info = (const char*) sqlite3_column_text(stmt.ppStmt, 2);
		int rating = sqlite3_column_int(stmt.ppStmt, 3);

		politicians.emplace_back(move(name), move(party), move(info), rating);
	}
	check_return<db_exception>(
			ret, SQLITE_DONE, "Search", function_name, sqlite3_errmsg(connection));

	return politicians;
}

const vector<rating> database::get_politician_ratings(const politician_core& p) const
{
	const string function_name = "get_politician_ratings";

	sqlite_stmt_obj stmt(connection, sql_strings::show_ratings, function_name);

	vector<rating> ratings;

	int ret = sqlite3_bind_text(stmt.ppStmt, 1, p.name.c_str(), -1, SQLITE_STATIC);
#ifdef DEBUG
	check_return<db_exception>(
			ret, SQLITE_OK, "Bind name", function_name, sqlite3_errmsg(connection));
#endif

	ret = sqlite3_bind_text(stmt.ppStmt, 2, p.party.c_str(), -1, SQLITE_STATIC);
#ifdef DEBUG
	check_return<db_exception>(
			ret, SQLITE_OK, "Bind party", function_name, sqlite3_errmsg(connection));
#endif

	while((ret = sqlite3_step(stmt.ppStmt)) == SQLITE_ROW)
	{
		string name = (const char*) sqlite3_column_text(stmt.ppStmt, 0);
		string party = (const char*) sqlite3_column_text(stmt.ppStmt, 1);
		int rating = sqlite3_column_int(stmt.ppStmt, 2);
		string description = (const char*) sqlite3_column_text(stmt.ppStmt, 3);
		string date_time = (const char*) sqlite3_column_text(stmt.ppStmt, 4);

		ratings.emplace_back(move(name), move(party), move(description), rating, move(date_time));
	}
	check_return<db_exception>(
			ret, SQLITE_DONE, "Search", function_name, sqlite3_errmsg(connection));

	return ratings;
}

const vector<politician> database::get_all_politicians(const string& order) const
{
	const string function_name = "get_all_politicians";

	if(order != "ASC" && order != "DESC")
		throw std::domain_error(
				"'order' parameter of function '" + function_name + "' not satisfed.\n"
				"Expected: [DESC | ASC]. Got: " + order);

	char sql_query[100];
	std::sprintf(sql_query, sql_strings::show_politicians, order.c_str());

	sqlite_stmt_obj stmt(connection, sql_query, function_name);

	vector<politician> politicians;

	int ret;
	while((ret = sqlite3_step(stmt.ppStmt)) == SQLITE_ROW)
	{
		string name = (const char*) sqlite3_column_text(stmt.ppStmt, 0);
		string party = (const char*) sqlite3_column_text(stmt.ppStmt, 1);
		string info = (const char*) sqlite3_column_text(stmt.ppStmt, 2);
		int rating = sqlite3_column_int(stmt.ppStmt, 3);

		politicians.emplace_back(move(name), move(party), move(info), rating);
	}
	check_return<db_exception>(
			ret, SQLITE_DONE, "Search", function_name, sqlite3_errmsg(connection));

	return politicians;
}

const vector<politician_core> database::get_politicians_compact(const string& order) const
{
	const string function_name = "get_politicians_compact";

	if(order != "ASC" && order != "DESC")
		throw std::domain_error(
				"'order' parameter of function '" + function_name + "' not satisfed.\n"
				"Expected: [DESC | ASC]. Got: " + order);

	char sql_query[100];
	std::sprintf(sql_query, sql_strings::show_politicians_compact, order.c_str());

	sqlite_stmt_obj stmt(connection, sql_query, function_name);

	vector<politician_core> politicians;

	int ret;
	while((ret = sqlite3_step(stmt.ppStmt)) == SQLITE_ROW)
	{
		string name = (const char*) sqlite3_column_text(stmt.ppStmt, 0);
		string party = (const char*) sqlite3_column_text(stmt.ppStmt, 1);

		politicians.emplace_back(move(name), move(party));
	}
	check_return<db_exception>(
			ret, SQLITE_DONE, "Search", function_name, sqlite3_errmsg(connection));

	return politicians;
}

const string database::DB_FILE("data.db");

const string database::DB_PATH("/.local/share/politician/");

sqlite_stmt_obj::sqlite_stmt_obj(sqlite3* connection, const char* query,
		const string& function_name)
	: function_name(function_name)
{
	[[maybe_unused]] int ret = sqlite3_prepare_v2(connection, query, -1, &ppStmt, nullptr);
#ifdef DEBUG
	check_return<db_exception>(
			ret, SQLITE_OK, "Prepare", function_name, sqlite3_errmsg(connection));
#endif
}

sqlite_stmt_obj::~sqlite_stmt_obj()
{
	sqlite3_finalize(ppStmt);
}

namespace sql_strings
{
	const char* create_tables =
		"PRAGMA foreign_keys = ON;"

		"CREATE TABLE IF NOT EXISTS politician"
		"(name TEXT NOT NULL,"
		" party TEXT NOT NULL,"
		" information TEXT,"
		" total_rating INTEGER DEFAULT 0,"
		" CONSTRAINT pk_politician PRIMARY KEY (name, party));"

		"CREATE TABLE IF NOT EXISTS ratings"
		"(polit_name TEXT NOT NULL,"
		" polit_party TEXT NOT NULL,"
		" rating INTEGER NOT NULL CHECK(rating BETWEEN -5 AND 5),"
		" description TEXT,"
		" date_time TEXT NOT NULL DEFAULT (DATETIME('now', 'localtime')),"
		" CONSTRAINT pk_ratings PRIMARY KEY (polit_name, polit_party, date_time),"
		" CONSTRAINT fk_ratings FOREIGN KEY (polit_name, polit_party)"
		"   REFERENCES politician(name, party)"
		"   ON DELETE CASCADE"
		"   ON UPDATE CASCADE);"

		"CREATE TRIGGER IF NOT EXISTS update_total_rating"
		" AFTER INSERT ON ratings"
		" WHEN NEW.rating <> 0"
		" BEGIN"
		"   UPDATE politician"
		"   SET total_rating = (total_rating + NEW.rating)"
		"   WHERE name = NEW.polit_name AND party = NEW.polit_party;"
		"END;";

	const char* insert_to_politician =
		"INSERT INTO politician(name, party, information) "
		"VALUES(?1, ?2, ?3);";

	const char* insert_to_ratings =
		"INSERT INTO ratings(polit_name, polit_party, rating, description)"
		" VALUES(?1, ?2, ?3, ?4);";

	const char* search_by_name =
		"SELECT * FROM politician"
		" WHERE name = ?1;";

	const char* search_by_party =
		"SELECT * FROM politician"
		" WHERE party = ?1;";

	const char* show_ratings =
		"SELECT * FROM ratings"
		" WHERE polit_name = ?1 AND polit_party = ?2;";

	const char* show_politicians =
		"SELECT * FROM politician"
		" ORDER BY total_rating %s, name ASC, party ASC";

	const char* show_politicians_compact =
		"SELECT name, party FROM politician"
		" ORDER BY total_rating %s, name ASC, party ASC";

	const char* update_party =
		"UPDATE politician"
		" SET party = ?1"
		" WHERE name = ?2 AND party = ?3;";

	const char* delete_politician =
		"DELETE FROM politician"
		" WHERE name = ?1 AND party = ?2;";
}
