#ifndef DATABASE_H
#define DATABASE_H

#include <ini_parser.h>

namespace database
{
class Handler
{
  public:
    const std::string kHost;
    const std::string kPort;
    const std::string kUser;
    const std::string kPassword;
    const std::string kName;

    // Connect to the database
    bool connect();

    // Disconnect from the database
    bool disconnect();

    explicit Handler(config::IniParser& ini);
    ~Handler();
};
} // namespace Database

#endif