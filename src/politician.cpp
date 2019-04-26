//standard libraries
#include <limits>

//local headers
#include <politician.h>

void politician_core::get_data()
{
  std::cout << "Politician's name: ";
  std::getline(std::cin, name);
  if(name == "")
    throw "Politicians's name cannot be empty.";

  std::cout << "Politician's party: ";
  std::getline(std::cin, party);
  if(party == "")
    throw "Politician's party cannot be empty.";
}

void politician::get_data()
{
  politician_core::get_data();

  std::cout << "General information (optional - press enter to ignore):\n\t";
  std::getline(std::cin, info);
}

void politician_update::get_data()
{
  std::cout << "Name: ";
  std::getline(std::cin, name);
  if(name == "")
    throw "Politicians's name cannot be empty.";

  std::cout << "Old party: ";
  std::getline(std::cin, party);
  if(party == "")
    throw "Politician's party cannot be empty.";

  std::cout << "New party: ";
  std::getline(std::cin, new_party);
  if(party == "")
    throw "Politician's party cannot be empty.";
}

confirm politician_core::confirm_data(const std::string& operation)
{
  std::string confirm;
  std::cout << "\nConfirm " << operation << "? [y]es/[n]o/[c]ancel ";
  std::getline(std::cin, confirm);

  if(confirm == "y" || confirm == "Y")
    return yes;
  else if(confirm == "n" || confirm == "N")
    return no;
  else if(confirm == "c" || confirm == "C")
    return cancel;
  else
    throw "Invalid option.";
}

void rating::get_data()
{
  politician_core::get_data();

  bool valid;
  do
  {
    valid = true;
    std::cout << "Points to add/subtract [-5 to 5]: ";
    std::cin >> points;
    if(std::cin.fail())
    {
      std::cout << "--> Invalid entry. Type only a number!\n";
      valid = false;
      std::cin.clear();
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  while(!valid);
  if(points < -5 || points > 5)
    throw "Points must be between -5 and 5 (included).";
  
  std::cout << "Description or reason for this rate (optional - press enter to ignore):\n\t";
  std::getline(std::cin, description);
}

