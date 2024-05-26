// CornersGame.h 
#ifndef CORNERSGAME_H 
#define CORNERSGAME_H 

#include <vector> 
#include <windows.h> 

class CornersGame {
public:
    CornersGame();
    void InitializeBoard(std::vector<std::vector<int>>& board);
    void InitializeGame();
    void DisplayMenu();
    void SetColor(int textColor, int bgColor);
    void DisplayBoard();
    void DisplayAvailableJumps(const std::vector<std::pair<int, int>>& jumps);
    bool MakeMove(int startY, int startX, int endY, int endX);
    bool CanJump(int currentY, int currentX);
    void PerformJump(int startY, int startX);
    bool IsMoveValid(int startY, int startX, int endY, int endX);
    void GameLoop();
    bool CheckWin();

private:
    std::vector<std::vector<int>> board;
    const int boardSize = 8;
    int currentPlayer;
    void FindAllJumps(int startY, int startX, std::vector<std::pair<int, int>>& jumps);
};

#endif // CORNERSGAME_H