#include <iostream>

#include "../include/ncurses_display.h"
#include "../include/system.h"

int main() {
  System system;

  // std::cout << system.OperatingSystem() << std::endl;

  NCursesDisplay::Display(system);
}