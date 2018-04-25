#pragma once
#include "CursesClass.h"
#include <fstream>
#include <string>

using namespace std;

typedef enum MODE
{
	READ, WRITE
};

MODE selectMode(CursesClass& curses);

string askForFile(CursesClass& curses);

bool fileExists(const char * fileName);

void fileDoesntExist(CursesClass& curses, string fileName);

bool askToOverwrite(CursesClass& curses);