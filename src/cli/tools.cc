#include "tools.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>

namespace cli_tools
{
void clear_screen()
{
    // Code to clear the console screen
    std::system("clear"); // This command is specific to Unix/Linux
}

std::string bold(const std::string& message)
{
    // Code to make the text bold
    return "\033[1m" + message + "\033[0m";
}

void print_color(const std::string& message, color::Code col, std::ostream& output)
{
    // Code to print colored text
    output << color::Modifier(col) << message
           << color::Modifier(color::FG_DEFAULT) << std::endl;
}

void print_error(const std::string& message)
{
    // Code to print an error message in red
    print_color(message, color::FG_RED, std::cerr);
}

void print_success(const std::string& message)
{
    // Code to print a success message in green
    print_color(message, color::FG_GREEN);
}

void print_warning(const std::string& message)
{
    // Code to print a warning message in yellow
    print_color(message, color::FG_YELLOW);
}

void print_info(const std::string& message)
{
    // Code to print an info message in blue
    print_color(message, color::FG_BLUE);
}

bool confirm_action(const std::string& prompt)
{
    // Code to ask the user for confirmation
    std::string input;
    std::cout << prompt << bold(" (yes/no) ");
    std::cin >> input;

    return (input == "yes" || input == "y" || input == "Y");
}

std::string get_input(const std::string& prompt)
{
    // Code to get user input
    std::string input;
    std::cout << prompt;
    std::cin >> input;
    return input;
}

int create_file(const char* path, const char* mode)
{
    FILE* file = fopen(path, mode);
    if (file == NULL)
    {
        return CLI_EXIT_FAILURE;
    }
    fclose(file);
    return CLI_EXIT_SUCCESS;
}

int create_dir(const char* path)
{
#ifdef _WIN32
    // Windows specific code
    if (!CreateDirectory(path, NULL))
    {
        return CLI_EXIT_FAILURE;
    }
#else
    // Unix/Linux specific code
    if (mkdir(path, 0777) == -1)
    {
        return CLI_EXIT_FAILURE;
    }
#endif
    return CLI_EXIT_SUCCESS;
}


int remove_file(const char* path)
{
    if (remove(path) != 0)
    {
        return CLI_EXIT_FAILURE;
    }
    return CLI_EXIT_SUCCESS;
}

int remove_dir(const char* path)
{
#ifdef _WIN32
    // Windows specific code
    if (!RemoveDirectory(path))
    {
        return CLI_EXIT_FAILURE;
    }
#else
    // Unix/Linux specific code
    std::string command = "rm -rf " + std::string(path);
    int status = system(command.c_str());
    if (status != 0)
    {
        return CLI_EXIT_FAILURE;
    }
#endif
    return CLI_EXIT_SUCCESS;
}
} // namespace cli_tools

// Path: src/cli/tools.cpp