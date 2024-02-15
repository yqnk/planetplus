#include <iostream>
#include <string>

#include "cli/commands/commands.h"
#include "cli/tools.h"

using namespace std;

int main(int argc, char const* argv[])
{
    // Print help if no arguments are given
    if (argc == 1)
        return CLI_EXIT_FAILURE;

    // process parameters
    int argIt;
    for (argIt = 1; argIt < argc; ++argIt)
    {
        string tmp = argv[argIt];

        if (tmp == "--version" || tmp == "-v")
            cli_commands::planetplus_version();
        else if (tmp == "--setup")
            cli_commands::planetplus_setup();
        else
            break;
    }
    return 0;
}
