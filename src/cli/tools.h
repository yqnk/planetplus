#ifndef CLI_TOOLS_H
#define CLI_TOOLS_H

#include <iostream>
#include <string>
#include <map>

// Output codes
#define CLI_EXIT_SUCCESS 0
#define CLI_EXIT_FAILURE 1

// colors because why not
namespace color
{
enum Code
{
    FG_RED = 31,
    FG_GREEN = 32,
    FG_YELLOW = 33,
    FG_BLUE = 34,
    FG_DEFAULT = 39,
    BG_RED = 41,
    BG_GREEN = 42,
    BG_YELLOW = 43,
    BG_BLUE = 44,
    BG_DEFAULT = 49
};

class Modifier
{
    Code code;

  public:
    Modifier(Code pCode)
        : code(pCode)
    {
    }
    friend std::ostream& operator<<(std::ostream& os, const Modifier& mod)
    {
        return os << "\033[" << mod.code << "m";
    }
};
} // namespace color

namespace cli_tools
{
/**
 * @brief Clears the console screen.
 */
void clearScreen();

/**
 * @brief Makes the text bold.
 *
 * @param message The message to be made bold.
 * @return The bold message.
 */
std::string bold(const std::string& message);

/**
 * @brief Prints colored text.
 *
 * @param message The message to be printed.
 * @param color   The color of the text.
 * @param output  The output stream to be used.
 */
void printColor(const std::string& message, color::Code color, std::ostream& output = std::cout, char end = '\n');

/**
 * @brief Prints an error message in red.
 *
 * @param message The error message to be printed.
 */
void printError(const std::string& message);

/**
 * @brief Prints a success message in green.
 *
 * @param message The success message to be printed.
 */
void printSuccess(const std::string& message);

/**
 * @brief Prints a warning message in yellow.
 *
 * @param message The warning message to be printed.
 */
void printWarning(const std::string& message);

/**
 * @brief Prints an info message in blue.
 *
 * @param message The info message to be printed.
 * @param end     The character to be printed at the end of the message.
 */
void printInfo(const std::string& message, char end = '\n');

/**
 * @brief Asks the user to confirm an action with "yes" or "no".
 *
 * @param prompt The confirmation message to be displayed.
 * @return true if the user confirms with "yes", otherwise false.
 */
bool confirmAction(const std::string& prompt);

/**
 * @brief Gets user input.
 *
 * @param prompt The message to be displayed to the user.
 * @return The user's input.
 */
std::string getInput(const std::string& prompt);

/**
 * @brief Creates a file.
 *
 * @param path The path to the file to be created.
 * @param mode The mode to be used when creating the file.
 * @return 0 if the file was created successfully, otherwise -1.
 */
int createFile(const char* path, const char* mode);

/**
 * @brief Creates a directory.
 *
 * @param path The path to the directory to be created.
 * @return 0 if the directory was created successfully, otherwise -1.
 */
int createDir(const char* path);

/**
 * @brief Removes a file.
 *
 * @param path The path to the file to be removed.
 * @return 0 if the file was removed successfully, otherwise -1.
 */
int removeFile(const char* path);

/**
 * @brief Removes a directory.
 *
 * @param path The path to the directory to be removed.
 * @return 0 if the directory was removed successfully, otherwise -1.
 */
int removeDir(const char* path);
} // namespace cli

#endif // CLI_TOOLS_H