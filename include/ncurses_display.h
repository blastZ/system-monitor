#ifndef NCURSES_DISPLAY_H
#define NCURSES_DISPLAY_H

#include <curses.h>

#include "process.h"
#include "system.h"

namespace NCursesDisplay {
void Display(System &system, int n = 10);
void DisplaySystem(System &system, WINDOW *window);
void DisplayProcess(std::vector<Process> &processes, WINDOW *window, int n);
std::string PorgressBar(float percent);
};  // namespace NCursesDisplay

#endif