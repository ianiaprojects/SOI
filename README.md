# SOI
The application should offer the following facilities:

    * Accept user commands from the command line. The interpretor will expose a specific prompt (e.g. '$>') showing that it is prepared to accept a user command.
    * Execute user commands, from the list of accepted commands. The following error situations MUST be considered:
          o Command execution returned an error. The exact error code must be reported.
          o Issued command is not an accepted command. Error information will contain 'offending' command name.
          o Arguments are in a wrong format: the only accepted format is "-o ARG", where ARG is the optional argument of your option. It is not accepted to use the '/' notation for options or the --option long notation for options. These errors should be identified during preprocessing of command line.
    * Below is a list of _internal_ commands that you need to implement yourself. These will be simple function calls in your program. 
    * All the other commands that your shell needs to support are called _external_ commands. They will be running in a separate process, using the fork/exec workflow discussed during the labs. You shall still provide error reporting for these commands.
