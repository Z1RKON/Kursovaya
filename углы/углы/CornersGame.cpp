#include "CornersGame.h"
#include <iostream>

// ����������� ������ CornersGame
CornersGame::CornersGame() : currentPlayer(1), board(boardSize, std::vector<int>(boardSize, 0)) {
    InitializeBoard(board);
}

// ������������� ������� �����
void CornersGame::InitializeBoard(std::vector<std::vector<int>>& board) {
    // ������� �����
    board.resize(boardSize, std::vector<int>(boardSize, 0));

    // ����������� ����� ��� ������� ������ (1) �������� ���� � �����
    for (int y = 0; y < 3; ++y) {
        for (int x = 0; x < 4; ++x) {
            board[y][x] = 1; // ��������� ������������� 3�4 �������
        }
    }

    // ����������� ����� ��� ������� ������ (2) �������� ���� � �����
    for (int y = boardSize - 3; y < boardSize; ++y) {
        for (int x = boardSize - 4; x < boardSize; ++x) {
            board[y][x] = 2; // ��������� ������������� 3�4 �������
        }
    }
}

// ������������� ����
void CornersGame::InitializeGame() {
    // ��������� ����� ������� ��������
    for (int y = 0; y < boardSize; ++y) {
        for (int x = 0; x < boardSize; ++x) {
            board[y][x] = 0;
        }
    }

    // ����������� ����� ��� ������� ������ (1) �������� ���� � �����
    for (int y = 0; y < 3; ++y) {
        for (int x = 0; x < 4; ++x) {
            board[y][x] = 1; // ��������� ������������� 3�4 �������
        }
    }

    // ����������� ����� ��� ������� ������ (2) �������� ���� � �����
    for (int y = boardSize - 3; y < boardSize; ++y) {
        for (int x = boardSize - 4; x < boardSize; ++x) {
            board[y][x] = 2; // ��������� ������������� 3�4 �������
        }
    }

    // ����� �������� ������
    currentPlayer = 1;
}


// ����������� ����
void CornersGame::DisplayMenu() {
    std::cout << "1. ������� ���\n";
    std::cout << "2. ������ ����� ����\n";
    std::cout << "3. ����� �� ����\n";
    std::cout << "�������� �����: ";
}


// ��������� �����
void CornersGame::SetColor(int textColor, int bgColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (WORD)((bgColor << 4) | textColor));
}

// ����������� ������� �����
void CornersGame::DisplayBoard() {
    // ������� ������ ��������
    std::cout << "  ";
    for (int x = 0; x < boardSize; ++x) {
        std::cout << x << "   ";
    }
    std::cout << std::endl;

    for (int y = 0; y < boardSize; ++y) {
        // ������� ����� ������ ����� ������ ������� �����
        std::cout << y << " ";
        for (int x = 0; x < boardSize; ++x) {
            if (board[y][x] == 1) {
                SetColor(15, 0); // ������������� ����� ���� ��� ����� �����
                std::cout << 'O';
            }
            else if (board[y][x] == 2) {
                SetColor(0, 15); // ������������� ������ ���� ��� ������ �����
                std::cout << 'X';
            }
            else {
                SetColor(7, 0); // ������������� ����� ���� ��� ������ ������
                std::cout << '.';
            }
            SetColor(7, 0); // ���������� ����������� ���� ����� ������ ������
            std::cout << "   "; // ����������� ���������� ����� �������
        }
        std::cout << std::endl;
        std::cout << std::endl; // ��������� ������ ����� ��������
    }
}


// ����������� ��������� �������
void CornersGame::DisplayAvailableJumps(const std::vector<std::pair<int, int>>& jumps) {
    std::cout << "��������� ����:" << std::endl;
    for (const auto& jump : jumps) {
        std::cout << "(" << jump.first << ", " << jump.second << ")" << std::endl;
    }
}

// ���������� ����
bool CornersGame::MakeMove(int startY, int startX, int endY, int endX) {
    std::cout << "�������� ���������: (" << startY << ", " << startX << ") -> (" << endY << ", " << endX << ")\n";

    // ��������, ��� ��������� � �������� ����� ��������� � �������� �����
    if (startY < 0 || startY >= boardSize || startX < 0 || startX >= boardSize ||
        endY < 0 || endY >= boardSize || endX < 0 || endX >= boardSize) {
        std::cout << "���������� ��� �����.\n";
        return false;
    }

    // ��������, ��� �� ��������� ������� ���� �����
    if (board[startY][startX] == 0) {
        std::cout << "�� ��������� ������� ��� �����.\n";
        return false;
    }

    // ��������, ��� �������� ������� �����
    if (board[endY][endX] != 0) {
        std::cout << "�������� ������� �� �����.\n";
        return false;
    }

    // ��������, ��� ��� ��������
    if (!IsMoveValid(startY, startX, endY, endX)) {
        std::cout << "��� ����������.\n";
        return false;
    }

    // ���������� ����
    board[endY][endX] = board[startY][startX];
    board[startY][startX] = 0;

    std::cout << "��� ��������.\n";
    return true;
}

// ����� ��� ����������� ����������� ��������������
bool CornersGame::CanJump(int currentY, int currentX) {
    // ��������� ����������� �������������� � ������ �����������
    static const int directions[4][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
    for (const auto& dir : directions) {
        int nextY = currentY + dir[0] * 2;
        int nextX = currentX + dir[1] * 2;
        int middleY = currentY + dir[0];
        int middleX = currentX + dir[1];

        // ���������, ��� ��������� ������ ��������� � �������� �����
        if (nextY >= 0 && nextY < boardSize && nextX >= 0 && nextX < boardSize) {
            // ���������, ��� ��������� ������ ����� � ����� ������� � ��������� ������� ���� �����
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
                // ��������� ��������� ��� � ������
                jumps.push_back({ nextY, nextX });
                // ������� �����, ����� ������� ������������
                board[middleY][middleX] = 0;
                // ���������� ���� ��������� ��������������
                FindAllJumps(nextY, nextX, jumps);
                // ���������� ����� ������� ��� ������ ��������� �����
                board[middleY][middleX] = 1;
            }
        }
    }
}

// ���������� ������
void CornersGame::PerformJump(int startY, int startX) {
    std::vector<std::pair<int, int>> availableJumps;
    FindAllJumps(startY, startX, availableJumps);

    // ���������, ���� �� ��������� ���� ����� ��������������
    if (!availableJumps.empty()) {
        // ����� ��� ��� ������ ��������� ����� � ������ ���������� ���� �������
        // ...
    }
}

bool CornersGame::IsMoveValid(int startY, int startX, int endY, int endX) {
    // ��������, ��� ��������� � �������� ����� ��������� � �������� �����
    if (startY < 0 || startY >= boardSize || startX < 0 || startX >= boardSize ||
        endY < 0 || endY >= boardSize || endX < 0 || endX >= boardSize) {
        return false;
    }

    // ��������, ��� �� ��������� ������� ���� �����
    if (board[startY][startX] == 0) {
        return false;
    }

    // ��������, ��� �������� ������� �����
    if (board[endY][endX] != 0) {
        return false;
    }

    // ������ ������� ��������� ����� ��������� � �������� ���������
    int deltaY = abs(endY - startY);
    int deltaX = abs(endX - startX);

    // �������� �� ����������� �� ���� ������ �� ����������� ��� ���������
    if (deltaY + deltaX == 1) {
        return true;
    }

    // �������� �� �������������� ����� �����
    if ((deltaY == 2 && deltaX == 0) || (deltaX == 2 && deltaY == 0)) {
        int middleY = (startY + endY) / 2;
        int middleX = (startX + endX) / 2;

        // ��������, ��� ����� ��������� � �������� ��������� ���� ����� ��� ��������������
        if (board[middleY][middleX] != 0) {
            return true;
        }
    }

    return false;
}

// ������� ����
void CornersGame::GameLoop() {
    while (true) {
        DisplayBoard();
        DisplayMenu();
        int choice;
        std::cin >> choice;
        switch (choice) {
        case 1: {
            int startX, startY, endX, endY;
            std::cout << "������� ���������� ��������� ������� (X Y): ";
            std::cin >> startX >> startY;
            std::cout << "������� ���������� �������� ������� (X Y): ";
            std::cin >> endX >> endY;
            if (MakeMove(startX, startY, endX, endY)) {
                std::cout << "��� ��������.\n";
                if (CheckWin()) {
                    std::cout << "���� ��������!\n";
                    return;
                }
            }
            else {
                std::cout << "���������� ��������� ���. ���������� ������ ����������.\n";
            }
            break;
        }
        case 2: {
            InitializeGame(); // ����������������� ����
            std::cout << "����� ���� ��������!\n";
            break;
        }
        case 3:
            std::cout << "������� �� ����!\n";
            return; // ����� �� ����
        default:
            std::cout << "�������� �����. ����������, ���������� �����.\n";
        }
    }
}


// �������� �� ������
bool CornersGame::CheckWin() {
    bool whiteWins = true;
    bool blackWins = true;
    int winConditionSize = 4;
    // ���������, ��� �� ����� ����� � ������ ����
    for (int y = 0; y < winConditionSize; ++y) {
        for (int x = 0; x < winConditionSize; ++x) {
            if (board[y][x] != 1) {
                whiteWins = false;
                break;
            }
        }
        if (!whiteWins) break;
    }

    // ���������, ��� �� ������ ����� � ����� ����
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
        std::cout << "����� ��������!" << std::endl;
        return true;
    }
    else if (blackWins) {
        std::cout << "׸���� ��������!" << std::endl;
        return true;
    }

    return false; // ���������� ��� �� ��������
}