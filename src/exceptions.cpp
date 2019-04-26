//standard libraries
#include <cstring>

//local headers
#include <exceptions.h>

db_exception::db_exception(const std::string& str)
  : std::runtime_error(str)
{}

db_exception::db_exception(const std::string& operation, const std::string& function, const int result_code,
    const std::string& err_msg)
  : db_exception(generate_string(operation, function, result_code, err_msg))
{}

db_exception::db_exception(const std::string& operation, const std::string& function, const int result_code,
    char* err_msg)
  : db_exception(generate_string(operation, function, result_code, err_msg))
{}

const std::string db_exception::generate_string(const std::string& operation, const std::string& function,
    const int result_code, const std::string& err_msg)
{
  std::stringstream temp;
  if(result_code != SQLITE_MISUSE)
    temp << operation << " error on function '" << function << "'. Code: " << result_code << "\n" << err_msg;
  else
    temp << operation << " error on function '" << function << "'. Code: " << result_code;
  const std::string msg = temp.str();
  return msg;
}

const std::string db_exception::generate_string(const std::string& operation, const std::string& function,
    const int result_code, char* err_msg)
{
  std::stringstream temp;
  if(err_msg != NULL)
  {
    temp << operation << " error on function '" << function << "'. Code: " << result_code << "\n" << err_msg;
    sqlite3_free(err_msg);
  }
  else
    temp << operation << " error on function '" << function << "'. Code: " << result_code;
  const std::string msg = temp.str();
  return msg;
}

politician_op_exception::politician_op_exception(const std::string& operation, const std::string& function, 
    const int result_code, const std::string& err_msg)
  : db_exception(generate_string(operation, function, result_code, err_msg))
{}

politician_op_exception::politician_op_exception(const std::string& operation, const std::string& function, 
    const int result_code, char* err_msg)
  : db_exception(generate_string(operation, function, result_code, err_msg))
{}

const std::string politician_op_exception::generate_string(const std::string& operation, const std::string& function,
    const int result_code, const std::string& err_msg)
{
  switch(result_code)
  {
    case SQLITE_CONSTRAINT_PRIMARYKEY:
      return "Politician already exists on the data-base.";

    case SQLITE_CONSTRAINT_NOTNULL:
      return "Required field ommited.";

    default:
      return db_exception::generate_string(operation, function, result_code, err_msg);
  }
}

const std::string politician_op_exception::generate_string(const std::string& operation, const std::string& function,
    const int result_code, char* err_msg)
{
  switch(result_code)
  {
    case SQLITE_CONSTRAINT_PRIMARYKEY:
      sqlite3_free(err_msg);
      return "Politician already exists on the data-base.";

    case SQLITE_CONSTRAINT_NOTNULL:
      sqlite3_free(err_msg);
      return "Required field ommited.";

    default:
      return db_exception::generate_string(operation, function, result_code, err_msg);
  }
}

rating_op_exception::rating_op_exception(const std::string& operation, const std::string& function, const int result_code,
  const std::string& err_msg)
    : db_exception(generate_string(operation, function, result_code, err_msg))
{}

rating_op_exception::rating_op_exception(const std::string& operation, const std::string& function, const int result_code,
  char* err_msg)
    : db_exception(generate_string(operation, function, result_code, err_msg))
{}

const std::string rating_op_exception::generate_string(const std::string& operation, const std::string& function,
    const int result_code, const std::string& err_msg)
{
  switch(result_code)
  {
    case SQLITE_CONSTRAINT_CHECK:
      return "Rating points must be between -5 and 5.";

    case SQLITE_CONSTRAINT_FOREIGNKEY:
      return "Politician does not exist on the data-base.";
    
    case SQLITE_CONSTRAINT_PRIMARYKEY:
      return "Rating already exists on the data-base.";

    case SQLITE_CONSTRAINT_NOTNULL:
      return "Required field ommited.";

    default:
      return db_exception::generate_string(operation, function, result_code, err_msg);
  }
}

const std::string rating_op_exception::generate_string(const std::string& operation, const std::string& function,
    const int result_code, char* err_msg)
{
  switch(result_code)
  {
    case SQLITE_CONSTRAINT_CHECK:
      sqlite3_free(err_msg);
      return "Rating points must be between -5 and 5.";

    case SQLITE_CONSTRAINT_FOREIGNKEY:
      sqlite3_free(err_msg);
      return "Politician does not exist on the data-base.";
    
    case SQLITE_CONSTRAINT_PRIMARYKEY:
      sqlite3_free(err_msg);
      return "Rating already exists on the data-base.";

    case SQLITE_CONSTRAINT_NOTNULL:
      sqlite3_free(err_msg);
      return "Required field ommited.";

    default:
      return db_exception::generate_string(operation, function, result_code, err_msg);
  }
}
