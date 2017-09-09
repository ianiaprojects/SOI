#include "inputmanager.h"

InputManager::InputManager(Environment *env) : commands(), input(), environment(env)
{
    rl_bind_key('\t', rl_complete);
}

void InputManager::runShell()
{
    for(;;)
    {
        input = readline("$> ");

        if(input.empty())
            continue;

        add_history(input.c_str());

        splitInput();
        runInput();
    }
}

void InputManager::splitInput()
{
    stringstream sstream(input);
    string temp;
    commands.clear();

    while(getline(sstream, temp, '|'))
    {
        if(temp.empty() == false && temp != " ")
            commands.push_back(Command(temp, environment));
    }
}

void InputManager::runInput()
{
    int in, fd[2];
    in = STDIN_FILENO;

    for(Command& it : commands)
    {
        pipe(fd);

        it.run(in, fd[1]);
        close(fd[1]);
        in = fd[0];
    }

    string res;

    char buf[1024];
    FILE* stream = fdopen(in, "r");
    while(fgets(buf, sizeof(buf), stream))
        res += buf;
    fclose(stream);
    cout << res;

}
