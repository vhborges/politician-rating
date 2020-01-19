// Standard libraries
#include <cstring>
#include <sstream>

// Local headers
#include <exceptions.hpp>

// External libraries
#include <sqlite3.h>

using std::string;
using std::stringstream;
using std::move;

db_exception::db_exception(string error_msg)
	: std::runtime_error(move(error_msg))
{}

db_exception::db_exception(
		string operation, string function, const int return_code, const char* errmsg)
	: db_exception(generate_string(move(operation), move(function), return_code, errmsg))
{}

string db_exception::generate_string(
		string operation, string function, const int return_code, const char* errmsg)
{
	stringstream msg;
	msg << operation << " error in function '" << function << "'. Return code: " << return_code;
	if(errmsg != NULL)
		msg << "\nDescription: " << errmsg;
	return msg.str();
}

politician_op_exception::politician_op_exception(
		string operation, string function, const int return_code, const char* errmsg)
	: db_exception(generate_string(move(operation), move(function), return_code, errmsg))
{}

string politician_op_exception::generate_string(
		string operation, string function, const int return_code, const char* errmsg)
{
	switch(return_code)
	{
		case SQLITE_CONSTRAINT_PRIMARYKEY:
			return "Politician already exists in the database.";

		case SQLITE_CONSTRAINT_NOTNULL:
			return "Required field ommited.";

		default:
			return db_exception::generate_string(move(operation), move(function), return_code, errmsg);
	}
}

rating_op_exception::rating_op_exception(
		string operation, string function, const int return_code, const char* errmsg)
	: db_exception(generate_string(move(operation), move(function), return_code, errmsg))
{}

string rating_op_exception::generate_string(
		string operation, string function, const int return_code, const char* errmsg)
{
	switch(return_code)
	{
		case SQLITE_CONSTRAINT_CHECK:
			return "Rating points must be between -5 and 5.";

		case SQLITE_CONSTRAINT_FOREIGNKEY:
			return "Politician does not exist in the database.";

		case SQLITE_CONSTRAINT_PRIMARYKEY:
			return "Rating already exists in the database.";

		case SQLITE_CONSTRAINT_NOTNULL:
			return "Required field ommited.";

		default:
			return db_exception::generate_string(move(operation), move(function), return_code, errmsg);
	}
}
