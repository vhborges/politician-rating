//standard libraries
#include <iostream>
#include <sstream>
#include <limits>

//external libraries
#include <sqlite3.h>

//local headers
#include <politician.h>
#include <system.h>
#include <exceptions.h>
#include <data_base.h>

// application main menu
unsigned short menu(const std::string& str_menu)
{
  unsigned short opt;
  bool valid;
  do
  {
    valid = true;
    std::cout << str_menu;
    std::cin >> opt;
    clear_screen();
    if(std::cin.fail())
    {
      std::cout << "--> Invalid entry. Type only a number!\n\n";
      valid = false;
      std::cin.clear();
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  while(!valid);

  return opt;
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  try
  {
    sqlite3* db = init_db();
    create_tables(db);
    const std::string main_str = "1 - Register new politician\n"
                                 "2 - Add a new rate to an existing politician\n"
                                 "3 - Update a politician's party\n"
                                 "4 - Delete a politician\n"
                                 "5 - Search options\n"
                                 "6 - Exit\n\n"
                                 "Choose a option [1-6]: ";
    unsigned short opt;
    while((opt = menu(main_str)) != 6)
    {
      switch(opt)
      {
        case 1:
        {
          politician p;
          while(true)
          {
            if(new_entity<politician>(p, "insertion"))
            {
              try
              {
                insert_to_politician(db, p);
              }
              catch(const politician_op_exception& e)
              {
                std::cout << "\n--> Insertion error: " << e.what() << "\n\n";
                continue;
              }
            }
            break;
          }
          break;
        }

        case 2:
        {
          rating r;
          while(true)
          {
            if(new_entity<rating>(r, "insertion"))
            {
              try
              {
                insert_to_ratings(db, r);
              }
              catch(const rating_op_exception& e)
              {
                std::cout << "\n--> Insertion error: " << e.what() << "\n\n";
                continue;
              }
            }
            break;
          }
          break;
        }

        case 3:
        {
          politician_update p;
          while(true)
          {
            if(new_entity<politician_update>(p, "update"))
              update_party(db, p);
            break;
          }
          break;
        }

        case 4:
        {
          politician_core p;
          while(true)
          {
            if(new_entity<politician_core>(p, "delete"))
            {
              try
              {
                delete_politician(db, p);
              }
              catch(const politician_op_exception& e)
              {
                std::cout << "\n--> Deletion error: " << e.what() << "\n\n";
                continue;
              }
            }
            break;
          }
          break;
        }

        case 5:
        {
          const std::string search_str = "1 - Search by name\n"
                                         "2 - Search by party\n"
                                         "3 - Show all ratings belonging to a politician\n"
                                         "4 - Show all politicians ordered by highest rating\n"
                                         "5 - Show all politicians ordered by lowest rating\n"
                                         "6 - Return to main menu\n\n"
                                         "Choose a option [1-6]: ";
          unsigned short search_opt;
          while((search_opt = menu(search_str)) != 6)
          {
            switch(search_opt)
            {
              case 1:
              {
                std::cout << "Name: ";
                std::string name;
                std::getline(std::cin, name);
                clear_screen();
                search_by_name(db, name);
                break;
              }

              case 2:
              {
                std::cout << "Party: ";
                std::string party;
                std::getline(std::cin, party);
                clear_screen();
                search_by_party(db, party);
                break;
              }

              case 3:
              {
                std::string name, party;
                std::cout << "Politician's name: ";
                std::getline(std::cin, name);
                std::cout << "Politician's party: ";
                std::getline(std::cin, party);
                clear_screen();
                show_ratings(db, name, party);
                break;
              }

              case 4:
              {
                clear_screen();
                show_politicians(db, "DESC");
                break;
              }

              case 5:
              {
                clear_screen();
                show_politicians(db, "ASC");
                break;
              }

              default:
                std::cout << "--> Invalid option. Please try again.\n\n";
            }
          }
          break;
        }

        default:
          std::cout << "--> Invalid option. Please try again.\n\n";
      }
    }
    int ret = sqlite3_close(db);
    if(ret != SQLITE_OK)
      throw db_exception("Close", "main", ret, std::string(sqlite3_errmsg(db)));
  }
  catch(const db_exception& e)
  {
    std::cerr << e.what() << "\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
