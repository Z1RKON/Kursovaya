#include "CornersGame.h"
#include <iostream>

// Конструктор класса CornersGame
CornersGame::CornersGame() : currentPlayer(1), board(boardSize, std::vector<int>(boardSize, 0)) {
    InitializeBoard(board);
}

// Инициализация игровой доски
void CornersGame::InitializeBoard(std::vector<std::vector<int>>& board) {
    // Очистка доски
    board.resize(boardSize, std::vector<int>(boardSize, 0));

    // Расстановка шашек для первого игрока (1) вплотную друг к другу
    for (int y = 0; y < 3; ++y) {
        for (int x = 0; x < 4; ++x) {
            board[y][x] = 1; // Заполняем прямоугольник 3х4 шашками
        }
    }

    // Расстановка шашек для второго игрока (2) вплотную друг к другу
    for (int y = boardSize - 3; y < boardSize; ++y) {
        for (int x = boardSize - 4; x < boardSize; ++x) {
            board[y][x] = 2; // Заполняем прямоугольник 3х4 шашками
        }
    }
}

// Инициализация игры
void CornersGame::InitializeGame() {
    // Заполняем доску пустыми клетками
    for (int y = 0; y < boardSize; ++y) {
        for (int x = 0; x < boardSize; ++x) {
            board[y][x] = 0;
        }
    }

    // Расставляем шашки для первого игрока (1) вплотную друг к другу
    for (int y = 0; y < 3; ++y) {
        for (int x = 0; x < 4; ++x) {
            board[y][x] = 1; // Заполняем прямоугольник 3х4 шашками
        }
    }

    // Расставляем шашки для второго игрока (2) вплотную друг к другу
    for (int y = boardSize - 3; y < boardSize; ++y) {
        for (int x = boardSize - 4; x < boardSize; ++x) {
            board[y][x] = 2; // Заполняем прямоугольник 3х4 шашками
        }
    }

    // Сброс текущего игрока
    currentPlayer = 1;
}


// Отображение меню
void CornersGame::DisplayMenu() {
    std::cout << "1. Сделать ход\n";
    std::cout << "2. Начать новую игру\n";
    std::cout << "3. Выйти из игры\n";
    std::cout << "Выберите опцию: ";
}


// Установка цвета
void CornersGame::SetColor(int textColor, int bgColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (WORD)((bgColor << 4) | textColor));
}

// Отображение игровой доски
void CornersGame::DisplayBoard() {
    // Выводим номера столбцов
    std::cout << "  ";
    for (int x = 0; x < boardSize; ++x) {
        std::cout << x << "   ";
    }
    std::cout << std::endl;

    for (int y = 0; y < boardSize; ++y) {
        // Выводим номер строки перед каждой строкой доски
        std::cout << y << " ";
        for (int x = 0; x < boardSize; ++x) {
            if (board[y][x] == 1) {
                SetColor(15, 0); // Устанавливаем белый цвет для белых шашек
                std::cout << 'O';
            }
            else if (board[y][x] == 2) {
                SetColor(0, 15); // Устанавливаем чёрный цвет для чёрных шашек
                std::cout << 'X';
            }
            else {
                SetColor(7, 0); // Устанавливаем серый цвет для пустых клеток
                std::cout << '.';
            }
            SetColor(7, 0); // Возвращаем стандартный цвет после каждой клетки
            std::cout << "   "; // Увеличиваем расстояние между точками
        }
        std::cout << std::endl;
        std::cout << std::endl; // Добавляем пробел между строками
    }
}


// Отображение доступных прыжков
void CornersGame::DisplayAvailableJumps(const std::vector<std::pair<int, int>>& jumps) {
    std::cout << "Доступные ходы:" << std::endl;
    for (const auto& jump : jumps) {
        std::cout << "(" << jump.first << ", " << jump.second << ")" << std::endl;
    }
}

// Совершение хода
bool CornersGame::MakeMove(int startY, int startX, int endY, int endX) {
    std::cout << "Проверка координат: (" << startY << ", " << startX << ") -> (" << endY << ", " << endX << ")\n";

    // Проверка, что начальная и конечная точки находятся в пределах доски
    if (startY < 0 || startY >= boardSize || startX < 0 || startX >= boardSize ||
        endY < 0 || endY >= boardSize || endX < 0 || endX >= boardSize) {
        std::cout << "Координаты вне доски.\n";
        return false;
    }

    // Проверка, что на начальной позиции есть шашка
    if (board[startY][startX] == 0) {
        std::cout << "На начальной позиции нет шашки.\n";
        return false;
    }

    // Проверка, что конечная позиция пуста
    if (board[endY][endX] != 0) {
        std::cout << "Конечная позиция не пуста.\n";
        return false;
    }

    // Проверка, что ход допустим
    if (!IsMoveValid(startY, startX, endY, endX)) {
        std::cout << "Ход недопустим.\n";
        return false;
    }

    // Выполнение хода
    board[endY][endX] = board[startY][startX];
    board[startY][startX] = 0;

    std::cout << "Ход выполнен.\n";
    return true;
}

// Метод для определения возможности перепрыгивания
bool CornersGame::CanJump(int currentY, int currentX) {
    // Проверяем возможность перепрыгивания в каждом направлении
    static const int directions[4][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
    for (const auto& dir : directions) {
        int nextY = currentY + dir[0] * 2;
        int nextX = currentX + dir[1] * 2;
        int middleY = currentY + dir[0];
        int middleX = currentX + dir[1];

        // Проверяем, что следующая клетка находится в пределах доски
        if (nextY >= 0 && nextY < boardSize && nextX >= 0 && nextX < boardSize) {
            // Проверяем, что следующая клетка пуста и между текущей и следующей клеткой есть шашка
            if (board[nextY][nextX] == 0 && board[middleY][middleX] != 0) {
                return true;
            }
        }
    }
    return false;
}

void CornersGame::FindAllJumps(int currentY, int currentX, std::vector<std::pair<int, int>>& jumps) {
    if (CanJump(currentY, currentX)) {
        static const int directions[4][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
        for (const auto& dir : directions) {
            int nextY = currentY + dir[0] * 2;
            int nextX = currentX + dir[1] * 2;
            int middleY = currentY + dir[0];
            int middleX = currentX + dir[1];

            if (nextY >= 0 && nextY < boardSize && nextX >= 0 && nextX < boardSize &&
                board[nextY][nextX] == 0 && board[middleY][middleX] != 0) {
                // Добавляем возможный ход в список
                jumps.push_back({ nextY, nextX });
                // Удаляем шашку, через которую перепрыгнули
                board[middleY][middleX] = 0;
                // Рекурсивно ищем следующие перепрыгивания
                FindAllJumps(nextY, nextX, jumps);
                // Возвращаем шашку обратно для других возможных ходов
                board[middleY][middleX] = 1;
            }
        }
    }
}

// Выполнение прыжка
void CornersGame::PerformJump(int startY, int startX) {
    std::vector<std::pair<int, int>> availableJumps;
    FindAllJumps(startY, startX, availableJumps);

    // Проверяем, есть ли доступные ходы после перепрыгивания
    if (!availableJumps.empty()) {
        // Здесь код для вывода доступных ходов и выбора следующего хода игроком
        // ...
    }
}

bool CornersGame::IsMoveValid(int startY, int startX, int endY, int endX) {
    // Проверка, что начальная и конечная точки находятся в пределах доски
    if (startY < 0 || startY >= boardSize || startX < 0 || startX >= boardSize ||
        endY < 0 || endY >= boardSize || endX < 0 || endX >= boardSize) {
        return false;
    }

    // Проверка, что на начальной позиции есть шашка
    if (board[startY][startX] == 0) {
        return false;
    }

    // Проверка, что конечная позиция пуста
    if (board[endY][endX] != 0) {
        return false;
    }

    // Расчет разницы координат между начальной и конечной позициями
    int deltaY = abs(endY - startY);
    int deltaX = abs(endX - startX);

    // Проверка на перемещение на одну клетку по горизонтали или вертикали
    if (deltaY + deltaX == 1) {
        return true;
    }

    // Проверка на перепрыгивание через шашку
    if ((deltaY == 2 && deltaX == 0) || (deltaX == 2 && deltaY == 0)) {
        int middleY = (startY + endY) / 2;
        int middleX = (startX + endX) / 2;

        // Проверка, что между начальной и конечной позициями есть шашка для перепрыгивания
        if (board[middleY][middleX] != 0) {
            return true;
        }
    }

    return false;
}

// Игровой цикл
void CornersGame::GameLoop() {
    while (true) {
        DisplayBoard();
        DisplayMenu();
        int choice;
        std::cin >> choice;
        switch (choice) {
        case 1: {
            int startX, startY, endX, endY;
            std::cout << "Введите координаты начальной позиции (X Y): ";
            std::cin >> startX >> startY;
            std::cout << "Введите координаты конечной позиции (X Y): ";
            std::cin >> endX >> endY;
            if (MakeMove(startX, startY, endX, endY)) {
                std::cout << "Ход выполнен.\n";
                if (CheckWin()) {
                    std::cout << "Игра окончена!\n";
                    return;
                }
            }
            else {
                std::cout << "Невозможно выполнить ход. Попробуйте другие координаты.\n";
            }
            break;
        }
        case 2: {
            InitializeGame(); // Переинициализация игры
            std::cout << "Новая игра началась!\n";
            break;
        }
        case 3:
            std::cout << "Спасибо за игру!\n";
            return; // Выход из игры
        default:
            std::cout << "Неверный выбор. Пожалуйста, попробуйте снова.\n";
        }
    }
}


// Проверка на победу
bool CornersGame::CheckWin() {
    bool whiteWins = true;
    bool blackWins = true;
    int winConditionSize = 4;
    // Проверяем, все ли белые шашки в чёрном углу
    for (int y = 0; y < winConditionSize; ++y) {
        for (int x = 0; x < winConditionSize; ++x) {
            if (board[y][x] != 1) {
                whiteWins = false;
                break;
            }
        }
        if (!whiteWins) break;
    }

    // Проверяем, все ли чёрные шашки в белом углу
    for (int y = boardSize - winConditionSize; y < boardSize; ++y) {
        for (int x = boardSize - winConditionSize; x < boardSize; ++x) {
            if (board[y][x] != 2) {
                blackWins = false;
                break;
            }
        }
        if (!blackWins) break;
    }

    if (whiteWins) {
        std::cout << "Белые победили!" << std::endl;
        return true;
    }
    else if (blackWins) {
        std::cout << "Чёрные победили!" << std::endl;
        return true;
    }

    return false; // Победитель ещё не определён
}