#include "command.h"

Command::Command(Environment* environment) :
    commands(),
    input(),
    isShellCommand(true),
    environment(environment),
    redirectMode(RedirectMode::NONE),
    redirectFile(NULL)
{}

Command::Command(const string &command, Environment* environment) :
    Command(environment)
{
    setInput(command);
    splitCommand();
    setType();
    setRedirectMode();
}

void Command::splitCommand()
{
    stringstream sstream(input);
    string temp;
    commands.clear();

    while(getline(sstream, temp, ' '))
        if(temp.empty() == false)
            commands.push_back(temp);
}

void Command::setType()
{
    string cmd = *commands.begin();
    if(cmd == "exit" || cmd == "sort" || cmd == "basename" || cmd == "dir")
        isShellCommand = false;
    else
        isShellCommand = true;
}

void Command::run(int in, int out)
{
    if(*commands.begin() == "cd")
        environment->cdCommand(getArguments());

    if(isShellCommand)
        runShellCommand(in, out);
    else
        runExternalCommand();

    if(redirectMode != RedirectMode::NONE)
    {
        fprintf(redirectFile, "%s", output.c_str());
        output = string();
        fclose(redirectFile);
    }
}

void Command::runShellCommand(int in, int out)
{
    pid_t pid;

    if((pid = fork ()) == 0)
    {
        if(in != 0)
        {
            dup2(in, STDIN_FILENO);
            close(in);
        }

        if(out != 1)
        {
            dup2(out, STDOUT_FILENO);
            close(out);
        }

        char** arg = getCommand();
        array<char*, 2> path;
        path.at(0) = getenv("PATH");
        path.at(1) = NULL;
        execvp(arg[0], arg);
        if(errno == 2)
            environment->exitConsole(string(arg[0]) + ": No such file ");
        else
            environment->exitConsole("execvpe error");

    }
    else if(pid < 0)
    {
        int s;
        waitpid(pid, &s, 0);
        char buf[1024];
        FILE* stream = fdopen(in, "r");
        while(fgets(buf, sizeof(buf), stream))
            output += buf;
        fclose(stream);
    }

}

void Command::runExternalCommand()
{
    if(*commands.begin() == "exit")
        environment->exitConsole("");

    if(*commands.begin() == "sort")
        runSortCommand();

    if(*commands.begin() == "basename")
        runBasenameCommand();

    if(*commands.begin() == "dir")
        runDirCommand();
}

void Command::runSortCommand()
{
    //find flags
    auto whereR = find(commands.begin(), commands.end(), "-r");
    auto whereN = find(commands.begin(), commands.end(), "-n");
    auto whereRN = find(commands.begin(), commands.end(), "-rn");
    auto whereNR = find(commands.begin(), commands.end(), "-nr");

    //read input
    //check commands arguments
    bool isFromConsole = true;
    for(int i = 1; i < commands.size(); i++)
    {
        string pathName = commands.at(i);
        if(pathName != "sort"
                && pathName != "-r"
                && pathName != "-n"
                && pathName != "-nr"
                && pathName != "-rn"
                && pathName != "")
            isFromConsole = false;
    }

    vector<string> lines;

    if(isFromConsole)
    {
        string input;
        char ch;
        do {
            ch = getchar();
            if(ch == '\n')
            {
                input += ch;
                lines.push_back(input);
                input.clear();
            }
            else
                input += ch;
        } while(ch != EOF);
    }
    else
    {
        for(int i = 1; i < commands.size(); i++)
        {
            string pathName = commands.at(i);
            ifstream in(pathName);

            string line;
            if(pathName != "sort"
                    && pathName != "-r"
                    && pathName != "-n"
                    && pathName != "-nr"
                    && pathName != "-rn"
                    && pathName != ""
                    && !in.is_open())
                cerr << "sort: cannot read: " << commands.at(i) << " : No such file or directory";

            while(getline(in, line))
                lines.push_back(line);

            in.close();
        }
    }

    //sort input
    auto lambda = [](const string & a, const string & b) -> bool
    {
        return a < b;
    };

    if(whereR != commands.end())
    {
        if(whereN != commands.end())
        {
            sort(lines.begin(), lines.end(), lambda);
            reverse(lines.begin(), lines.end());
        }
        else
        {
            sort(lines.begin(), lines.end());
            reverse(lines.begin(), lines.end());
        }
    }
    else
    {
        if((whereN != commands.end()))
            sort(lines.begin(), lines.end(),lambda);
        else
            sort(lines.begin(), lines.end());
    }

    if(whereNR != commands.end() || whereRN != commands.end())
    {
        sort(lines.begin(), lines.end(), lambda);
        reverse(lines.begin(), lines.end());
    }

    // display sorted
    for(int i = 0; i < lines.size(); i++)
        cout << lines.at(i);
    lines.clear();
}

void Command::runBasenameCommand()
{
    if(commands.size() == 1)
        cerr << "basename: missing operand";
    else
    {
        string result = commands.at(1);

        int i = 0;
        for(i = result.size()-1; i > 0 && result.at(i) != '/'; i--);

        for(int j = i+1; j < result.size(); j++)
            cout << result.at(j);
        cout << endl;
    }
}

void Command::runDirCommand()
{
    DIR *pDIR;
    struct dirent *entry;

    vector<char*> result;

    if(commands.size() < 1)
    {
        string currentDirectory = environment->getCurrentDirectory();
        if(pDIR = opendir(currentDirectory.c_str()))
        {
            while(entry = readdir(pDIR)){
                if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && entry->d_name[0] != '.')
                {
                    result.push_back(entry->d_name);
                }
            }

            closedir(pDIR); //close_file

            sort(result.begin(), result.end(), [] (const char* a, const char* b) {
                return a < b;
            });

            for(const string& i : result)
                cout << i << "\t";

        }//end_if
    }
    else
    {
        for(int i = 1; i < commands.size(); i++)
        {
            cout << commands.at(i) << " ";
            if( pDIR=opendir(commands.at(i).c_str()) ){
                while(entry = readdir(pDIR))
                {
                    if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && entry->d_name[0] != '.')
                    {
                        result.push_back(entry->d_name);
                    }
                }
                closedir(pDIR); //close_file
                sort(result.begin(), result.end(), [] (const char* a, const char* b) {
                    return a < b;
                });

                for(int i = 0; i < result.size(); i++)
                    cout << result.at(i) << "\t";

                result.clear();
                cout << endl;
            }
        }//end_for
    }
}

char **Command::getCommand()
{
    char** retVal = new char*[commands.size() + 1];

    for(int i = 0; i < commands.size(); ++i)
    {
        retVal[i] = new char[commands.at(i).size()];
        strcpy(retVal[i], commands.at(i).c_str());
    }

    retVal[commands.size()] = NULL;

    return retVal;
}

void Command::addInput(string input)
{
    commands.push_back(input);
}

void Command::setInput(const string &value)
{
    input = value;
}

string Command::getArguments()
{
    string retValue;

    for(auto it = ++commands.begin(); it != commands.end(); it++)
        if(it != --commands.end())
            retValue += *it + " ";
        else
            retValue += *it;

    return retValue;
}

void Command::setRedirectMode()
{
    for(string& i : commands)
        if(i == ">")
            redirectMode = RedirectMode::WRITE;
        else if(i == ">>")
            redirectMode = RedirectMode::APPEND;

    if(redirectMode == RedirectMode::WRITE)
    {
        auto it = find(commands.begin(), commands.end(), ">");
        redirectFile = fopen((*(++it)).c_str(), "w");
        commands.erase(--it, commands.end());
    }
    else if(redirectMode == RedirectMode::APPEND)
    {
        auto it = find(commands.begin(), commands.end(), ">>");
        redirectFile = fopen((*(++it)).c_str(), "a");
        commands.erase(--it, commands.end());
    }
}
