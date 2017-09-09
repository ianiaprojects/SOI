#include "environment.h"

Environment::Environment(char** env) :
    mEnvVars(),
    mPrompt(),
    mEnvMatrix(env)
{
    initVars();
    chdir(getVariable("mCurrentPath").c_str());
    mPrompt = "[" + getVariable("USER") + "@localhost " + getVariable("mCurrentDir") + "] $ ";
}


void Environment::cdCommand(string dirPath)
{
    cout << dirPath;
    if(dirPath == "~" || dirPath.empty())
        dirPath = getVariable("HOME");

    if(chdir(dirPath.c_str()) != 0)
        cout << "Directory: " << dirPath << " is invalid!";
}


string& Environment::getVariable(const string& varName)
{
    return mEnvVars[varName];
}

void Environment::exitConsole(const string& message)
{
    cerr << message << endl;
    exit(1);
}

void Environment::initVars()
{
    string currentVar;

    auto splitVar = [](string& i) {
        stringstream stream(i);
        string temp;
        pair<string, string> tmp;

        getline(stream, temp, '=');
        tmp.first = temp;
        getline(stream, temp, '=');
        tmp.second = temp;

        return tmp;
    };

    for(int i = 0; mEnvMatrix[i] != nullptr; ++i)
    {
        currentVar = mEnvMatrix[i];
        pair<string, string> var = splitVar(currentVar);
        mEnvVars.insert(var);
    }
    mEnvVars["mCurrentDir"] = "~";
    mEnvVars["mRootPath"] = "/";
    mEnvVars["mCurrentPath"] = string(getVariable("HOME"));
}

string& Environment::getCurrentDirectory()
{
    return mEnvVars["mCurrentDir"];
}
