//standard libraries
#include <iostream>

// clear the screen according to OS
void clear_screen()
{
#ifdef _WIN32
  std::system("cls");
#else
  std::system("clear");
#endif
}

