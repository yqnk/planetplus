#include "commands.h"

#include <string>
#include <vector>

#include "cli/tools.h"
#include "utils/config.h"

namespace cli_commands
{
int planetplusGetConfig(std::string section, std::string key)
{
    std::string base_dir_path = BASE_DIR;
    base_dir_path += "planetplus/";

#ifndef _WIN32
    base_dir_path = std::getenv("HOME") + base_dir_path;
#endif

    config::Config config(base_dir_path + "config/config.conf");
    config.load();

    std::string value = config.get(section, key);

    std::cout << section << "." << key << " = " << cli_tools::bold(value) << std::endl;
    
    config.save();
    return CLI_EXIT_SUCCESS;
}

int PlanetplusSetConfig(
    std::string section, std::string key, std::string value)
{
    std::string base_dir_path = BASE_DIR;
    base_dir_path += "planetplus/";

#ifndef _WIN32
    base_dir_path = std::getenv("HOME") + base_dir_path;
#endif

    config::Config config(base_dir_path + "config/config.conf");
    config.load();

    config.set(section, key, value);
    config.save();

    std::cout << section << "." << key << " = " << cli_tools::bold(value) << std::endl;

    return CLI_EXIT_SUCCESS;
}
} // namespace cli_commands