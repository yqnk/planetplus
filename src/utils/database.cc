#include "database.h"

#include <mysql/mysql.h>

#include <string>
#include <fstream>

#include "cli/tools.h"
#include "utils/config.h"

namespace database
{
Manager::Manager(std::string config_path, config::Config* config)
{
    this->host = config->get("database", "host");
    this->user = config->get("database", "user");
    this->name = config->get("database", "name");
    this->port = config->get("database", "port");
    this->password = config->get("database", "password");
}

Manager::~Manager()
{
    if (!this->disconnected_)
    {
        cli_tools::print_warning("Did not disconnect from the database, some "
                                 "changes may not have been saved.");
        cli_tools::print_info("-> This is a mistake from the developer of the "
                              "project, please report it.");
        this->disconnect();
    }
}

bool Manager::connect()
{
    this->conn = mysql_init(nullptr);
    if (this->conn == nullptr)
    {
        cli_tools::print_error("!! mysql_init() failed");
        return false;
    }

    if (mysql_real_connect(this->conn, this->host.c_str(), this->user.c_str(),
                           this->password.c_str(), this->name.c_str(),
                           std::stoi(this->port), nullptr, 0) == nullptr)
    {
        cli_tools::print_error("!! mysql_real_connect() failed");
        return false;
    }

    cli_tools::print_success("Connection to database established.");
    return true;
}

void Manager::disconnect()
{
    mysql_close(this->conn);
    cli_tools::print_success("Disconnected from database.");
    this->disconnected_ = true;
}

int Manager::executeQuery(const std::string& query)
{
    if (this->disconnected_)
    {
        cli_tools::print_error("!! Database is disconnected");
        return -1;
    }

    if (this->conn == nullptr)
    {
        cli_tools::print_error("!! Database connection returned nullptr.");
        return -1;
    }

    if (mysql_query(this->conn, query.c_str()))
    {
        cli_tools::print_error("!! mysql_query() failed");
        return -1;
    }

    cli_tools::print_success("Query executed successfully.");
    return 0;
}

int Manager::executeFromFile(const std::string& file_path)
{
    if (this->disconnected_)
    {
        cli_tools::print_error("!! Database is disconnected");
        return -1;
    }

    if (this->conn == nullptr)
    {
        cli_tools::print_error("!! Database connection returned nullptr.");
        return -1;
    }

    std::string query;
    std::string line;
    std::ifstream file(file_path);
    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            query += line;
        }
        file.close();
    }
    else
    {
        cli_tools::print_error("!! Unable to open file: " + file_path);
        return -1;
    }

    if (mysql_query(this->conn, query.c_str()))
    {
        cli_tools::print_error("!! mysql_query() failed");
        return -1;
    }

    cli_tools::print_success("Query executed successfully.");
    return 0;
}
} // namespace Database