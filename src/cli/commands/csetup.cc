#include "commands.h"

#include <filesystem>
#include <fstream>
#include <string>

#include "cli/tools.h"
#include "utils/config.h"
#include "utils/utils.h"

namespace
{
/**
 * @brief Check if user has mariadb installed
 *
 * @return int
 */
int checkForMariadb()
{
#ifdef _WIN32
    return system("mariadb --version");
#else
    return system("mariadb --version");
#endif
}

int checkForCurl()
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
void createDefaultConfigFile(const std::string& filePath)
{
    std::string configContent = R"(
# This is the default configuration.
# This file is automatically generated. Do not modify it unless you know what you are doing.

[planetplus]
description = "This server uses PlanetPlus !"
owners = ""
masteradmins = ""
admins = ""

[database]
name = "planetplus"
host = "localhost"
port = "3306"
user = "mariadb-user"
password = "mariadb-password"

[server]
host = ""
port = ""
login = ""
password = ""
)";

    std::ofstream config_file(filePath);
    if (config_file.is_open())
    {
        config_file << configContent;
        config_file.close();
    }
    else
    {
        cli_tools::printWarning(
            "! Failed to create default configuration file: " +
            cli_tools::bold(filePath));
    }
}
} // namespace

namespace cli_commands
{
void planetplusSetup()
{
    cli_tools::clearScreen();
    std::cout << "> Setting up PlanetPlus...\n" << std::endl;

    std::string base_dir_path = BASE_DIR;
    base_dir_path += "planetplus/";

#ifndef _WIN32
    base_dir_path = std::getenv("HOME") + base_dir_path;
#endif

    if (std::filesystem::exists(base_dir_path))
    {
        cli_tools::printWarning(
            "! Directory already exists: " + cli_tools::bold(base_dir_path));
        if (cli_tools::confirmAction(
                "Do you want to overwrite it?\nThis will erase everything in "
                "the planetplus folder !"))
        {
            if (cli_tools::removeDir(base_dir_path.c_str()) == 0)
            {
                cli_tools::printSuccess(
                    "Removed directory: " + cli_tools::bold(base_dir_path));
                putchar('\n');
            }
            else
            {
                cli_tools::printError("! Failed to remove directory: " +
                    cli_tools::bold(base_dir_path));
                return;
            }
        }
    }

    if (cli_tools::createDir(base_dir_path.c_str()) == 0)
    {
        cli_tools::printSuccess(
            "Created directory: " + cli_tools::bold(base_dir_path));
    }
    else
    {
        cli_tools::printError(
            "! Failed to create directory: " + cli_tools::bold(base_dir_path));
        return;
    }

    std::string config_path = base_dir_path + "config/";

    if (cli_tools::createDir(config_path.c_str()) == 0)
    {
        cli_tools::printSuccess(
            "Created directory: " + cli_tools::bold(config_path));
    }
    else
    {
        cli_tools::printError(
            "! Failed to create directory: " + cli_tools::bold(config_path));
        return;
    }

    // create default configuration file
    createDefaultConfigFile(config_path + "config.conf");

    std::string logs_path = base_dir_path + "logs/";

    if (cli_tools::createDir(logs_path.c_str()) == 0)
    {
        cli_tools::printSuccess(
            "Created directory: " + cli_tools::bold(logs_path));
    }
    else
    {
        cli_tools::printError(
            "! Failed to create directory: " + cli_tools::bold(logs_path));
        return;
    }

    // create log files
    std::string server_log_file = logs_path + "server.log";
    std::string chat_log_file = logs_path + "chat.log";

    if (cli_tools::createFile(server_log_file.c_str(), "w") == 0)
    {
        cli_tools::printSuccess(
            "Created file: " + cli_tools::bold(server_log_file));
    }
    else
    {
        cli_tools::printWarning(
            "! Failed to create file: " + cli_tools::bold(server_log_file));
        return;
    }

    if (cli_tools::createFile(chat_log_file.c_str(), "w") == 0)
    {
        cli_tools::printSuccess(
            "Created file: " + cli_tools::bold(chat_log_file));
    }
    else
    {
        cli_tools::printWarning(
            "! Failed to create file: " + cli_tools::bold(chat_log_file));
        return;
    }

    // Check for requirements

    std::cout << "\n> Checking for requirements..." << std::endl;

    // Check for MariaDB
    std::cout << "\nChecking for MariaDB..." << std::endl;
    if (checkForMariadb() == 0)
    {
        cli_tools::printSuccess("MariaDB is installed !");
    }
    else
    {
        cli_tools::printError(
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
    if (checkForCurl() == 0)
    {
        cli_tools::printSuccess("cURL is installed !");
    }
    else
    {
        cli_tools::printError(
            "! cURL is not installed !\nPlease install cURL and try again.\n");
#ifdef _WIN32
        std::cout << "cURL already installed ? Try adding it to your PATH\n"
                     "https://curl.se/windows/"
                  << std::endl;
#endif
    }

    // Creating database folder
    std::cout << "\n> Creating database folder...\n" << std::endl;

    std::string database_path = base_dir_path + "database/";

    if (cli_tools::createDir(database_path.c_str()) == 0)
    {
        cli_tools::printSuccess(
            "Created directory: " + cli_tools::bold(config_path));
    }
    else
    {
        cli_tools::printError(
            "! Failed to create directory: " + cli_tools::bold(config_path));
        return;
    }

    // Database setup
    std::cout << "\n> Setting up Config file..." << std::endl;
    cli_tools::printInfo("Leave blank (whitespace) if you don't know, you will be able to change it later. See --help.\n");

    config::Config config(config_path + "config.conf");
    config.load();

    std::string owner = cli_tools::getInput("Enter the server owner ID: ");

    std::string db_host = cli_tools::getInput("Enter your MariaDB host: ");
    std::string db_port = cli_tools::getInput("Enter your MariaDB port: ");
    std::string db_user = cli_tools::getInput("Enter your MariaDB username: ");
    std::string db_password =
        cli_tools::getInput("Enter your MariaDB password: ");

    std::cout << "\n> Saving database credentials..." << std::endl;

    utils::trim(db_host);
    utils::trim(db_port);

    if (db_host.empty())
    {
        db_host = "localhost";
    }

    if (db_port.empty())
    {
        db_port = "3306";
    }
    
    config.set("database", "host", db_host);
    config.set("database", "port", db_port);
    config.set("database", "user", db_user);
    config.set("database", "password", db_password);
    config.save();
    cli_tools::printSuccess("Database credentials saved !\n");

    // Server setup
    std::string server_host = cli_tools::getInput("Enter your server host: ");
    std::string server_port = cli_tools::getInput("Enter your server port: ");
    std::string server_login =
        cli_tools::getInput("Enter your server login: ");
    std::string server_password =
        cli_tools::getInput("Enter your server password: ");

    std::cout << "\n> Saving server credentials..." << std::endl;
    config.set("server", "host", server_host);
    config.set("server", "port", server_port);
    config.set("server", "login", server_login);
    config.set("server", "password", server_password);
    config.save();
    cli_tools::printSuccess("Server credentials saved !\n");
    cli_tools::printSuccess("Configuration can be found in " + cli_tools::bold(config_path + "config.conf"));
}
} // namespace cli_commands