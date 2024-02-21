#include "tools.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <termios.h>
#endif

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>

namespace cli_tools
{
void clearScreen()
{
    // Code to clear the console screen
    std::system("clear"); // This command is specific to Unix/Linux
}

std::string bold(const std::string& message)
{
    // Code to make the text bold
    return "\033[1m" + message + "\033[0m";
}

void printColor(const std::string& message, color::Code col, std::ostream& output, char end)
{
    // Code to print colored text
    output << color::Modifier(col) << message
           << color::Modifier(color::FG_DEFAULT) << end;
}

void printError(const std::string& message)
{
    // Code to print an error message in red
    printColor(message, color::FG_RED, std::cerr);
}

void printSuccess(const std::string& message)
{
    // Code to print a success message in green
    printColor(message, color::FG_GREEN);
}

void printWarning(const std::string& message)
{
    // Code to print a warning message in yellow
    printColor(message, color::FG_YELLOW);
}

void printInfo(const std::string& message, char end)
{
    // Code to print an info message in blue
    printColor(message, color::FG_BLUE, std::cout, end);
}

bool confirmAction(const std::string& prompt)
{
    // Code to ask the user for confirmation
    std::string input;
    std::cout << prompt << bold(" (yes/no) ");
    std::cin >> input;

    return (input == "yes" || input == "y" || input == "Y");
}

std::string getInput(const std::string& prompt)
{
    // Code to get user input.
    // If input = '\n' then send empty string
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

std::string getHiddenInput(const std::string& prompt)
{
    // Replace every character written by the user with '*'
    std::string input;
    std::cout << prompt;
    
}

int createFile(const char* path, const char* mode)
{
    FILE* file = fopen(path, mode);
    if (file == NULL)
    {
        return CLI_EXIT_FAILURE;
    }
    fclose(file);
    return CLI_EXIT_SUCCESS;
}

int createDir(const char* path)
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


int removeFile(const char* path)
{
    if (remove(path) != 0)
    {
        return CLI_EXIT_FAILURE;
    }
    return CLI_EXIT_SUCCESS;
}

int removeDir(const char* path)
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