// Standard libraries
#include <cstdlib>

// Local headers
#include <data_base.hpp>
#include <exceptions.hpp>

using std::string;
using std::move;

/**
 * Checks if the return_code differs from expected_code,
 * if so, throws an Except exception passing the operation being performed,
 * the function in which it occured and the error message (errmsg) from
 * the database.
 */
template<class Except>
inline void check_return(
		const int return_code, const int expected_code,
		string operation, string function, const char* errmsg)
{
	if(return_code != expected_code)
		throw Except(move(operation), move(function), return_code, errmsg);
}

void init_db(sqlite3** db)
{
	int ret = sqlite3_open(DB_PATH.c_str(), db);
	if(ret != SQLITE_OK)
		// Not using the check_return function because a db_exception would trigger
		// the main function to close the database file
		throw std::runtime_error(
				"Could not open the database file.\n"
				"Function: 'init_db'. Return code: " + std::to_string(ret));

	ret = sqlite3_extended_result_codes(*db, 1);
	check_return<db_exception>(ret, SQLITE_OK, "Extend result codes", "init_db", nullptr);
}

void create_tables(sqlite3* db)
{
	char* errmsg;
	int ret = sqlite3_exec(db, sql_strings::create_tables, nullptr, nullptr, &errmsg);
	check_return<db_exception>(ret, SQLITE_OK, "Execute", "create_tables", errmsg);
	sqlite3_free(errmsg);
}

int insert_to_politician(sqlite3* db, const politician& p)
{
	sqlite3_stmt* ppStmt;

	try
	{
		int ret = sqlite3_prepare_v2(db, sql_strings::insert_to_politician, -1, &ppStmt, nullptr);
		check_return<db_exception>(
				ret, SQLITE_OK, "Prepare", "insert_to_politician", sqlite3_errmsg(db));

		ret = sqlite3_bind_text(ppStmt, 1, p.name.c_str(), -1, SQLITE_STATIC);
		check_return<db_exception>(
				ret, SQLITE_OK, "Bind", "insert_to_politician", sqlite3_errmsg(db));

		ret = sqlite3_bind_text(ppStmt, 2, p.party.c_str(), -1, SQLITE_STATIC);
		check_return<db_exception>(
				ret, SQLITE_OK, "Bind", "insert_to_politician", sqlite3_errmsg(db));

		ret = sqlite3_bind_text(ppStmt, 3, p.info.c_str(), -1, SQLITE_STATIC);
		check_return<db_exception>(
				ret, SQLITE_OK, "Bind", "insert_to_politician", sqlite3_errmsg(db));

		ret = sqlite3_step(ppStmt);
		check_return<politician_op_exception>(
				ret, SQLITE_DONE, "Insert", "insert_to_politician", sqlite3_errmsg(db));

		ret = sqlite3_finalize(ppStmt);
		check_return<db_exception>(
				ret, SQLITE_OK, "Finalize", "insert_to_politician", sqlite3_errmsg(db));
	}
	catch(...)
	{
		sqlite3_finalize(ppStmt);
		throw;
	}
	return sqlite3_changes(db);
}

int insert_to_ratings(sqlite3* db, const rating& r)
{
	sqlite3_stmt* ppStmt;

	try
	{
		int ret = sqlite3_prepare_v2(db, sql_strings::insert_to_ratings, -1, &ppStmt, nullptr);
		check_return<db_exception>(
				ret, SQLITE_OK, "Prepare", "insert_to_ratings", sqlite3_errmsg(db));

		ret = sqlite3_bind_text(ppStmt, 1, r.name.c_str(), -1, SQLITE_STATIC);
		check_return<db_exception>(
				ret, SQLITE_OK, "Bind", "insert_to_ratings", sqlite3_errmsg(db));

		ret = sqlite3_bind_text(ppStmt, 2, r.party.c_str(), -1, SQLITE_STATIC);
		check_return<db_exception>(
				ret, SQLITE_OK, "Bind", "insert_to_ratings", sqlite3_errmsg(db));

		ret = sqlite3_bind_int(ppStmt, 3, r.points);
		check_return<db_exception>(
				ret, SQLITE_OK, "Bind", "insert_to_ratings", sqlite3_errmsg(db));

		ret = sqlite3_bind_text(ppStmt, 4, r.description.c_str(), -1, SQLITE_STATIC);
		check_return<db_exception>(
				ret, SQLITE_OK, "Bind", "insert_to_ratings", sqlite3_errmsg(db));

		ret = sqlite3_step(ppStmt);
		check_return<rating_op_exception>(
				ret, SQLITE_DONE, "Insert", "insert_to_ratings", sqlite3_errmsg(db));

		ret = sqlite3_finalize(ppStmt);
		check_return<db_exception>(
				ret, SQLITE_OK, "Finalize", "insert_to_ratings", sqlite3_errmsg(db));
	}
	catch(...)
	{
		sqlite3_finalize(ppStmt);
		throw;
	}
	return sqlite3_changes(db);
}

int update_party(sqlite3* db, const politician_update& p)
{
	sqlite3_stmt* ppStmt;
	try
	{
		int ret = sqlite3_prepare_v2(db, sql_strings::update_party, -1, &ppStmt, nullptr);
		check_return<db_exception>(
				ret, SQLITE_OK, "Prepare", "update_party", sqlite3_errmsg(db));

		ret = sqlite3_bind_text(ppStmt, 1, p.new_party.c_str(), -1, SQLITE_STATIC);
		check_return<db_exception>(
				ret, SQLITE_OK, "Bind", "update_party", sqlite3_errmsg(db));

		ret = sqlite3_bind_text(ppStmt, 2, p.name.c_str(), -1, SQLITE_STATIC);
		check_return<db_exception>(
				ret, SQLITE_OK, "Bind", "update_party", sqlite3_errmsg(db));

		ret = sqlite3_bind_text(ppStmt, 3, p.party.c_str(), -1, SQLITE_STATIC);
		check_return<db_exception>(
				ret, SQLITE_OK, "Bind", "update_party", sqlite3_errmsg(db));

		ret = sqlite3_step(ppStmt);
		check_return<db_exception>(
				ret, SQLITE_DONE, "Update", "update_party", sqlite3_errmsg(db));

		ret = sqlite3_finalize(ppStmt);
		check_return<db_exception>(
				ret, SQLITE_OK, "Finalize", "update_party", sqlite3_errmsg(db));
	}
	catch(...)
	{
		sqlite3_finalize(ppStmt);
		throw;
	}
	return sqlite3_changes(db);
}

int delete_politician(sqlite3* db, const politician_core& p)
{
	sqlite3_stmt* ppStmt;
	try
	{
		int ret = sqlite3_prepare_v2(db, sql_strings::delete_politician, -1, &ppStmt, nullptr);
		check_return<db_exception>(
				ret, SQLITE_OK, "Prepare", "delete_politician", sqlite3_errmsg(db));

		ret = sqlite3_bind_text(ppStmt, 1, p.name.c_str(), -1, SQLITE_STATIC);
		// TODO necessário checar?
		check_return<db_exception>(
				ret, SQLITE_OK, "Bind", "delete_politician", sqlite3_errmsg(db));

		ret = sqlite3_bind_text(ppStmt, 2, p.party.c_str(), -1, SQLITE_STATIC);
		check_return<db_exception>(
				ret, SQLITE_OK, "Bind", "delete_politician", sqlite3_errmsg(db));

		ret = sqlite3_step(ppStmt);
		check_return<politician_op_exception>(
				ret, SQLITE_DONE, "Delete", "delete_politician", sqlite3_errmsg(db));

		ret = sqlite3_finalize(ppStmt);
		check_return<db_exception>(
				ret, SQLITE_OK, "Finalize", "delete_politician", sqlite3_errmsg(db));
	}
	catch(...)
	{
		sqlite3_finalize(ppStmt);
		throw;
	}
	return sqlite3_changes(db);
}

const vector<politician> get_politician_by_name(sqlite3* db, const string& name)
{
	sqlite3_stmt* ppStmt;

	vector<politician> vec;

	try
	{
		int ret = sqlite3_prepare_v2(db, sql_strings::search_by_name, -1, &ppStmt, nullptr);
		check_return<db_exception>(
				ret, SQLITE_OK, "Prepare", "get_politician_by_name", sqlite3_errmsg(db));

		ret = sqlite3_bind_text(ppStmt, 1, name.c_str(), -1, SQLITE_STATIC);
		check_return<db_exception>(
				ret, SQLITE_OK, "Bind", "get_politician_by_name", sqlite3_errmsg(db));

		while((ret = sqlite3_step(ppStmt)) == SQLITE_ROW)
		{
			string name = (const char*) sqlite3_column_text(ppStmt, 0);
			string party = (const char*) sqlite3_column_text(ppStmt, 1);
			string info = (const char*) sqlite3_column_text(ppStmt, 2);
			int rating = sqlite3_column_int(ppStmt, 3);

			vec.emplace_back(move(name), move(party), move(info), rating);
		}
		check_return<db_exception>(
				ret, SQLITE_DONE, "Step", "get_politician_by_name", sqlite3_errmsg(db));

		ret = sqlite3_finalize(ppStmt);
		check_return<db_exception>(
				ret, SQLITE_OK, "Finalize", "get_politician_by_name", sqlite3_errmsg(db));
	}
	catch(...)
	{
		sqlite3_finalize(ppStmt);
		throw;
	}
	return vec;
}

const vector<politician> get_politicians_by_party(sqlite3* db, const string& party)
{
	sqlite3_stmt* ppStmt;

	vector<politician> vec;

	try
	{
		int ret = sqlite3_prepare_v2(db, sql_strings::search_by_party, -1, &ppStmt, nullptr);
		check_return<db_exception>(
				ret, SQLITE_OK, "Prepare", "get_politicians_by_party", sqlite3_errmsg(db));

		ret = sqlite3_bind_text(ppStmt, 1, party.c_str(), -1, SQLITE_STATIC);
		check_return<db_exception>(
				ret, SQLITE_OK, "Bind", "get_politicians_by_party", sqlite3_errmsg(db));

		while((ret = sqlite3_step(ppStmt)) == SQLITE_ROW)
		{
			string name = (const char*) sqlite3_column_text(ppStmt, 0);
			string party = (const char*) sqlite3_column_text(ppStmt, 1);
			string info = (const char*) sqlite3_column_text(ppStmt, 2);
			int rating = sqlite3_column_int(ppStmt, 3);

			vec.emplace_back(move(name), move(party), move(info), rating);
		}
		check_return<db_exception>(
				ret, SQLITE_DONE, "Step", "get_politicians_by_party", sqlite3_errmsg(db));

		ret = sqlite3_finalize(ppStmt);
		check_return<db_exception>(
				ret, SQLITE_OK, "Finalize", "get_politicians_by_party", sqlite3_errmsg(db));
	}
	catch(...)
	{
		sqlite3_finalize(ppStmt);
		throw;
	}
	return vec;
}

const vector<rating> get_politician_ratings(sqlite3* db, const politician_core& p)
{
	sqlite3_stmt* ppStmt;

	vector<rating> vec;

	try
	{
		int ret = sqlite3_prepare_v2(db, sql_strings::show_ratings, -1, &ppStmt, nullptr);
		check_return<db_exception>(
				ret, SQLITE_OK, "Prepare", "get_politician_ratings", sqlite3_errmsg(db));

		ret = sqlite3_bind_text(ppStmt, 1, p.name.c_str(), -1, SQLITE_STATIC);
		check_return<db_exception>(
				ret, SQLITE_OK, "Bind", "get_politician_ratings", sqlite3_errmsg(db));

		ret = sqlite3_bind_text(ppStmt, 2, p.party.c_str(), -1, SQLITE_STATIC);
		check_return<db_exception>(
				ret, SQLITE_OK, "Bind", "get_politician_ratings", sqlite3_errmsg(db));

		while((ret = sqlite3_step(ppStmt)) == SQLITE_ROW)
		{
			string name = (const char*) sqlite3_column_text(ppStmt, 0);
			string party = (const char*) sqlite3_column_text(ppStmt, 1);
			int rating = sqlite3_column_int(ppStmt, 2);
			string description = (const char*) sqlite3_column_text(ppStmt, 3);
			string date_time = (const char*) sqlite3_column_text(ppStmt, 4);

			vec.emplace_back(move(name), move(party), move(description), rating, move(date_time));
		}
		check_return<db_exception>(
				ret, SQLITE_DONE, "Step", "get_politician_ratings", sqlite3_errmsg(db));

		ret = sqlite3_finalize(ppStmt);
		check_return<db_exception>(
				ret, SQLITE_OK, "Finalize", "get_politician_ratings", sqlite3_errmsg(db));
	}
	catch(...)
	{
		sqlite3_finalize(ppStmt);
		throw;
	}
	return vec;
}

const vector<politician> get_all_politicians(sqlite3* db, const string& order)
{
	if(order != "ASC" && order != "DESC")
		throw std::domain_error(
				"'order' parameter of function 'get_all_politicians' not satisfed.\n"
				"Expected: [DESC | ASC]. Got: " + order);

	char sql_query[100];
	std::sprintf(sql_query, sql_strings::show_politicians, order.c_str());

	sqlite3_stmt* ppStmt;

	vector<politician> vec;

	try
	{
		int ret = sqlite3_prepare_v2(db, sql_query, -1, &ppStmt, nullptr);
		check_return<db_exception>(
				ret, SQLITE_OK, "Prepare", "get_all_politicians", sqlite3_errmsg(db));

		while((ret = sqlite3_step(ppStmt)) == SQLITE_ROW)
		{
			string name = (const char*) sqlite3_column_text(ppStmt, 0);
			string party = (const char*) sqlite3_column_text(ppStmt, 1);
			string info = (const char*) sqlite3_column_text(ppStmt, 2);
			int rating = sqlite3_column_int(ppStmt, 3);

			vec.emplace_back(move(name), move(party), move(info), rating);
		}
		check_return<db_exception>(
				ret, SQLITE_DONE, "Step", "get_all_politicians", sqlite3_errmsg(db));

		ret = sqlite3_finalize(ppStmt);
		check_return<db_exception>(
				ret, SQLITE_OK, "Finalize", "get_all_politicians", sqlite3_errmsg(db));
	}
	catch(...)
	{
		sqlite3_finalize(ppStmt);
		throw;
	}
	return vec;
}

const vector<politician_core> get_politicians_compact(sqlite3* db, const string& order)
{
	if(order != "ASC" && order != "DESC")
		throw std::domain_error(
				"'order' parameter of function 'get_politicians_compact' not satisfed.\n"
				"Expected: [DESC | ASC]. Got: " + order);

	char sql_query[100];
	std::sprintf(sql_query, sql_strings::show_politicians_compact, order.c_str());

	sqlite3_stmt* ppStmt;

	vector<politician_core> vec;

	try
	{
		int ret = sqlite3_prepare_v2(db, sql_query, -1, &ppStmt, nullptr);
		check_return<db_exception>(
				ret, SQLITE_OK, "Prepare", "get_politicians_compact", sqlite3_errmsg(db));

		while((ret = sqlite3_step(ppStmt)) == SQLITE_ROW)
		{
			string name = (const char*) sqlite3_column_text(ppStmt, 0);
			string party = (const char*) sqlite3_column_text(ppStmt, 1);

			vec.emplace_back(move(name), move(party));
		}
		check_return<db_exception>(
				ret, SQLITE_DONE, "Step", "get_politicians_compact", sqlite3_errmsg(db));

		ret = sqlite3_finalize(ppStmt);
		check_return<db_exception>(
				ret, SQLITE_OK, "Finalize", "get_politicians_compact", sqlite3_errmsg(db));
	}
	catch(...)
	{
		sqlite3_finalize(ppStmt);
		throw;
	}
	return vec;
}

// Where the database file will be located
const string DB_FILE = "data.db";
//const string HOME_DIR = std::getenv("HOME");
//const string DB_PATH = HOME_DIR + "/.local/share/politician/" + DB_FILE;
const string DB_PATH = "db/" + DB_FILE;

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
