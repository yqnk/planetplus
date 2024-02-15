#ifndef COMMANDS_H
#define COMMANDS_H

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
 * Create the default configuration file.
 * Create the default database.
 * Create the default tables.
 * Create the default logs.
 */
void planetplus_setup();

/**
 * @brief Print the current version of the planetplus server/instance.
 */
void planetplus_version();
}

#endif