#include "ini_parser.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "cli/tools.h"

namespace config
{
IniParser::IniParser(const std::string& filePath)
    : path(filePath)
{
    parse();
}

IniParser::~IniParser()
{
    if (!saved)
    {
        cli_tools::print_warning(
            "Configuration file not saved. Some changes may be lost.");
    }
    save(path);
}

void IniParser::trim(std::string& str)
{
    str.erase(0, str.find_first_not_of(" \t"));
    str.erase(str.find_last_not_of(" \t") + 1);
}

std::string IniParser::get_value(
    const std::string& sectionName, const std::string& key) const
{
    auto section = sections.find(sectionName);
    if (section != sections.end())
    {
        for (const auto& line : section->second)
        {
            if (line.key == key)
            {
                return line.value;
            }
        }
    }
    return ""; // Return empty string if key not found
}

std::vector<std::string> IniParser::get_master_admins() const
{
    std::vector<std::string> masterAdmins;
    auto section = sections.find("master_admins");
    if (section != sections.end())
    {
        for (const auto& line : section->second)
        {
            masterAdmins.push_back(line.value);
        }
    }
    return masterAdmins;
}

void print_sections(const std::map<std::string, std::vector<Line>>& sections)
{
    for (const auto& section : sections)
    {
        std::cout << "[" << section.first << "]" << std::endl;
        for (const auto& line : section.second)
        {
            std::cout << line.key << " = " << line.value << std::endl;
        }
        std::cout << std::endl;
    }
}

std::vector<std::string> IniParser::get_admins() const
{
    std::vector<std::string> admins;
    auto section = sections.find("admins");
    if (section != sections.end())
    {
        for (const auto& line : section->second)
        {
            admins.push_back(line.value);
        }
    }
    return admins;
}

void IniParser::set_value(const std::string& sectionName,
    const std::string& key, const std::string& value)
{
    for (auto& line : sections[sectionName])
    {
        if (line.key == key)
        {
            line.value = value;
            return;
        }
    }
    // If key not found, add a new line
    Line newLine;
    newLine.key = key;
    newLine.value = value;
    sections[sectionName].push_back(newLine);
}

void IniParser::remove_key(
    const std::string& sectionName, const std::string& key)
{
    auto& section = sections[sectionName];
    section.erase(std::remove_if(section.begin(), section.end(),
                      [&](const Line& line) { return line.key == key; }),
        section.end());
}

void IniParser::remove_section(const std::string& sectionName)
{
    sections.erase(sectionName);
}

void IniParser::save(const std::string& filePath)
{
    std::ofstream outFile(filePath);
    if (!outFile.is_open())
    {
        cli_tools::print_error(
            "Failed to open file: " + cli_tools::bold(filePath));
        return;
    }

    for (const auto& section : sections)
    {
        outFile << "[" << section.first << "]" << std::endl;
        for (const auto& line : section.second)
        {
            if (!line.comment.empty())
            {
                outFile << "; " << line.comment << std::endl;
            }
            outFile << line.key << " = " << line.value << std::endl;
        }
        outFile << std::endl; // Ajoute une ligne vide après chaque section
    }

    saved = true;
    outFile.close();
}

void IniParser::parse()
{
    std::ifstream fileStream(path);
    if (!fileStream.is_open())
    {
        cli_tools::print_error("Failed to open file: " + cli_tools::bold(path));
        return;
    }

    std::string currentSection;
    std::string line;
    while (std::getline(fileStream, line))
    {
        trim(line);
        if (line.empty() || line[0] == ';' || line[0] == '#')
        {
            // Ignore empty lines and comments
            continue;
        }
        else if (line[0] == '[' && line[line.length() - 1] == ']')
        {
            // Found a section
            currentSection = line.substr(1, line.length() - 2);
        }
        else
        {
            // Found a key-value pair
            std::stringstream lineStream(line);
            std::string key, value;
            std::getline(lineStream, key, '=');
            trim(key);
            std::getline(lineStream, value);
            trim(value);

            Line configLine;
            configLine.key = key;
            configLine.value = value;
            sections[currentSection].push_back(configLine);
        }
    }
    fileStream.close();
};
} // namespace config