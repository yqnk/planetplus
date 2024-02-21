#include <iostream>

namespace cli_commands
{
void planetplusHelp()
{
    std::cout << "Usage: planetplus [OPTION]...\n"
                 "Planetplus is a server manager for ManiaPlanet 4.\n"
                 "\n"
                 "  -h, --help     display this help and exit\n"
                 "  -v, --version  output version information and exit\n"
                 "  --setup        setup the planetplus server\n"
                 "  --test         test the planetplus server\n"
                 "  --get-config   get the value of a configuration key\n"
                 "  --set-config   set the value of a configuration key\n"
                 "\n"
                 "Please report bugs on GitHub or on Discord (DISCORD_INVITE_LINK)."
              << std::endl;
}
} // namespace cli_commands