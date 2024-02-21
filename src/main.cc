#include <iostream>
#include <string>

#include "cli/commands/commands.h"
#include "cli/tools.h"
#include "utils/config.h"

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
        else if (tmp == "--test")
        {
            std::string value = "\"planetplus\"";
            while (value[0] == '"' || value[0] == '\'')
            {
                value.erase(0, 1);
            }

            while (value[value.size() - 1] == '"' ||
                value[value.size() - 1] == '\'')
            {
                value.erase(value.size() - 1, 1);
            }
            std::cout << value << std::endl;
        }
        else if (tmp == "--get-config")
        {
            if (argIt + 2 >= argc)
            {
                cli_tools::print_error("Not enough arguments for --get-config");
                return CLI_EXIT_FAILURE;
            }
            argIt++;
            cli_commands::planetplus_get_config(argv[argIt], argv[argIt + 1]);
            argIt++;
        }
        else if (tmp == "--set-config")
        {
            if (argIt + 3 >= argc)
            {
                cli_tools::print_error("Not enough arguments for --set-config");
                return CLI_EXIT_FAILURE;
            }
            argIt++;
            cli_commands::planetplus_set_config(
                argv[argIt], argv[argIt + 1], argv[argIt + 2]);
            argIt += 2;
        }
        else
        {
            cli_tools::print_error("Unknown argument: " + tmp);
            return CLI_EXIT_FAILURE;
        }
        break;
    }
    return 0;
}
