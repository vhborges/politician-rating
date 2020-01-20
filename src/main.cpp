// Standard libraries
#include <iostream>

// External libraries
#include <sqlite3.h>

// Local headers
#include <input.hpp>
#include <exceptions.hpp>
#include <data_base.hpp>

int main(int argc, char** argv)
{
	// Better performance when using only C++ stdin/stdout
	std::ios_base::sync_with_stdio(false);

	sqlite3* db;
	try
	{
		init_db(&db);
		create_tables(db);
		int ret = process_input(argc, argv, db);
		if(sqlite3_close(db) != SQLITE_OK)
		{
			std::cerr << "Error: could not close the database file\n";
			return EXIT_FAILURE;
		}
		return ret;
	}
	catch(const db_exception& e)
	{
		std::cerr << "Database error: " << e.what() << "\n";
		if(sqlite3_close(db) != SQLITE_OK)
			std::cerr << "Database error: could not close the database file\n";
		return EXIT_FAILURE;
	}
	catch(const std::domain_error& e)
	{
		std::cerr << "Domain error: " << e.what() << "\n";
		if(sqlite3_close(db) != SQLITE_OK)
			std::cerr << "Database error: could not close the database file\n";
		return EXIT_FAILURE;
	}
	catch(const std::runtime_error& e)
	{
		std::cerr << "Runtime error: " << e.what() << "\n";
		return EXIT_FAILURE;
	}
}
