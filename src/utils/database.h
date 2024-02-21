#ifndef DATABASE_H
#define DATABASE_H

#include <mysql/mysql.h>

#include <string>

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
    int execute_query(const std::string& query);

    Manager::Manager(std::string config_path, config::Config* config);
    ~Manager();

  private:
    bool disconnected_;
};
} // namespace Database

#endif