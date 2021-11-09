// MasterMind.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// Comentario Kawaii
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <conio.h>
#include "ConsoleHelper.h"

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ENTER '\n'

using std::vector;
using std::array;

struct Game
{
    int maxNumber;
    vector<int> requiredCombination;
    int tries = 0;
};

enum class Answer
{
    C,
    F,
    X
};

char ansToChar(Answer ans) {
    switch (ans)
    {
    case Answer::C:
        return 'C';
    case Answer::F:
        return 'F';
    case Answer::X:
        return 'X';
    default:
        return '\0';
    }
}

bool isWin(vector<Answer> ansComb) {
    int vectorSize = ansComb.size();
    for (int i = 0; i < vectorSize; i++)
    {
        if (Answer::C != ansComb[i]) {
            return false;
        }
    }
    return true;
}

bool isIn(int number, vector<int> haystack) {
    int vectorSize = haystack.size();
    for (int i = 0; i < vectorSize; i++)
    {
        if (haystack[i] == number) {
            return true;
        }
    }
    return false;
}

vector<Answer> attemptAtGame(Game* game, vector<int> combination) {
    int vectorSize = game->requiredCombination.size();
    int attemptSize = combination.size();

    vector<Answer> answers;
    int correct = 0;

    if (vectorSize != attemptSize) {
        return answers;
    }

    for (int i = 0; i < vectorSize; i++)
    {
        int columnAttempt = combination[i];
        if (game->requiredCombination[i] == columnAttempt) {
            answers.push_back(Answer::C);
        }
        else if (isIn(columnAttempt, game->requiredCombination)) {
            answers.push_back(Answer::F);
        }
        else {
            answers.push_back(Answer::X);
        }
    }
    game->tries += 1;
    return answers;
}

int rand(int min, int max) {
    return min + rand() % max;
}

int rand(int max) {
    return max == 0 ? 0 : rand() % max;
}

vector<int> generateNumArray(int combinationSize, int maxNumber, bool repeat = false) {
    if (!repeat && (combinationSize > maxNumber)) {
        throw  std::invalid_argument("No puedes tener mas combinaciones que numeros");
    }
    vector<int> baseList;
    if (repeat) {
        for (int i = 0; i < combinationSize; i++)
        {
            baseList.push_back(rand(maxNumber));
        }
    }
    else {
        for (int i = 0; i < maxNumber; i++)
        {
            baseList.push_back(i);
        }
    }
    vector<int> combinations;
    while (combinations.size() < combinationSize) {
        int removedIndex = rand(baseList.size());
        int removedNo = baseList[removedIndex];
        combinations.push_back(removedNo);
        baseList.erase(baseList.begin() + removedIndex);
    }
    return combinations;
}

Game newGame(int combinations, int newGameMaxNumber, bool usrRepeatNumber) {
    Game newGame = Game();
    newGame.maxNumber = newGameMaxNumber;
    newGame.requiredCombination = generateNumArray(combinations, newGameMaxNumber, usrRepeatNumber);
    return newGame;
}

string padded(string str, int width) {
    int pad = width - str.length();
    int right = pad / 2;
    return string(pad - right, ' ') + str + string(right, ' ');
}

string padded(char str, int width) {
    int pad = width - 1;
    int right = pad / 2;
    return string(pad - right, ' ') + string(1, str) + string(right, ' ');
}

string paddedNumber(int number, int width) {
    string str = std::to_string(number);
    return padded(str, width);
}

void renderCombGrid(vector<int> combs, int selected, int tryno) {
    int combsize = combs.size();
    for (int i = 0; i < combsize; i++)
    {
        for (int k = 0; k < 3; k++)
        {
            KCon::CursorPosition(i * 7, tryno * 3 + 1 + k);
            switch (k)
            {
            case 0:
                if (i == selected) {
                    KCon::write(' ', " " + padded("^", 5) + " ");
                }
                else {
                    KCon::write(' ', " " + padded(" ", 5) + " ");
                }
                break;
            case 1:
                if (i == selected) {
                    KCon::write(' ', "<" + paddedNumber(combs[i] + 1, 5) + ">", KCon::Color::BLACK, KCon::Color::GREEN);
                }
                else {
                    KCon::write(' ', " " + paddedNumber(combs[i]+1, 5) + " ");
                }
                break;
            case 2:
                if (i == selected) {
                    KCon::write(' ', " " + padded("v", 5) + " ");
                }
                else {
                    KCon::write(' ', " " + padded(" ", 5) + " ");
                }
                break;
            }
        }
    }
}

void renderAnsGrid(vector<Answer> hints, vector<int> prev, int tryno) {
    int hintSize = hints.size();
    for (int i = 0; i < hintSize; i++)
    {
        KCon::Color color;
        switch (hints[i])
        {
        case Answer::C:
            color = KCon::Color::BLUE;
            break;
        case Answer::F:
            color = KCon::Color::CYAN;
            break;
        case Answer::X:
            color = KCon::Color::RED;
            break;
        default:
            color = KCon::Color::WHITE;
            break;
        }
        char hint = ansToChar(hints[i]);
        for (int k = 0; k < 3; k++)
        {
            KCon::CursorPosition(i * 7, tryno * 3 + 1 + k);
            switch (k)
            {
            case 0:
                KCon::write(' ', " " + padded(hint, 5) + " ", color);
                break;
            case 1:
                KCon::write(' ', " " + paddedNumber(prev[i]+1, 5) + " ", color);
                break;
            case 2:
                KCon::write(' ', " " + padded(" ", 5) + " ");
                break;
            }
        }
    }
}

void beepTone(int tone, int duration=100, bool th = true) {
    if (th) {
        std::thread t1(beepTone, tone, duration, false);
        t1.detach();
    } else {
        float freq;
        switch (tone % 12) // First Octave frequencies
        {
        case 0:
            freq = 32.70; // C
            break;
        case 1:
            freq = 34.65; // C#
            break;
        case 2:
            freq = 36.71; // D
            break;
        case 3:
            freq = 38.89; // D#
            break;
        case 4:
            freq = 41.20; // E
            break;
        case 5:
            freq = 43.65; // F
            break;
        case 6:
            freq = 46.25; // F#
            break;
        case 7:
            freq = 49.00; // G
            break;
        case 8:
            freq = 51.91; // G#
            break;
        case 9:
            freq = 55.00; // A
            break;
        case 10:
            freq = 58.27; // A#
            break;
        case 11:
            freq = 61.74; // B
            break;
        default:
            break;
        }
        int octave = tone / 12;
        freq *= pow(2, octave);
        Beep(freq, duration);
    }
}

vector<int> getUserComb(int slots, int maxno, int tryno=0) {
    vector<int> combs;
    for (int i = 0; i < slots; i++)
    {
        combs.push_back(0);
    }
    int selected = 0;
    KCon::CursorVisibility(false);
    while (true) {
        renderCombGrid(combs, selected, tryno);
        fflush(stdin);
        int ch = _getch();
        switch (ch)
        {
        case KEY_LEFT:
            selected -= 1;
            selected = selected < 0 ? slots + selected : selected;
            beepTone(selected * 3 + 12 * 4);
            break;
        case KEY_RIGHT:
            selected += 1;
            selected %= slots;
            beepTone(selected * 3 + 12 * 4);
            break;
        case KEY_UP:
            combs[selected] += 1;
            combs[selected] %= maxno;
            beepTone(selected * 3 + 12 * 4 + 1);
            break;
        case KEY_DOWN:
            combs[selected] -= 1;
            combs[selected] = combs[selected] < 0 ? maxno + combs[selected] : combs[selected];
            beepTone(selected * 3 + 12 * 4 - 1);
            break;
        }
        if (ch == 13)
            break;
    }
    KCon::CursorVisibility(true);
    return combs;
}

string vtos(vector<int> v) {
    string ans = std::to_string(v[0]);
    int vlen = v.size();
    for (int i = 1; i < vlen; i++)
    {
        ans = ans + ", " + std::to_string(v[i]);
    }
    return ans;
}

int main()
{
    srand(time(NULL));

    while (true) {
        int usrinCombinations;
        KCon::write("Cuantas combinaciones?");
        while (true) {
            try {
                usrinCombinations = KCon::readInt();
                if (usrinCombinations <= 0) {
                    KCon::write("Ecriba un numero mayor a 0 >:(", KCon::Color::RED);
                }
                else {
                    KCon::clear();
                    break;
                }
            }
            catch (std::invalid_argument e) {
                KCon::write("Ecriba un numero de velda >:(", KCon::Color::RED);
            }
        }

        bool usrRepeatNumber;
        KCon::write("Repetir numeros? [y/n]");
        while (true) {
            try {
                usrRepeatNumber = KCon::readYN();
                KCon::clear();
                break;
            }
            catch (std::invalid_argument e) {
                KCon::write("Escribe y o n", KCon::Color::RED);
            }
        }

        int usrinMaxNumber;
        KCon::write("Cual es el numero mas grande?");
        while (true) {
            try {
                usrinMaxNumber = KCon::readInt();
                if (!usrRepeatNumber && (usrinCombinations > usrinMaxNumber)) {
                    KCon::write("Si no se repiten numeros no pueden haber mas combinaciones que numeros", KCon::Color::RED);
                } else
                    KCon::clear();
                    break;
            }
            catch (std::invalid_argument e) {
                KCon::write("Ecriba un numero de velda >:(", KCon::Color::RED);
            }
        }

        Game game = newGame(usrinCombinations, usrinMaxNumber, usrRepeatNumber);

        KCon::write("Combinacion Generada. Ingrese sus entradas:");
        bool victory = false;
        while ((game.tries < 10) && !victory) {
            vector<int> attempt = getUserComb(usrinCombinations, game.maxNumber, game.tries);
            vector<Answer> hints = attemptAtGame(&game, attempt);
            renderAnsGrid(hints, attempt, game.tries-1);
            victory = isWin(hints);
        }
        if (victory) {
            KCon::write("Ganaste!", KCon::Color::YELLOW);
        } else {
            KCon::write("Perdiste...", KCon::Color::RED);
        }

        KCon::write("Quieres intentarlo otra vez? [y/n]");
        while (true) {
            try {
                usrRepeatNumber = KCon::readYN();
                KCon::clear();
                break;
            }
            catch (std::invalid_argument e) {
                KCon::write("Escribe y o n", KCon::Color::RED);
            }
        }
    }
}
