#pragma once
/** console_helper.h
 * Version: 1.0.1
 * Date: 10/6/2021
 * Written by Jerry Rivas
 **/

#include <string>
#include <iostream>
#include <Windows.h>

using namespace std;

namespace KCon
{
    enum class Color {
        BLACK,
        DARKBLUE,
        DARKGREEN,
        DARKCYAN,
        DARKRED,
        DARKMAGENTA,
        DARKYELLOW,
        WHITE,
        GRAY,
        BLUE,
        GREEN,
        CYAN,
        RED,
        MAGENTA,
        YELLOW
    };

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Default colors for Output
    Color defaultFG = Color::GREEN;
    Color defaultBG = Color::BLACK;

    // Default colors for Input
    Color defaultInputFG = Color::DARKYELLOW;
    Color defaultInputBG = Color::BLACK;

    void SetColor(Color foreground = Color::WHITE, Color background = Color::BLACK) {
        int fg = (int)foreground;
        int bg = (int)background;
        SetConsoleTextAttribute(hConsole, fg + bg * 16);
    }

    void CursorPosition(short x, short y) {
        COORD position = { x, y };
        SetConsoleCursorPosition(hConsole, position);
    }

    void CursorVisibility(bool show) {
        CONSOLE_CURSOR_INFO info;
        info.dwSize = show ? 100 : 0;
        info.bVisible = show;
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    }

    void write(char endch, string text, Color fg = defaultFG, Color bg = defaultBG) {
        SetColor(fg, bg);
        cout << text << endch;
        SetColor(defaultFG, defaultBG);
    }

    void write(string text, Color fg = defaultFG, Color bg = defaultBG, char endch = '\n') {
        SetColor(fg, bg);
        cout << text << endch;
        SetColor(defaultFG, defaultBG);
    }

    string readText() {
        string in;
        SetColor(defaultInputFG, defaultInputBG);
        cin >> in;
        cin.clear();
        SetColor(defaultFG, defaultBG);
        return in;
    }

    int readInt() {
        string in = readText();
        return stoi(in);
    }

    float readFloat() {
        string in = readText();
        return stof(in);
    }

    char asciitolower(char in) {
        if (in <= 'Z' && in >= 'A')
            return in - ('Z' - 'z');
        return in;
    }

    string asciitolower(string in) {
        int size = in.size();
        string out;
        for (int i = 0; i < size; i++)
        {
            out.push_back(in[i]);
        }
        return out;
    }

    bool readYN() {
        SetColor(defaultInputFG, defaultInputBG);
        char in = asciitolower(readText()[0]);
        SetColor(defaultFG, defaultBG);
        if (in == 'y') {
            return true;
        }
        if (in == 'n') {
	        return false;
        }
        throw invalid_argument("User didn't write y or n");
    }

    void clear() {
        system("cls");
    }

    void waitToContinue() {
        system("pause");
    }
} // namespace KCon