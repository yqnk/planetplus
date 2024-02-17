#include "commands.h"

#include <filesystem>
#include <fstream>
#include <string>

#include "cli/tools.h"
#include "utils/ini_parser.h"

namespace
{
/**
 * @brief Check if user has mariadb installed
 *
 * @return int
 */
int check_for_mariadb()
{
#ifdef _WIN32
    return system("mariadb --version");
#else
    return system("mariadb --version");
#endif
}

int check_for_curl()
{
#ifdef _WIN32
    return system("curl --version");
#else
    return system("curl --version");
#endif
}

/**
 * @brief Creates the default configuration file.
 *
 * @param filePath The path to the configuration file.
 */
void create_default_config_file(const std::string& filePath)
{
    std::string configContent = R"(
; This is the default configuration.
; Feel free to change it to your liking.
[planetplus]
description = This server uses PlanetPlus !
owner = your-id-here ; you will be MasterAdmin automatically

[server]
host =
port =
login =
password =

[database]
host = localhost ; or whatever your database host is
port = 3306 ; or whatever your database port is
user=mysql-user ; or whatever your database user is
password=mysql-password ; or whatever your database password is
name = planetplus

[masteradmins]      ; the order doesn't matter
id[] = your-id-here ; id of the first masteradmin
id[] = your-id-here ; id of the second masteradmin
                    ; and so on...

[admins]            ; the order doesn't matter
id[] = your-id-here ; id of the first admin
id[] = your-id-here ; id of the second admin
                    ; and so on...
)";

    std::ofstream config_file(filePath);
    if (config_file.is_open())
    {
        config_file << configContent;
        config_file.close();
    }
    else
    {
        cli_tools::print_warning(
            "! Failed to create default configuration file: " +
            cli_tools::bold(filePath));
    }
}
}

namespace cli_commands
{
void planetplus_setup()
{
    cli_tools::clear_screen();
    std::cout << "> Setting up PlanetPlus...\n" << std::endl;

    std::string base_dir_path = BASE_DIR;
    base_dir_path += "planetplus/";

#ifndef _WIN32
    base_dir_path = std::getenv("HOME") + base_dir_path;
#endif

    if (std::filesystem::exists(base_dir_path))
    {
        cli_tools::print_warning(
            "! Directory already exists: " + cli_tools::bold(base_dir_path));
        if (cli_tools::confirm_action(
                "Do you want to overwrite it?\nThis will erase everything in "
                "the planetplus folder !"))
        {
            if (cli_tools::remove_dir(base_dir_path.c_str()) == 0)
            {
                cli_tools::print_success(
                    "Removed directory: " + cli_tools::bold(base_dir_path));
                putchar('\n');
            }
            else
            {
                cli_tools::print_error("! Failed to remove directory: " +
                    cli_tools::bold(base_dir_path));
                return;
            }
        }
    }

    if (cli_tools::create_dir(base_dir_path.c_str()) == 0)
    {
        cli_tools::print_success(
            "Created directory: " + cli_tools::bold(base_dir_path));
    }
    else
    {
        cli_tools::print_error(
            "! Failed to create directory: " + cli_tools::bold(base_dir_path));
        return;
    }

    std::string config_path = base_dir_path + "config/";

    if (cli_tools::create_dir(config_path.c_str()) == 0)
    {
        cli_tools::print_success(
            "Created directory: " + cli_tools::bold(config_path));
    }
    else
    {
        cli_tools::print_error(
            "! Failed to create directory: " + cli_tools::bold(config_path));
        return;
    }

    // create default configuration file
    create_default_config_file(config_path + "config.ini");

    std::string logs_path = base_dir_path + "logs/";

    if (cli_tools::create_dir(logs_path.c_str()) == 0)
    {
        cli_tools::print_success(
            "Created directory: " + cli_tools::bold(logs_path));
    }
    else
    {
        cli_tools::print_error(
            "! Failed to create directory: " + cli_tools::bold(logs_path));
        return;
    }

    // create log files
    std::string server_log_file = logs_path + "server.log";
    std::string chat_log_file = logs_path + "chat.log";

    if (cli_tools::create_file(server_log_file.c_str(), "w") == 0)
    {
        cli_tools::print_success(
            "Created file: " + cli_tools::bold(server_log_file));
    }
    else
    {
        cli_tools::print_warning(
            "! Failed to create file: " + cli_tools::bold(server_log_file));
        return;
    }

    if (cli_tools::create_file(chat_log_file.c_str(), "w") == 0)
    {
        cli_tools::print_success(
            "Created file: " + cli_tools::bold(chat_log_file));
    }
    else
    {
        cli_tools::print_warning(
            "! Failed to create file: " + cli_tools::bold(chat_log_file));
        return;
    }

    // Check for requirements

    std::cout << "\n> Checking for requirements..." << std::endl;

    // Check for MariaDB
    std::cout << "\nChecking for MariaDB..." << std::endl;
    if (check_for_mariadb() == 0)
    {
        cli_tools::print_success("MariaDB is installed !");
    }
    else
    {
        cli_tools::print_error(
            "! MariaDB is not installed !\nPlease install MariaDB and try "
            "again.\n");
#ifdef _WIN32
        std::cout << "https://www.mariadbtutorial.com/getting-started/"
                     "install-mariadb/"
                  << std::endl;
#endif
        return;
    }

    // Check for cURL
    std::cout << "\nChecking for cURL..." << std::endl;
    if (check_for_curl() == 0)
    {
        cli_tools::print_success("cURL is installed !");
    }
    else
    {
        cli_tools::print_error(
            "! cURL is not installed !\nPlease install cURL and try again.\n");
#ifdef _WIN32
        std::cout << "cURL already installed ? Try adding it to your PATH\n"
                     "https://curl.se/windows/"
                  << std::endl;
#endif
    }

    // Creating database folder
    std::cout << "\n> Creating database...\n" << std::endl;

    std::string database_path = base_dir_path + "database/";

    if (cli_tools::create_dir(database_path.c_str()) == 0)
    {
        cli_tools::print_success(
            "Created directory: " + cli_tools::bold(config_path));
    }
    else
    {
        cli_tools::print_error(
            "! Failed to create directory: " + cli_tools::bold(config_path));
        return;
    }

    std::string user = cli_tools::get_input("Enter your MariaDB username: ");
    std::string password =
        cli_tools::get_input("Enter your MariaDB password: ");

    config::IniParser ini(config_path + "config.ini");
    ini.set("database", "user", user);
    ini.set("database", "password", password);
}
} // namespace cli_commands