#include "commands.h"

#include <iostream>

#include "cli/tools.h"
#include "cversion.h"

namespace cli_commands
{
void planetplus_version()
{
    std::cout << "planetplus version "
              << cli_tools::bold(Version::current().asShortStr()) << std::endl;
}
}
