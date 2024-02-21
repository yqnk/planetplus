#include "commands.h"

#include <iostream>

#include "cli/tools.h"
#include "version.h"

namespace cli_commands
{
void planetplusVersion()
{
    std::cout << "planetplus version "
              << cli_tools::bold(Version::current().asShortStr()) << std::endl;
}
}
