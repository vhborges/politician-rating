//standard libraries
#include <limits>

//local headers
#include <data_base.h>
#include <exceptions.h>

// initialize the sqlite3 object
// create data-base file if it doesn't exists
sqlite3* init_db()
{
  sqlite3* db;
  int ret = sqlite3_open(DB_FILE, &db);
  if(ret != SQLITE_OK)
    throw db_exception("Opening", "init_db", ret, std::string(sqlite3_errmsg(db)));

  ret = sqlite3_extended_result_codes(db, 1);
  if(ret != SQLITE_OK)
    throw db_exception("Extended result codes", "init_db", ret, std::string(sqlite3_errmsg(db)));

  return db;
}

// create the politician and ratings tables if they don't already exist
void create_tables(sqlite3* db)
{
  char* errmsg;
  int ret = sqlite3_exec(db, sql_strings::create_tables, NULL, NULL, &errmsg);
  if(ret != SQLITE_OK)
    throw db_exception("Execute", "create_tables", ret, errmsg);
}

void insert_to_politician(sqlite3* db, const politician& p)
{
  sqlite3_stmt* ppStmt;

  try
  {
    int ret = sqlite3_prepare_v2(db, sql_strings::insert_to_politician, -1, &ppStmt, NULL);
    if(ret != SQLITE_OK)
      throw db_exception("Prepare", "insert_to_politician", ret, std::string(sqlite3_errmsg(db)));

    ret = sqlite3_bind_text(ppStmt, 1, p.name.c_str(), -1, SQLITE_TRANSIENT);
    if(ret != SQLITE_OK)
      throw db_exception("Bind", "insert_to_politician", ret, std::string(sqlite3_errmsg(db)));

    ret = sqlite3_bind_text(ppStmt, 2, p.party.c_str(), -1, SQLITE_TRANSIENT);
    if(ret != SQLITE_OK)
      throw db_exception("Bind", "insert_to_politician", ret, std::string(sqlite3_errmsg(db)));

    ret = sqlite3_bind_text(ppStmt, 3, p.info.c_str(), -1, SQLITE_TRANSIENT);
    if(ret != SQLITE_OK)
      throw db_exception("Bind", "insert_to_politician", ret, std::string(sqlite3_errmsg(db)));

    ret = sqlite3_step(ppStmt);
    if(ret != SQLITE_DONE)
      throw politician_op_exception("Insert", "insert_to_politician", ret, std::string(sqlite3_errmsg(db)));

    ret = sqlite3_finalize(ppStmt);
    if(ret != SQLITE_OK)
      throw db_exception("Finalize", "insert_to_politician", ret, std::string(sqlite3_errmsg(db)));

    std::cout << "\nSucessfuly inserted\n"
                 "Press enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    clear_screen();
  }
  catch(...)
  {
    sqlite3_finalize(ppStmt);
    throw;
  }
}

void insert_to_ratings(sqlite3* db, const rating& r)
{
  sqlite3_stmt* ppStmt;
  try
  {
    int ret = sqlite3_prepare_v2(db, sql_strings::insert_to_ratings, -1, &ppStmt, NULL);
    if(ret != SQLITE_OK)
      throw db_exception("Prepare", "insert_to_ratings", ret, std::string(sqlite3_errmsg(db)));

    ret = sqlite3_bind_text(ppStmt, 1, r.name.c_str(), -1, SQLITE_TRANSIENT);
    if(ret != SQLITE_OK)
      throw db_exception("Bind", "insert_to_ratings", ret, std::string(sqlite3_errmsg(db)));

    ret = sqlite3_bind_text(ppStmt, 2, r.party.c_str(), -1, SQLITE_TRANSIENT);
    if(ret != SQLITE_OK)
      throw db_exception("Bind", "insert_to_ratings", ret, std::string(sqlite3_errmsg(db)));

    ret = sqlite3_bind_int(ppStmt, 3, r.points);
    if(ret != SQLITE_OK)
      throw db_exception("Bind", "insert_to_ratings", ret, std::string(sqlite3_errmsg(db)));

    ret = sqlite3_bind_text(ppStmt, 4, r.description.c_str(), -1, SQLITE_TRANSIENT);
    if(ret != SQLITE_OK)
      throw db_exception("Bind", "insert_to_ratings", ret, std::string(sqlite3_errmsg(db)));

    ret = sqlite3_step(ppStmt);
    if(ret != SQLITE_DONE)
      throw rating_op_exception("Insert", "insert_to_ratings", ret, std::string(sqlite3_errmsg(db)));

    ret = sqlite3_finalize(ppStmt);
    if(ret != SQLITE_OK)
      throw db_exception("Finalize", "insert_to_ratings", ret, std::string(sqlite3_errmsg(db)));

    std::cout << "\nSucessfuly inserted\n"
                 "Press enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    clear_screen();
  }
  catch(...)
  {
    sqlite3_finalize(ppStmt);
    throw;
  }
}

void search_by_name(sqlite3* db, const std::string& name)
{
  sqlite3_stmt* ppStmt;
  try
  {
    int ret = sqlite3_prepare_v2(db, sql_strings::search_by_name, -1, &ppStmt, NULL);
    if(ret != SQLITE_OK)
      throw db_exception("Prepare", "search_by_name", ret, std::string(sqlite3_errmsg(db)));

    ret = sqlite3_bind_text(ppStmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    if(ret != SQLITE_OK)
      throw db_exception("Bind", "search_by_name", ret, std::string(sqlite3_errmsg(db)));

    while((ret = sqlite3_step(ppStmt)) == SQLITE_ROW)
    {
      const unsigned char* name = sqlite3_column_text(ppStmt, 0);
      const unsigned char* party = sqlite3_column_text(ppStmt, 1);
      const unsigned char* info = sqlite3_column_text(ppStmt, 2);
      int rating = sqlite3_column_int(ppStmt, 3);

      std::cout << "Name: " << name << "\n"
                << "Party: " << party << "\n"
                << "Rating points: " << rating << "\n"
                << "General information:\n\t" << info << "\n\n";
    }
    if(ret != SQLITE_DONE)
      throw db_exception("Step", "search_by_name", ret, std::string(sqlite3_errmsg(db)));

    ret = sqlite3_finalize(ppStmt);
    if(ret != SQLITE_OK)
      throw db_exception("Finalize", "search_by_name", ret, std::string(sqlite3_errmsg(db)));

    std::cout << "\nPress enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    clear_screen();
  }
  catch(...)
  {
    sqlite3_finalize(ppStmt);
    throw;
  }
}

void search_by_party(sqlite3* db, const std::string& party)
{
  sqlite3_stmt* ppStmt;
  try
  {
    int ret = sqlite3_prepare_v2(db, sql_strings::search_by_party, -1, &ppStmt, NULL);
    if(ret != SQLITE_OK)
      throw db_exception("Prepare", "search_by_party", ret, std::string(sqlite3_errmsg(db)));

    ret = sqlite3_bind_text(ppStmt, 1, party.c_str(), -1, SQLITE_TRANSIENT);
    if(ret != SQLITE_OK)
      throw db_exception("Bind", "search_by_party", ret, std::string(sqlite3_errmsg(db)));

    while((ret = sqlite3_step(ppStmt)) == SQLITE_ROW)
    {
      const unsigned char* name = sqlite3_column_text(ppStmt, 0);
      const unsigned char* party = sqlite3_column_text(ppStmt, 1);
      const unsigned char* info = sqlite3_column_text(ppStmt, 2);
      int rating = sqlite3_column_int(ppStmt, 3);

      std::cout << "Name: " << name << "\n"
                << "Party: " << party << "\n"
                << "Rating points: " << rating << "\n"
                << "General information:\n\t" << info << "\n\n";
    }
    if(ret != SQLITE_DONE)
      throw db_exception("Step", "search_by_party", ret, std::string(sqlite3_errmsg(db)));

    ret = sqlite3_finalize(ppStmt);
    if(ret != SQLITE_OK)
      throw db_exception("Finalize", "search_by_party", ret, std::string(sqlite3_errmsg(db)));

    std::cout << "\nPress enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    clear_screen();
  }
  catch(...)
  {
    sqlite3_finalize(ppStmt);
    throw;
  }
}

void show_ratings(sqlite3* db, const std::string& name, const std::string& party)
{
  sqlite3_stmt* ppStmt;
  try
  {
    int ret = sqlite3_prepare_v2(db, sql_strings::show_ratings, -1, &ppStmt, NULL);
    if(ret != SQLITE_OK)
      throw db_exception("Prepare", "show_ratings", ret, std::string(sqlite3_errmsg(db)));

    ret = sqlite3_bind_text(ppStmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    if(ret != SQLITE_OK)
      throw db_exception("Bind", "show_ratings", ret, std::string(sqlite3_errmsg(db)));

    ret = sqlite3_bind_text(ppStmt, 2, party.c_str(), -1, SQLITE_TRANSIENT);
    if(ret != SQLITE_OK)
      throw db_exception("Bind", "show_ratings", ret, std::string(sqlite3_errmsg(db)));

    while((ret = sqlite3_step(ppStmt)) == SQLITE_ROW)
    {
      const unsigned char* name = sqlite3_column_text(ppStmt, 0);
      const unsigned char* party = sqlite3_column_text(ppStmt, 1);
      int rating = sqlite3_column_int(ppStmt, 2);
      const unsigned char* description = sqlite3_column_text(ppStmt, 3);
      const unsigned char* date_time = sqlite3_column_text(ppStmt, 4);

      std::cout << "Name: " << name << "\n"
                << "Party: " << party << "\n"
                << "Rating: " << rating << "\n"
                << "Date/time: " << date_time << "\n"
                << "Description: " << description << "\n\n";
    }
    if(ret != SQLITE_DONE)
      throw db_exception("Step", "show_ratings", ret, std::string(sqlite3_errmsg(db)));

    ret = sqlite3_finalize(ppStmt);
    if(ret != SQLITE_OK)
      throw db_exception("Finalize", "show_ratings", ret, std::string(sqlite3_errmsg(db)));

    std::cout << "\nPress enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    clear_screen();
  }
  catch(...)
  {
    sqlite3_finalize(ppStmt);
    throw;
  }
}

void show_politicians(sqlite3* db, const std::string& order)
{
  std::string sql = sql_strings::show_politicians + order + ";";
  sqlite3_stmt* ppStmt;
  try
  {
    int ret = sqlite3_prepare_v2(db, sql.c_str(), -1, &ppStmt, NULL);
    if(ret != SQLITE_OK)
      throw db_exception("Prepare", "show_politicians", ret, std::string(sqlite3_errmsg(db)));

    while((ret = sqlite3_step(ppStmt)) == SQLITE_ROW)
    {
      const unsigned char* name = sqlite3_column_text(ppStmt, 0);
      const unsigned char* party = sqlite3_column_text(ppStmt, 1);
      const unsigned char* info = sqlite3_column_text(ppStmt, 2);
      int rating = sqlite3_column_int(ppStmt, 3);

      std::cout << "Name: " << name << "\n"
                << "Party: " << party << "\n"
                << "Rating points: " << rating << "\n"
                << "General information:\n\t" << info << "\n\n";
    }
    if(ret != SQLITE_DONE)
      throw db_exception("Step", "show_politicians", ret, std::string(sqlite3_errmsg(db)));

    ret = sqlite3_finalize(ppStmt);
    if(ret != SQLITE_OK)
      throw db_exception("Finalize", "show_politicians", ret, std::string(sqlite3_errmsg(db)));

    std::cout << "\nPress enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    clear_screen();
  }
  catch(...)
  {
    sqlite3_finalize(ppStmt);
    throw;
  }
}

void update_party(sqlite3* db, const politician_update& p)
{
  sqlite3_stmt* ppStmt;
  try
  {
    int ret = sqlite3_prepare_v2(db, sql_strings::update_party, -1, &ppStmt, NULL);
    if(ret != SQLITE_OK)
      throw db_exception("Prepare", "update_party", ret, std::string(sqlite3_errmsg(db)));

    ret = sqlite3_bind_text(ppStmt, 1, p.new_party.c_str(), -1, SQLITE_TRANSIENT);
    if(ret != SQLITE_OK)
      throw db_exception("Bind", "update_party", ret, std::string(sqlite3_errmsg(db)));

    ret = sqlite3_bind_text(ppStmt, 2, p.name.c_str(), -1, SQLITE_TRANSIENT);
    if(ret != SQLITE_OK)
      throw db_exception("Bind", "update_party", ret, std::string(sqlite3_errmsg(db)));

    ret = sqlite3_bind_text(ppStmt, 3, p.party.c_str(), -1, SQLITE_TRANSIENT);
    if(ret != SQLITE_OK)
      throw db_exception("Bind", "update_party", ret, std::string(sqlite3_errmsg(db)));

    ret = sqlite3_step(ppStmt);
    if(ret != SQLITE_DONE)
      throw db_exception("Update", "update_party", ret, std::string(sqlite3_errmsg(db)));

    ret = sqlite3_finalize(ppStmt);
    if(ret != SQLITE_OK)
      throw db_exception("Finalize", "update_party", ret, std::string(sqlite3_errmsg(db)));

    std::cout << "\nSucessfuly updated\n"
                 "Press enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    clear_screen();
  }
  catch(...)
  {
    sqlite3_finalize(ppStmt);
    throw;
  }
}

void delete_politician(sqlite3* db, const politician_core& p)
{
  sqlite3_stmt* ppStmt;
  try
  {
    int ret = sqlite3_prepare_v2(db, sql_strings::delete_politician, -1, &ppStmt, NULL);
    if(ret != SQLITE_OK)
      throw db_exception("Prepare", "delete_politician", ret, std::string(sqlite3_errmsg(db)));

    ret = sqlite3_bind_text(ppStmt, 1, p.name.c_str(), -1, SQLITE_TRANSIENT);
    if(ret != SQLITE_OK)
      throw db_exception("Bind", "delete_politician", ret, std::string(sqlite3_errmsg(db)));

    ret = sqlite3_bind_text(ppStmt, 2, p.party.c_str(), -1, SQLITE_TRANSIENT);
    if(ret != SQLITE_OK)
      throw db_exception("Bind", "delete_politician", ret, std::string(sqlite3_errmsg(db)));

    ret = sqlite3_step(ppStmt);
    if(ret != SQLITE_DONE)
      throw politician_op_exception("Delete", "delete_politician", ret, std::string(sqlite3_errmsg(db)));

    ret = sqlite3_finalize(ppStmt);
    if(ret != SQLITE_OK)
      throw db_exception("Finalize", "delete_politician", ret, std::string(sqlite3_errmsg(db)));

    std::cout << "\nSucessfuly deleted\n"
                 "Press enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    clear_screen();
  }
  catch(...)
  {
    sqlite3_finalize(ppStmt);
    throw;
  }
}

const char* sql_strings::create_tables = "PRAGMA foreign_keys = ON;"
                                          
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

const char* sql_strings::insert_to_politician = "INSERT INTO politician(name, party, information) "
                                                "VALUES(UPPER(?1), UPPER(?2), ?3);";

const char* sql_strings::insert_to_ratings = "INSERT INTO ratings(polit_name, polit_party, rating, description)"
                                             " VALUES(UPPER(?1), UPPER(?2), ?3, ?4);";

const char* sql_strings::search_by_name = "SELECT * FROM politician"
                                          " WHERE name = UPPER(?1);";

const char* sql_strings::search_by_party = "SELECT * FROM politician"
                                           " WHERE party = UPPER(?1);";

const char* sql_strings::show_ratings = "SELECT * FROM ratings"
                                        " WHERE polit_name = UPPER(?1) AND polit_party = UPPER(?2);";

const char* sql_strings::show_politicians = "SELECT * FROM politician"
                                              " ORDER BY total_rating ";

const char* sql_strings::update_party = "UPDATE politician"
                                        " SET party = UPPER(?1)"
                                        " WHERE name = UPPER(?2) AND party = UPPER(?3);";

const char* sql_strings::delete_politician = "DELETE FROM politician"
                                             " WHERE name = UPPER(?1) AND party = UPPER(?2);";
