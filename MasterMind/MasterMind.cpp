// MasterMind.cpp
// Juego de MasterMind
// Equipo:
//      Jerry       Rivas           1089317
//      Sebastian   Henriquez       1101507
//      Ismael      Dicent          1100368
//      Alex        Bujosa          1098913
//      Steven      Martinez        1101817
// Realizar un programa de C++ que replique el juego de Master Mind

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
#define KEY_ENTER 13

using std::vector;
using std::array;

// Game
// Representa el juego de MasterMind
// Guarda la configuración del juego
struct Game
{
    // El número máximo que puede ingresar el usuario en un campo
    int maxNumber = 6;
    // La cantidad de campos en el juego
    vector<int> requiredCombination;
    // La cantidad de intentos realizados por el jugador
    int tries = 0;
};

// Answer (Respuesta)
// Representa que tan correcta es una respuesta en el juego
enum class Answer
{
    C, // El número está en su posición
    F, // El número está en la combinación
    X // El número no está en la combinación
};

// ansToChar (Respuesta a Caracter)
// Convierte una respuesta (Answer) a un caracter referente a esa respuesta
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

// isWin (Es respuesta ganadora)
// Revisa si una lista de respuestas es una lista de respuestas correcta
// Retorna verdadero si todos los valores son correctos (C) y falso en cualquier otro momento
bool isWin(vector<Answer> ansComb) {
    // Para todos los valores dentro de la lista de respuestas...
    int vectorSize = ansComb.size();
    for (int i = 0; i < vectorSize; i++)
    {
        // ... Si no es correcta, retorna falso
        if (Answer::C != ansComb[i]) {
            return false;
        }
    }
    return true;
}

// isIn (está dentro)
// Retorna verdadero si el número dado está dentro de la lista dada
bool isIn(int number, vector<int> haystack) {
    // Por cada número dentro del vector...
    int vectorSize = haystack.size();
    for (int i = 0; i < vectorSize; i++)
    {
        // ...Si lo encuentra, retorna verdadero
        if (haystack[i] == number) {
            return true;
        }
    }
    return false;
}

// attemptAtGame (Intento de juego)
// Obtiene las pistas del juego según una combinación de números que dio el jugador.
// retorna un vector de pistas que, para cada numero en la combinación dada...
// ... retorna C si el número de la combinacion dada está en la misma posición que el mismo numero en la
//     combinación requerida
// ... retorna F si el número está en la combinación oculta
// ... retorna X si el número no está en la combinación oculta
// Agrega un intento al juego
//
// combination es una lista de respuestas en número
vector<Answer> attemptAtGame(Game* game, vector<int> combination) {
    // Obtenemos los tamaños de las listas
    int vectorSize = game->requiredCombination.size();
    int attemptSize = combination.size();

    // Lista de respuestas correctas
    vector<Answer> answers;

    // Programación defensiva: Si el tamaño de la contraseña y los intentos son diferentes...
    if (vectorSize != attemptSize) {
        // Retorna las respuestas vacías
        return answers;
    }

    // Por cada elemento dentro de las respuestas e intento...
    for (int i = 0; i < vectorSize; i++)
    {
        int columnAttempt = combination[i];
        // Si los de la misma posición son iguales ...
        if (game->requiredCombination[i] == columnAttempt) {
            // Agrega C a la lista de pistas
            answers.push_back(Answer::C); 
        } // Si el número está en la lista ...
        else if (isIn(columnAttempt, game->requiredCombination)) {
            // Agrega F a la lista de pistas
            answers.push_back(Answer::F);
        } // De lo contrario ...
        else {
            // Agrega X a la lista de pistas
            answers.push_back(Answer::X);
        }
    }
    // Finalmente, agrega un intento al juego
    game->tries += 1;
    return answers;
}

// getBiggestNumber (Obtener número más grande)
// Pregunta al usuario cual será el número más posible grande del juego
// retorna el numero ingresado
// No permite numeros menores a 0
// No permite menos numeros que combinaciones si no se repiten numeros
int getBiggestNumber(int usrinCombinations, bool usrRepeatNumber) {
    int usrinBigNumber;
    KCon::write("Cual es el numero mas grande en la combinacion?");
    while (true) {
        try {
            // Lee un número entero
            usrinBigNumber = KCon::readInt();
            // Excepción si se repiten numeros
            if (!usrRepeatNumber && (usrinCombinations > usrinBigNumber)) {
                KCon::write("Si no se repiten numeros no pueden haber mas combinaciones que numeros", KCon::Color::RED);
            } else if (usrinBigNumber <= 0) // Excepción con numeros menores o 0
                KCon::write("No es posible tener numeros menores que o 0");
            else { // Numero valido
                KCon::clear();
                break;
            }
        } catch (std::invalid_argument e) { // Numero inválido
            KCon::write("Escriba un numero valido", KCon::Color::RED);
        }
    }
    return usrinBigNumber;
}

// Crea un número aleatorio entre minimo y maximo
int rand(int min, int max) {
    return max == 0 ? 0 : (min + rand() % max);
}

// Crea un número aleatorio entre 0 y máximo
int rand(int max) {
    return rand(0, max);
}

// Obtiene un número aleatorio dentro de una lista
int popRandom(vector<int>* vec) {
    int removedIndex = rand(vec->size());
    int removedNo = vec->at(removedIndex);
    vec->erase(vec->begin() + removedIndex);
    return removedNo;
}

// Genera una lista de números aleatorios según...
// ... un tamaño (listSize)
// ... el número más grande
// ... si se permiten repeticiones dentro de la lista
vector<int> generateNumArray(int listSize, int biggestNumber, bool repeat = false) {
    // Creamos una lista base para obtener numeros aleatorios.
    vector<int> baseList;
    // Si se puede repetir...
    if (repeat) {
        /// ... Genera todos los números aleatoriamente
        for (int i = 0; i < listSize; i++)
        {
            baseList.push_back(rand(biggestNumber));
        }
    } else { // Si no ...
        // ... Genera todos los números en orden
        for (int i = 0; i < biggestNumber; i++)
        {
            baseList.push_back(i);
        }
    }
    // Inicializamos las respuestas finales
    vector<int> combinations;
    // Mientras no tenga el tamaño que queremos...
    while (combinations.size() < listSize) {     
        // Quita un numero aleatorio de los numeros generados y agregalo a la lista
        combinations.push_back(popRandom(&baseList));        
    }
    return combinations;
}

// Crea un nuevo juego con una combinacion de un tamaño, un numero máximo,
// y la regla de si es posible repetir numeros en la combinacion
Game newGame(int combinationSize, int newGameMaxNumber, bool usrRepeatNumber) {
    Game newGame = Game();
    newGame.maxNumber = newGameMaxNumber;
    newGame.requiredCombination = generateNumArray(combinationSize, newGameMaxNumber, usrRepeatNumber);
    return newGame;
}

// Centra un texto con un ancho querido (width) 
string padCenter(string str, int width) {
    int strLen = str.length();
    int setWidth = width < strLen ? strLen : width;
    int pad = setWidth - str.length();
    int right = pad / 2;
    int left = pad - right;
    return string(left, ' ') + str + string(right, ' ');
}

// Centra un caracter con un ancho querido (width) 
string padCenter(char str, int width) {
    return padCenter(string(1, str), width);
}

// Centra un número con un ancho requerido (width)
string padCenter(int number, int width) {
    string str = std::to_string(number);
    return padCenter(str, width);
}

// Renderiza (muestra) una lista de numeros, cambiando color a los seleccionados
// Muestra a una altura menor los numeros según los intentos
void renderCombGrid(vector<int> combs, int selected, int tryno) {
    int combsize = combs.size();
    int fieldWidth = 5;
    int fieldPadding = 1;
    for (int i = 0; i < combsize; i++)
    {
        for (int k = 0; k < 3; k++) 
        {
            // Calculamos la coordenada del cuadro a renderizar
            int xcoord = i * (fieldWidth + 2 * fieldPadding);
            int ycoord = tryno * 3 + 1 + k;
            // Colocamos el cursor en esta posicion
            KCon::CursorPosition(xcoord, ycoord);
            // Aseguramos lo que vamos a mostrar
            string middleString = " ";
            string leftChar = " ";
            string rightChar = " ";
            // Los colores por defecto
            KCon::Color fg = KCon::Color::GREEN;
            KCon::Color bg = KCon::Color::BLACK;
            switch (k)
            { // En la primera linea
            case 0: // Si está seleccionado, muestra una flecha
                if (i == selected) {
                    middleString = padCenter("^", fieldWidth);
                }
                break;
            case 1:// En la segunda linea
                middleString = padCenter(combs[i] + 1, fieldWidth); // Pon el numero que está
                if (i == selected) { // Si está seleccionado, muestra flechas
                    leftChar = "<";
                    rightChar = ">"; // Y cambia el color
                    fg = KCon::Color::BLACK;
                    bg = KCon::Color::GREEN;
                }
                break;
            case 2: // En la tercera linea
                if (i == selected) { // Si está seleccionado, muestra flecha
                    middleString = padCenter("v", fieldWidth);
                }
                break;
            }
            // Muestra según las características de la linea
            KCon::write(' ', leftChar + middleString + rightChar, fg, bg);
        }
    }
}

// Renderiza (muestra) las pistas con colores, y la altura segun el intento
void renderAnsGrid(vector<Answer> hints, vector<int> prev, int tryno) {
    // Por cada pista...
    int hintSize = hints.size();
    for (int i = 0; i < hintSize; i++)
    {
        // ... Decide su color
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
        // ... Consigue su caracter
        char hint = ansToChar(hints[i]);
        // .. Renderise segun tamaño
        int width = 7;
        for (int k = 0; k < 3; k++)
        {
            // Determina la coordenada
            int xcoord = i * width;
            int ycoord = tryno * 3 + 1 + k;
            KCon::CursorPosition(xcoord, ycoord);
            switch (k)
            {
            case 0:
                KCon::write(' ', " " + padCenter(hint, 5) + " ", color);
                break;
            case 1:
                KCon::write(' ', " " + padCenter(prev[i] + 1, 5) + " ", color);
                break;
            case 2:
                KCon::write(' ', " " + padCenter(hint, 5) + " ", color);
                break;
            }
        }
    }
}

// Hace un sonido según un tono (> 1, desde Do1 hasta n)
// Como Beep() pausa el programa, se llama desde otro hilo (otr
void beepTone(int tone, int duration = 100, bool th = true) {
    // Si se debe llamar en otro hilo...
    if (th) { 
        std::thread t1(beepTone, tone, duration, false); // Tigueraje
        t1.detach();
    } else { // Desde el otro hilo
        // Calcula la base de la frecuencia a tocar segun el tono
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
        // Calcula la octava segun la frecuencia
        int octave = tone / 12;
        freq *= pow(2, octave);
        // Toca la frecuencia
        Beep(freq, duration);
    }
}

// Funcion que permite al usuario seleccionar la combinacion que quiere
vector<int> getUserComb(int slots, int maxno, int tryno = 0) {
    vector<int> combs;
    // Llena de 0
    for (int i = 0; i < slots; i++)
        combs.push_back(0);
    // El primer seleccionado por defecto es el primero
    int selected = 0;
    KCon::CursorVisibility(false);
    while (true) {
        renderCombGrid(combs, selected, tryno);
        fflush(stdin);
        int ch = _getch();
        switch (ch)
        { // Da la vuelta (loop over) si se pasa por arriba o por abajo
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
        if (ch == KEY_ENTER)
            break;
    }
    KCon::CursorVisibility(true);
    return combs;
}

// Toma un vector y lo convierte en una cadena de caracteres separada por coma
// Se utilizó para hacer pruebas
string vectorToString(vector<int> v) {
    string ans = std::to_string(v[0]);
    int vlen = v.size();
    for (int i = 1; i < vlen; i++)
    {
        ans = ans + ", " + std::to_string(v[i]);
    }
    return ans;
}

void generateRandomComment(vector<string> bank) {
    int bankSize = bank.size() + 1;
    int randBank = rand(bankSize);
    if (randBank == 0) {
        KCon::write("Y entonce como tu ere duro yo me voy. Hablamo el malte.", KCon::Color::BLACK, KCon::Color::RED);
        Sleep(1000);
        exit(0);
    }
    else {
        KCon::write(bank[randBank - 1], KCon::Color::DARKCYAN);
        Sleep(1000);
    }
}

// Papi main
int main()
{
    vector<string> creativeGrudge;
    creativeGrudge.push_back("Va' segui?");
    creativeGrudge.push_back("Anjá, que ma?");
    creativeGrudge.push_back("Pero entonce tu ere duro?");
    creativeGrudge.push_back("Pero entonce tu te gobielna?");
    creativeGrudge.push_back("Pero yo soy loco?");
    creativeGrudge.push_back(">:(");
    creativeGrudge.push_back("Loco pero como asi?");
    creativeGrudge.push_back("A que eploto la pc");
    creativeGrudge.push_back("Ruede durisimo :|");
    creativeGrudge.push_back("Te falta calle");
    creativeGrudge.push_back("Te falta pampara");
    creativeGrudge.push_back("Con el lapi no");
    creativeGrudge.push_back("No");
    creativeGrudge.push_back("Tu sabe' sumar?");
    creativeGrudge.push_back("El PLD se robo tu inteligencia fue?");
    creativeGrudge.push_back("La Barrick Gold estaria orgulloso");
    creativeGrudge.push_back("Pero pol que?");
    creativeGrudge.push_back("Culpa del PRD");
    creativeGrudge.push_back("Cual corrupcion?");
    creativeGrudge.push_back("Llegaste a Puerto Plata o a Santiago?");
    creativeGrudge.push_back("Turbio");
    creativeGrudge.push_back("Bajaste sin trenza");
    creativeGrudge.push_back("Te va dobla?");
    creativeGrudge.push_back("Partete una pierna >:(");
    creativeGrudge.push_back("No me hagas eso :(");
    creativeGrudge.push_back("Va de ahi");
    creativeGrudge.push_back("Licey Campeon");
    creativeGrudge.push_back("Estrellitas donde estan?");
    creativeGrudge.push_back("RUGE EL LEOOOOOONNNNNNNN");
    creativeGrudge.push_back("Hace falta Trujillo por aqui");
    creativeGrudge.push_back("Se va armar un lio");
    creativeGrudge.push_back("Deja el relajo");
    creativeGrudge.push_back("Ya ta bueno");
    creativeGrudge.push_back("El programa explotara"); //No era necesario

    srand(time(NULL));
    while (true) {
        int usrinFields;
        KCon::write("Cuantas combinaciones?");
        while (true) {
            try {
                usrinFields = KCon::readInt();
                if (usrinFields <= 0) {
                    generateRandomComment(creativeGrudge);
                    KCon::write("Ecriba un numero mayor a 0 >:(", KCon::Color::RED);
                }
                else {
                    KCon::clear();
                    break;
                }
            }
            catch (std::invalid_argument e) {
                generateRandomComment(creativeGrudge);
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
                generateRandomComment(creativeGrudge);
                KCon::write("Escribe y o n", KCon::Color::RED);
            }
        }

        int usrinBigNumber;
        while (true) {
            usrinBigNumber = getBiggestNumber(usrinFields, usrRepeatNumber);
            if (!usrRepeatNumber && (usrinFields > usrinBigNumber)) {
                generateRandomComment(creativeGrudge);
                KCon::write("Si no se repiten numeros, deben haber mas o igual numeros que campos", KCon::Color::RED);
            }
            else
                break;
        }

        int usrinTries;
        KCon::write("Cuantos intentos?");
        while (true) {
            try {
                usrinTries = KCon::readInt();
                if (usrinTries <= 0) {
                    generateRandomComment(creativeGrudge);
                    KCon::write("Ecriba un numero mayor a 0 >:(", KCon::Color::RED);
                }
                else {
                    KCon::clear();
                    break;
                }
            }
            catch (std::invalid_argument e) {
                generateRandomComment(creativeGrudge);
                KCon::write("Ecriba un numero de velda >:(", KCon::Color::RED);
            }
        }

        Game game = newGame(usrinFields, usrinBigNumber, usrRepeatNumber);
        bool victory = false;
        KCon::write("Combinacion Generada. Ingrese sus entradas:");
        while (!victory && (game.tries < usrinTries)) {
            vector<int> attempt = getUserComb(usrinFields, game.maxNumber, game.tries);
            vector<Answer> hints = attemptAtGame(&game, attempt);
            renderAnsGrid(hints, attempt, game.tries - 1);
            victory = isWin(hints);
        }
        float maxScore = 100.0 * usrinFields * usrinBigNumber * (usrRepeatNumber ? usrinBigNumber : 1) / usrinTries;
        if (victory) {
            KCon::write("Ganaste! - Puntaje: " + std::to_string((maxScore / game.tries)), KCon::Color::YELLOW);
        }
        else {
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
                
                generateRandomComment(creativeGrudge);
                KCon::write("Escribe y o n", KCon::Color::RED);
            }
            
        }
        if (!usrRepeatNumber) {
            break;
        }
    }
}
