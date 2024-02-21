#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>

#ifdef _WIN32
#include <windows.h>
#define BASE_DIR "C:\\"
#else
#include <unistd.h>
#define BASE_DIR "/.local/share/"
#endif

namespace cli_commands
{
/**
 * @brief Set the up planetplus server/instance.
*          - Create the default configuration file.
*          - Create the default database.
*          - Create the default tables.
*          - Create the default logs.
 */
void planetplus_setup();

/**
 * @brief Print the current version of the planetplus server/instance.
 */
void planetplus_version();

/**
 * @brief Change config values
 *
 */
int planetplus_get_config(std::string section, std::string key);

/**
 * @brief Change config values
 *
 */
int planetplus_set_config(
    std::string section, std::string key, std::string value);
}

#endif