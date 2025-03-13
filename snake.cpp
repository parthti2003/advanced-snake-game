#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <fstream>
#include <thread>
#include <mutex>

using namespace std;

const int width = 20;
const int height = 20;

bool gameOver;
int x, y, fruitX, fruitY, score;
vector<pair<int, int>> snake;
int dir; // 1 - LEFT, 2 - RIGHT, 3 - UP, 4 - DOWN
mutex mtx;

void Setup() {
    gameOver = false;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    dir = 2; // Start moving to the right
    snake.clear();
    snake.push_back({x, y});
}

void Draw() {
    COORD cursorPos = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPos);

    for (int i = 0; i < width + 2; i++) cout << "#";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0) cout << "#";
            if (i == y && j == x)
                cout << "O";
            else if (i == fruitY && j == fruitX)
                cout << "F";
            else {
                bool print = false;
                for (auto &s : snake) {
                    if (s.first == j && s.second == i) {
                        cout << "o";
                        print = true;
                        break;
                    }
                }
                if (!print) cout << " ";
            }
            if (j == width - 1) cout << "#";
        }
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++) cout << "#";
    cout << "\nScore: " << score << endl;
}


void Input() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'a': if (dir != 2) dir = 1; break;
            case 'd': if (dir != 1) dir = 2; break;
            case 'w': if (dir != 4) dir = 3; break;
            case 's': if (dir != 3) dir = 4; break;
            case 'x': gameOver = true; break;
        }
    }
}

void Logic() {
    int prevX = snake[0].first;
    int prevY = snake[0].second;
    int prev2X, prev2Y;
    snake[0].first = x;
    snake[0].second = y;
    
    for (size_t i = 1; i < snake.size(); i++) {
        prev2X = snake[i].first;
        prev2Y = snake[i].second;
        snake[i].first = prevX;
        snake[i].second = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
        case 1: x--; break;
        case 2: x++; break;
        case 3: y--; break;
        case 4: y++; break;
    }

    if (x >= width || x < 0 || y >= height || y < 0)
        gameOver = true;

    for (size_t i = 1; i < snake.size(); i++) {
        if (snake[i].first == x && snake[i].second == y) {
            gameOver = true;
            break;
        }
    }

    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        snake.push_back({snake.back().first, snake.back().second});
    }
}

void GameLoop() {
    while (!gameOver) {
        mtx.lock();
        Draw();
        Input();
        Logic();
        mtx.unlock();
        Sleep(100);
    }
}
int main() {
    Setup();

    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(100);  // Controls the game speed
    }

    cout << "Game Over! Final Score: " << score << endl;
    return 0;
}
