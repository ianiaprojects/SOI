#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include "command.h"
#include "environment.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <list>
using namespace std;

class InputManager
{
public:
    InputManager(Environment *);
    ~InputManager() = default;

    void runShell();

private:
    string input;
    vector<Command> commands;
    Environment* environment;

    void splitInput();
    void runInput();
};

#endif // INPUTMANAGER_H
