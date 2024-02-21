#include "config.h"

#include <algorithm>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "cli/tools.h"
#include "utils.h"

namespace config
{
Config::Config(const std::string& path)
{
    path_ = path;
    data_ = std::map<std::string,
        std::vector<std::pair<std::string, std::string>>>();
    owners_ = std::vector<std::string>();
    masteradmins_ = std::vector<std::string>();
    admins_ = std::vector<std::string>();
}

Config::~Config()
{
    if (!saved_)
    {
        cli_tools::print_warning("Configuration file not saved. Consider using "
                                 "config::Config::save() first.");
        cli_tools::print_info("-> This is a mistake from the developer of the "
                              "project, please report it.");
        save();
    }
}

void Config::load()
{
    std::ifstream fileStream(path_);

    if (!fileStream.is_open())
    {
        cli_tools::print_error(
            "Failed to open file: " + cli_tools::bold(path_));
        return;
    }

    std::string currentSection;
    std::string line;
    while (std::getline(fileStream, line))
    {
        utils::trim(line);
        if (line.empty() || line[0] == '#')
        {
            continue;
        }
        else if (line[0] == '[' && line[line.size() - 1] == ']')
        {
            currentSection = line.substr(1, line.size() - 2);
        }
        else
        {
            if (currentSection.empty())
            {
                cli_tools::print_warning(
                    "No section for key-value pair: " + cli_tools::bold(line));
                continue;
            }
            else
            {
                std::string key = line.substr(0, line.find('='));
                utils::trim(key);
                std::string value = line.substr(line.find('=') + 1);
                utils::trim(value);
                parse_value(value);

                if (currentSection == "planetplus")
                {
                    if (key == "owners")
                    {
                        std::vector<std::string> owners =
                            utils::split(value, ",");
                        for (std::string owner : owners)
                        {
                            owners_.push_back(owner);
                        }
                    }
                    else if (key == "masteradmins")
                    {
                        std::vector<std::string> masteradmins =
                            utils::split(value, ",");
                        for (std::string masteradmin : masteradmins)
                        {
                            masteradmins_.push_back(masteradmin);
                        }
                    }
                    else if (key == "admins")
                    {
                        std::vector<std::string> admins =
                            utils::split(value, ",");
                        for (std::string admin : admins)
                        {
                            admins_.push_back(admin);
                        }
                    }
                    else
                    {
                        data_[currentSection].push_back(
                            std::make_pair(key, value));
                    }
                }
                else
                {
                    data_[currentSection].push_back(std::make_pair(key, value));
                }
            }
        }
    }
    loaded_ = true;
}

void Config::save()
{
    std::ofstream fileStream(path_);

    if (!fileStream.is_open())
    {
        cli_tools::print_error(
            "Failed to open file: " + cli_tools::bold(path_));
        return;
    }

    fileStream << "# Configuration file for PlanetPlus\n";
    fileStream << "# This file is automatically generated. Do not modify it "
                  "unless you know what you are doing.\n";
    fileStream << "\n";

    fileStream << "[planetplus]\n";
    for (auto& pairs : data_["planetplus"])
    {
        fileStream << pairs.first << " = \"" << pairs.second << "\"\n";
    }
    fileStream << "owners = \"" << utils::join(owners_, ", ") << "\"\n";
    fileStream << "masteradmins = \"" << utils::join(masteradmins_, ", ")
               << "\"\n";
    fileStream << "admins = \"" << utils::join(admins_, ", ") << "\"\n";
    fileStream << "\n";

    for (auto& section : data_)
    {
        if (section.first == "planetplus")
        {
            continue;
        }

        fileStream << "[" << section.first << "]\n";
        for (auto& pairs : section.second)
        {
            fileStream << pairs.first << " = \"" << pairs.second << "\"\n";
        }
        fileStream << "\n";
    }

    saved_ = true;
    fileStream.close();
}

std::string Config::get(const std::string& section, const std::string& key)
{
    check_if_loaded();
    if (data_.find(section) == data_.end())
    {
        cli_tools::print_warning(
            "Section not found: " + cli_tools::bold(section) + "\n");
        return "";
    }

    if (key == "owners")
    {
        return utils::join(owners_, ", ");
    }
    else if (key == "masteradmins")
    {
        return utils::join(masteradmins_, ", ");
    }
    else if (key == "admins")
    {
        return utils::join(admins_, ", ");
    }

    for (std::pair<std::string, std::string>& pair : data_[section])
    {
        if (pair.first == key)
        {
            return pair.second;
        }
    }

    cli_tools::print_warning("Key not found: " + cli_tools::bold(key) + "\n");
    return "";
}

std::vector<std::string> Config::get(ConfigType type)
{
    check_if_loaded();
    switch (type)
    {
        case ConfigType::OWNER:
            return owners_;
        case ConfigType::MASTERADMIN:
            return masteradmins_;
        case ConfigType::ADMIN:
            return admins_;
    }
    return std::vector<std::string>();
}

void Config::set(const std::string& section, const std::string& key,
    const std::string& value)
{
    check_if_loaded();
    if (data_.find(section) == data_.end())
    {
        cli_tools::print_warning(
            "Section not found: " + cli_tools::bold(section) + "\n");
    }
    else
    {
        for (std::pair<std::string, std::string>& pair : data_[section])
        {
            if (pair.first == key)
            {
                pair.second = value;
            }
        }
    }
}

void Config::set(const std::string& value, ConfigType type)
{
    check_if_loaded();
    switch (type)
    {
        case ConfigType::OWNER:
            owners_.push_back(value);
            break;
        case ConfigType::MASTERADMIN:
            masteradmins_.push_back(value);
            break;
        case ConfigType::ADMIN:
            admins_.push_back(value);
            break;
    }
}

void Config::parse_value(std::string& value)
{
    // Remove " and ' from the beginning and end of the string
    while (value[0] == '"' || value[0] == '\'')
    {
        value.erase(0, 1);
    }

    while (value[value.size() - 1] == '"' || value[value.size() - 1] == '\'')
    {
        value.erase(value.size() - 1, 1);
    }
}

void Config::check_if_loaded()
{
    if (!loaded_)
    {
        cli_tools::print_warning("Configuration file not loaded. Consider "
                                 "using config::Config::load() first.");
        cli_tools::print_info("-> This is a mistake from the developer of the "
                              "project, please report it.\n");
    }
}
} // namespace config
