#ifndef DATA_BASE_H
#define DATA_BASE_H

//external libraries
#include <sqlite3.h>

//local headers
#include <politician.h>

#define DB_DIR "db/"
#define DB_FILE DB_DIR "data.db"

namespace sql_strings
{
  extern const char* create_tables;

  extern const char* insert_to_politician;

  extern const char* insert_to_ratings;

  extern const char* search_by_name;

  extern const char* search_by_party;

  extern const char* show_ratings;

  extern const char* show_politicians;

  extern const char* update_party;

  extern const char* delete_politician;
}

sqlite3* init_db();

void create_tables(sqlite3* db);

void insert_to_politician(sqlite3* db, const politician& p);

void insert_to_ratings(sqlite3* db, const rating& r);

void search_by_name(sqlite3* db, const std::string& name);

void search_by_party(sqlite3* db, const std::string& party);

void show_ratings(sqlite3* db, const std::string& name, const std::string& party);

void show_politicians(sqlite3* db, const std::string& name);

void update_party(sqlite3* db, const politician_update& p);

void delete_politician(sqlite3* db, const politician_core& p);

#endif