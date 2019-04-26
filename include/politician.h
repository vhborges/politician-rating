#ifndef POLITICIAN_H
#define POLITICIAN_H

//standard libraries
#include <iostream>

//external libraries
#include <system.h>

enum confirm
{
  yes,
  no,
  cancel
};

struct politician_core
{
  std::string name;
  std::string party;

  void get_data();
  confirm confirm_data(const std::string& operation);
};

struct politician_update : politician_core
{
  std::string new_party;

  void get_data();
};

struct politician : politician_core
{
  std::string info;

  void get_data();
};

struct rating : politician
{
  short points;
  std::string description;

  void get_data();
};

template <typename entity>
bool new_entity(entity& e, const std::string& operation)
{
  bool return_value;
  bool loop;
  do
  {
    loop = false;
    while(true)
    {
      try
      {
        e.get_data();
        break;
      }
      catch(const char* e)
      {
        clear_screen();
        std::cout << "--> Error: " << e << "\n\n";
      }
    }
    while(true)
    {
      try
      {
        confirm c = e.confirm_data(operation);
        switch(c)
        {
          case yes:
            return_value = true;
            break;

          case no:
            clear_screen();
            loop = true;
            break;

          case cancel:
            clear_screen();
            return_value = false;
            break;
        }
        break;
      }
      catch(const char* e)
      {
        clear_screen();
        std::cout << "--> Error: " << e << "\n\n";
      }
    }
  }
  while(loop);

  return return_value;
}

#endif
