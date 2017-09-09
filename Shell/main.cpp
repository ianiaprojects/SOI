#include <iostream>
#include "inputmanager.h"
#include "environment.h"
using namespace std;

int main(int argc, char** argv, char** env)
{
    Environment environment(env);
    InputManager in(&environment);
    in.runShell();

    return 0;
}

