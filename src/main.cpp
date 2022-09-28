#include <iostream>

#include "../include/linux_parser.h"
#include "../include/ncurses_display.h"
#include "../include/system.h"

int main() {
  System system;

  // std::cout << LinuxParser::MemoryUtilization() << std::endl;

  NCursesDisplay::Display(system);
}