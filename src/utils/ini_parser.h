#ifndef INI_PARSER_H
#define INI_PARSER_H

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>


namespace config
{
struct Line
{
    std::string key;
    std::string value;
    std::string comment;
};

void print_sections(const std::map<std::string, std::vector<config::Line>>& sections);

class IniParser
{
  public:
    const std::string path;
    std::map<std::string, std::vector<Line>> sections;
    bool saved = false;

    IniParser(const std::string& filePath);
    ~IniParser();

    /**
     * @brief Save the configuration to a file.
     * 
     * @param filePath 
     */
    void save(const std::string& filePath);

    /**
     * @brief Get the value of a key in a section.
     *
     * @param sectionName The name of the section.
     * @param key The name of the key.
     * @return std::string The value of the key.
     */
    std::string get_value(
        const std::string& sectionName, const std::string& key) const;
    
    std::vector<std::string> get_master_admins() const;
    std::vector<std::string> get_admins() const;

    /**
     * @brief Set the value object
     * 
     * @param sectionName 
     * @param key 
     * @param value 
     */
    void set_value(const std::string& sectionName, const std::string& key,
        const std::string& value);

    /**
     * @brief Remove a key from a section.
     * 
     * @param sectionName 
     * @param key 
     */
    void remove_key(const std::string& sectionName, const std::string& key);

    /**
     * @brief Remove a section.
     * 
     * @param sectionName 
     */
    void remove_section(const std::string& sectionName);

  private:
    /**
     * @brief Parse the configuration file.
     *
     */
    void parse();

    /**
     * @brief Trim leading and trailing whitespace from a string.
     *
     * @param str The string to trim.
     */
    void trim(std::string& str);
};
} // namespace config

#endif // INI_PARSER_H