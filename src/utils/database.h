#ifndef DATABASE_H
#define DATABASE_H

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

    /**
     * @brief disconnect from the database
     * 
     * @return true 
     * @return false 
     */
    bool connect();

    /**
     * @brief disconnect from the database
     * 
     * @return true 
     * @return false 
     */
    bool disconnect();

    /**
     * @brief Execute a sql query
     * 
     * @param query 
     * @return int 
     */
    int execute_query(const std::string& query);

    explicit Manager(std::string config_path);
    ~Manager();
};
} // namespace Database

#endif