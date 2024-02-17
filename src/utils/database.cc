#include "database.h"

#include <string>

#include "ini_parser.h"

namespace database
{
Manager::Manager(std::string config_path)
{
    config::IniParser ini(config_path);
    host = ini.get("database", "host");
    port = ini.get("database", "port");
    user = ini.get("database", "user");
    password = ini.get("database", "password");
    name = ini.get("database", "name");

    connect();
}

Manager::~Manager()
{
    disconnect();
}
bool Manager::connect()
{
    // TODO: Implement this function
}

bool Manager::disconnect()
{
    // TODO: Implement this function
}

int Manager::execute_query(const std::string& query)
{
    // TODO : Implement this function
}
} // namespace Database