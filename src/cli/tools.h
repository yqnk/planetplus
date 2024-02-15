#ifndef CLI_TOOLS_H
#define CLI_TOOLS_H

#include <iostream>
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
void clear_screen();

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
 */
void print_color(const std::string& message, color::Code color);

/**
 * @brief Prints an error message in red.
 *
 * @param message The error message to be printed.
 */
void print_error(const std::string& message);

/**
 * @brief Prints a success message in green.
 *
 * @param message The success message to be printed.
 */
void print_success(const std::string& message);

/**
 * @brief Prints a warning message in yellow.
 *
 * @param message The warning message to be printed.
 */
void print_warning(const std::string& message);

/**
 * @brief Asks the user to confirm an action with "yes" or "no".
 *
 * @param prompt The confirmation message to be displayed.
 * @return true if the user confirms with "yes", otherwise false.
 */
bool confirm_action(const std::string& prompt);

int create_file(const char* path, const char* mode);
int create_dir(const char* path);
int remove_file(const char* path);
int remove_dir(const char* path);
} // namespace cli

#endif // CLI_TOOLS_H