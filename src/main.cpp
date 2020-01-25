// Standard libraries
#include <iostream>

// External libraries
#include <sqlite3.h>

// Local headers
#include <input.hpp>
#include <exceptions.hpp>

int main(int argc, char** argv)
{
	// Better performance when using only C++ stdin/stdout
	std::ios_base::sync_with_stdio(false);

	try
	{
		database db;
		return process_input(argc, argv, db);
	}
	catch(const db_exception& e)
	{
		std::cerr << "Database error: " << e.what() << "\n";
		return EXIT_FAILURE;
	}
	catch(const std::domain_error& e)
	{
		std::cerr << "Domain error: " << e.what() << "\n";
		return EXIT_FAILURE;
	}
	catch(const std::runtime_error& e)
	{
		std::cerr << "Runtime error: " << e.what() << "\n";
		return EXIT_FAILURE;
	}
}
