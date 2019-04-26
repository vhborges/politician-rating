#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

//standard libraries
#include <iostream>
#include <sstream>

//external libraries
#include <sqlite3.h>

class db_exception : public std::runtime_error
{
  protected:

  virtual const std::string generate_string(const std::string& operation, const std::string& function,
      const int result_code, const std::string& err_msg);
  virtual const std::string generate_string(const std::string& operation, const std::string& function,
      const int result_code, char* err_msg);

  public:

  db_exception(const std::string& str);

  db_exception(const std::string& operation, const std::string& function, const int result_code,
    const std::string& err_msg);

  db_exception(const std::string& operation, const std::string& function, const int result_code,
    char* err_msg);
};

class politician_op_exception : public db_exception
{
  protected:

  virtual const std::string generate_string(const std::string& operation, const std::string& function,
      const int result_code, const std::string& err_msg);
  virtual const std::string generate_string(const std::string& operation, const std::string& function,
      const int result_code, char* err_msg);

  public:

  politician_op_exception(const std::string& operation, const std::string& function, const int result_code,
    const std::string& err_msg);

  politician_op_exception(const std::string& operation, const std::string& function, const int result_code,
    char* err_msg);
};

class rating_op_exception : public db_exception
{
  protected:

  virtual const std::string generate_string(const std::string& operation, const std::string& function,
      const int result_code, const std::string& err_msg);
  virtual const std::string generate_string(const std::string& operation, const std::string& function,
      const int result_code, char* err_msg);

  public:

  rating_op_exception(const std::string& operation, const std::string& function, const int result_code,
    const std::string& err_msg);

  rating_op_exception(const std::string& operation, const std::string& function, const int result_code,
    char* err_msg);
};

#endif