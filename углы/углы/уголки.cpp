#include <iostream>
#include "CornersGame.h"

int main() {
    setlocale(LC_ALL, "Russian");
    CornersGame game; // Создаем объект игры
    game.InitializeGame(); // Инициализируем игру

    game.GameLoop(); // Запускаем главный игровой цикл

    return 0; // Завершаем программу
}