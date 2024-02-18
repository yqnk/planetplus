#ifndef CONFIG_H
#define CONFIG_H

#include <map>
#include <string>
#include <vector>

namespace config
{
enum class ConfigType
{
    OWNER,
    MASTERADMIN,
    ADMIN
};

class Config
{
  public:
    Config(const std::string& path);
    ~Config();

    void load();
    void save();

    std::string get(const std::string& section, const std::string& key);
    std::vector<std::string> get(ConfigType type);

    void set(const std::string& section, const std::string& key,
        const std::string& value);
    void set(const std::string& value, ConfigType type);

  private:
    std::string path_;
    std::map<std::string, std::vector<std::pair<std::string, std::string>>>
        data_;
    std::vector<std::string> owners_;
    std::vector<std::string> masteradmins_;
    std::vector<std::string> admins_;
    bool saved_;
    bool loaded_;

    void parse_value(std::string& value);
    void check_if_loaded();
};
} // namespace utils

#endif