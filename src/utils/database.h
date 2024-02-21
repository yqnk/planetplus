#ifndef DATABASE_H
#define DATABASE_H

#include <mysql/mysql.h>

#include <string>

#include "utils/config.h"

namespace database
{
class Manager
{
  public:
    std::string host;
    std::string user;
    std::string port;
    std::string password;
    std::string name;

    MYSQL* conn;

    bool connect();
    void disconnect();

    /**
     * @brief Execute a sql query
     *
     * @param query
     * @return int
     */
    int executeQuery(const std::string& query);

    /**
     * @brief Execute a sql query from a file
     *
     * @param file_path
     * @return int
     */
    int executeFromFile(const std::string& file_path);

    Manager::Manager(std::string config_path, config::Config* config);
    ~Manager();

  private:
    bool disconnected_;
};
} // namespace Database

#endif