#include <windows.h>
#include <iostream>
#include <chrono>
#include <conio.h>
#include <cstdlib>
#include <thread>
#include <set>
#include <fstream>
#include <filesystem>
#include <vector>

// GLOBAL VARIABLES
bool resetTimer = true;
bool loop = true;
int pressCount = 0;  //Counts keypresses for exit
int requiredPresses; //Loaded from INI: `exitKeypresses`
char exitKey;        //Loaded from INI: `exitKey`
int cooldownTime;  //Loaded from INI: `cooldown`

// INI VARIABLE IMPORT
std::vector<std::string> LoadSequentialCommands(const std::string &section, const std::string &filePath)
{
    std::vector<std::string> commands;
    int index = 1;

    while (true)
    {
        char buffer[256];
        std::string key = "cmd" + std::to_string(index);

        DWORD charsRead = GetPrivateProfileStringA(section.c_str(), key.c_str(), "", buffer, sizeof(buffer), filePath.c_str());

        if (charsRead == 0) //Stopping in case of no results found
        { 
            break;
        }

        commands.push_back(std::string(buffer));
        index++;
    }

    return commands;
}

void LoadSettings(const std::string &filePath)
{
    char buffer[256];

    GetPrivateProfileStringA("Program settings", "cooldown", "180", buffer, sizeof(buffer), filePath.c_str());
    cooldownTime = std::stoi(buffer);

    GetPrivateProfileStringA("Program settings", "exitKeypresses", "3", buffer, sizeof(buffer), filePath.c_str());
    requiredPresses = std::stoi(buffer);

    GetPrivateProfileStringA("Program settings", "exitKey", "x", buffer, sizeof(buffer), filePath.c_str());
    exitKey = buffer[0];
}

//  DELAY FUNCTION
void millisecsDelay(int millisecs)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(millisecs));
}

//  COOLDOWN TIMER (Separate Thread)
void cooldown()
{
    while (loop)
    {
        if (!resetTimer)
        {
            for (int i = cooldownTime; i > 0; i--)
            {
                millisecsDelay(950);
                system("cls");
                std::cout << "Next cycle in " << i << " seconds.\n";
                unsigned int remainingExitPresses = requiredPresses - pressCount;
                if (remainingExitPresses == 0)
                {
                    std::cout << "The program will be stopped in the next cycle.\n";
                }
                else
                {
                    std::cout << "Press '" << exitKey << "' " << remainingExitPresses << " more times to stop the program.\n";
                }
                if (resetTimer)
                    break;
            }
            resetTimer = true; //Set flag to restart every cycle
        }
        millisecsDelay(100); //Prevent CPU overuse
    }
}

//  KEYBOARD LISTENER (Separate Thread)
void keyListener()
{
    const int keyCooldown = 250;

    while (loop)
    {
        if (_kbhit())
        {
            char key = _getch();

            if (key == exitKey || key == toupper(exitKey))
            {
                pressCount++;
                if (pressCount >= requiredPresses)
                {
                    loop = false;
                    break;
                }
                millisecsDelay(keyCooldown);
            }
        }
        millisecsDelay(750); //Avoid CPU overuse
    }
}

// MAIN FUNCTION

void submain(const std::string &iniPath)
{
    std::vector<std::string> commands = LoadSequentialCommands("Cycled commands", iniPath);

    // Loading all imported commands sequentially
    for (size_t i = 0; i < commands.size(); i++)
    {
        std::cout << "cmd" << (i + 1) << ": " << commands[i] << std::endl;
        system(commands[i].c_str());
    }

    millisecsDelay(5000);

    system("cls");
    resetTimer = false; //(Re)start the timer for a cooldown till next command cycle.
}

int main()
{
    // Obtaining the current program's directory. Expected to have .ini in the same directory
    std::string currentPath = std::filesystem::current_path().string();
    std::string iniPath = currentPath + "\\variables.ini"; //Building the .ini file directory.

    LoadSettings(iniPath); // .ini needs to match

    std::cout << "Program started." << "\n";
    millisecsDelay(3000);

    // Start background threads for a cooldown timer and a key listener
    std::thread cooldownThread(cooldown);
    std::thread keyThread(keyListener);

    //Continuous cycles performed
    while (loop)
    {
        if (resetTimer)
        {
            submain(iniPath); // Perform the commands every cycle of the cooldown timer
        }
    }

    // Wait for threads to finish
    cooldownThread.join();
    keyThread.join();

    //Final message before exitting
    system("cls");
    std::cout << "Program stopped.\n";
    millisecsDelay(3000);

    return 0;
}
