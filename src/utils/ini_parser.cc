#include "ini_parser.h"

#include <fstream>
#include <map>
#include <string>

#include "cli/tools.h"

namespace config
{
IniParser::IniParser(const std::string& filePath)
    : path(filePath)
{
    // Load the INI file when creating the parser
    load();
}

IniParser::~IniParser()
{
    // Save the INI file when destroying the parser
    save();
}

void IniParser::load()
{
    std::ifstream file(path);
    std::string line;
    std::string currentSection;

    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            if (line.empty()) // Ignore empty lines
                continue;

            // Check if the line is a comment
            if (line[0] == ';')
            {
                // Add the comment to the current section
                data[currentSection][";comment"] += line + "\n";
            }
            else if (line[0] == '[' && line[line.size() - 1] == ']')
            { // New section
                currentSection = line.substr(1, line.size() - 2);
            }
            else
            {
                size_t pos = line.find('=');
                if (pos != std::string::npos)
                {
                    std::string key = line.substr(0, pos);
                    std::string value = line.substr(pos + 1);

                    // trim leading and trailing whitespace
                    trim(key);
                    trim(value);

                    data[currentSection][key] = value;
                }
            }
        }
        file.close();
    }
    else
    {
        cli_tools::print_error(
            "!! Unable to open file: " + cli_tools::bold(path));
    }
}

void IniParser::save()
{
    std::ofstream file(path);

    if (file.is_open())
    {
        for (const auto& section : data)
        {
            file << "[" << section.first << "]\n";
            // Write comments first if they exist
            auto commentIt = section.second.find(";comment");
            if (commentIt != section.second.end() && !commentIt->second.empty())
            {
                file << commentIt->second;
            }
            for (const auto& pair : section.second)
            {
                if (pair.first != ";comment")
                {
                    file << pair.first << "=" << pair.second << "\n";
                }
            }
            file << "\n";
        }
        file.close();
    }
    else
    {
        cli_tools::print_error(
            "!! Unable to open file: " + cli_tools::bold(path));
    }
}

void IniParser::trim(std::string& str)
{
    // TODO : trim
}

std::string IniParser::get(const std::string& section, const std::string& key)
{
    if (data.find(section) != data.end() &&
        data[section].find(key) != data[section].end())
    {
        return data[section][key];
    }
    return "";
}

void IniParser::set(const std::string& section, const std::string& key,
    const std::string& value)
{
    data[section][key] = value;
}

void IniParser::add_key(const std::string& section, const std::string& key,
    const std::string& value)
{
    data[section][key] = value;
}

void IniParser::remove_key(const std::string& section, const std::string& key)
{
    if (data.find(section) != data.end())
    {
        data[section].erase(key);
    }
}

void IniParser::remove_keys_with_value(
    const std::string& section, const std::string& value)
{
    if (data.find(section) != data.end())
    {
        for (auto it = data[section].begin(); it != data[section].end();)
        {
            if (it->second == value)
            {
                it = data[section].erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}
} // namespace config