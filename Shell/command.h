#ifndef COMMAND_H
#define COMMAND_H
#include "environment.h"
#include <dirent.h>
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fstream>
#include <readline/readline.h>
#include <map>
#include <array>
#include <errno.h>
using namespace std;
enum class RedirectMode {
    NONE,
    WRITE,
    APPEND
};

class Command
{
public:
    Command(Environment* environment);
    Command(const string& command, Environment* environment);

    void run(int in, int out);
    void addInput(string input);

    void setInput(const string &value);

private:
    Environment* environment;

    string input;
    string output;
    vector<string> commands;
    bool isShellCommand;
    RedirectMode redirectMode;
    FILE* redirectFile;
    void splitCommand();

    void setType();
    void runShellCommand(int in, int out);
    void runExternalCommand();

    void runSortCommand();
    void runBasenameCommand();
    void runDirCommand();

    char** getCommand();
    string getArguments();

    void setRedirectMode(); // partially working
};

#endif // COMMAND_H
