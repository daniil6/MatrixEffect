#include <Windows.h>
#include <algorithm>
#include <conio.h>
#include <iostream>
#include <list>
#include <random>
#include <thread>

#define MAX_SIZE_TAIL 100

int main(int argc, char** argv)
{
    bool errcode = false;
    std::list<int> positionMatrix;
    std::list<std::string> stringOut;

    int m_widht = 0;
    int m_height = 0;
    int m_widht_prev = 0;
    int m_height_prev = 0;
    std::default_random_engine generator;
    const std::vector<std::string> stringChouse = { "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "A", "S", "D",
        "F", "G", "H", "J", "K", "L", "Z", "X", "C", "V", "B", "N", "M", "0", "1", "2", "3", "4", "5", "6", "7", "8",
        "9" };

    HANDLE hWndConsole = GetStdHandle(-12);
    SetConsoleTextAttribute(hWndConsole, FOREGROUND_GREEN);

    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;  // Width of each character in the font
    cfi.dwFontSize.Y = 24; // Height
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    std::wcscpy(cfi.FaceName, L"Consolas"); // Choose your font
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

    while(1) {
        // determine click key
        if(kbhit())
            if(getch() == 13) // code key "enter"
                return EXIT_SUCCESS;

        if(hWndConsole != 0) {
            CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
            if(GetConsoleScreenBufferInfo(hWndConsole, &consoleInfo)) {
                m_widht = consoleInfo.srWindow.Right - consoleInfo.srWindow.Left;
                m_height = consoleInfo.srWindow.Bottom - consoleInfo.srWindow.Top;
                // printf("Widht: %d  Height: %d\n", widht, height);
            } else
                errcode = true;
        } else
            errcode = true;

        // check error
        if(errcode == true) {
            std::cout << GetLastError() << std::endl;
            return EXIT_SUCCESS;
        }

        if(m_widht != m_widht_prev || m_height != m_height_prev) {
            positionMatrix.clear();
            stringOut.clear();

            // init positionMatrix zero value
            for(int i = 0; i < m_widht; i++)
                positionMatrix.push_back(0);

            // init stringOut empty space
            for(int i = 0; i < m_height; i++) {
                std::string stringTemp;
                for(int j = 0; j < m_widht; j++)
                    stringTemp.append(" ");
                stringOut.push_front(stringTemp);
            }

            m_widht_prev = m_widht;
            m_height_prev = m_height;
        }

        std::uniform_int_distribution<int> distributionNumberPositionMatrix(0, positionMatrix.size() - 1);
        std::uniform_int_distribution<int> distributionTailMatrix(0, MAX_SIZE_TAIL);
        std::uniform_int_distribution<int> distributionChoiseSimble(0, stringChouse.size() - 1);

        int numberPosition = distributionNumberPositionMatrix(generator);
        int tail = distributionTailMatrix(generator);

        int i = 0;
        for(auto& itr : positionMatrix) {
            if(i == numberPosition)
                itr = tail;
            i++;
        }

        /*if(positionMatrix.at(numberPosition) == 0)
            positionMatrix.at(numberPosition) = tail;*/

        std::string stringTemp;
        for(auto& itr : positionMatrix) {
            if(itr != 0) {
                stringTemp.append(stringChouse.at(distributionChoiseSimble(generator)));
                itr--;
            } else
                stringTemp.append(" ");
        }
        stringOut.push_front(stringTemp);
        stringOut.pop_back();

        COORD coordinates = { 0, 0 };
        SetConsoleCursorPosition(hWndConsole, coordinates);
        for(auto& itr : stringOut)
            std::cout << itr << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return EXIT_SUCCESS;
}