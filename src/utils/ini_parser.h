#ifndef INI_PARSER_H
#define INI_PARSER_H

#include <fstream>
#include <iostream>
#include <map>

namespace config
{
class IniParser
{
  public:
    std::map<std::string, std::map<std::string, std::string>> data;
    const std::string path;
    /**
     * @brief Construct a new Ini Parser object.
     *
     * @param filePath Path to the INI file.
     */
    IniParser(const std::string& filePath);

    /**
     * @brief Destroy the Ini Parser object and save the changes to the INI
     * file.
     */
    ~IniParser();

    /**
     * @brief Load the contents of the INI file into memory.
     */
    void load();

    /**
     * @brief Trim leading and trailing whitespace from a string.
     *
     * @param str The string to trim.
     */
    void trim(std::string& str);

    /**
     * @brief Save the contents of the INI file to disk.
     */
    void save();

    /**
     * @brief Get the value of a key within a specific section.
     *
     * @param section The section name.
     * @param key The key name.
     * @return The value associated with the key within the section.
     */
    std::string get(const std::string& section, const std::string& key);

    /**
     * @brief Set the value of a key within a specific section.
     *
     * @param section The section name.
     * @param key The key name.
     * @param value The value to set.
     */
    void set(const std::string& section, const std::string& key,
        const std::string& value);

    /**
     * @brief Add a new key-value pair to a specific section.
     *
     * @param section The section name.
     * @param key The key name.
     * @param value The value to add.
     */
    void add_key(const std::string& section, const std::string& key,
        const std::string& value);

    /**
     * @brief Remove a key from a specific section.
     *
     * @param section The section name.
     * @param key The key name to remove.
     */
    void remove_key(const std::string& section, const std::string& key);

    /**
     * @brief Remove all keys with a specific value from a section.
     *
     * @param section The section name.
     * @param value The value to remove.
     */
    void remove_keys_with_value(
        const std::string& section, const std::string& value);
};
} // namespace config

#endif // INI_PARSER_H