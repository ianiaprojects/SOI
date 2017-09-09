#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <unistd.h>
using namespace std;

class Environment
{
public:
    Environment(char** env);
    ~Environment()=default;

    void cdCommand(string dirPath); //cd not working
    string& getVariable(const string& varName);
    string& getCurrentDirectory();

    void exitConsole(const string& message = string());
private:
    map<string, string> mEnvVars;
    string mPrompt;
    char** mEnvMatrix;
    void initVars();
};

#endif // ENVIRONMENT_H
